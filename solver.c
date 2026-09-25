#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "solver.h"
#include "linear_solver.h"

double calculate_power_mismatches(Bus *buses, Ybus *ybus, double *delta_p, double *delta_q) {
    int n = ybus->num_buses;
    double max_error = 0.0;

    for (int i = 0; i < n; i++) {
        buses[i].p_calc = 0.0;
        buses[i].q_calc = 0.0;

        for (int j = 0; j < n; j++) {
            double g_ij = creal(ybus->matrix[i][j]);
            double b_ij = cimag(ybus->matrix[i][j]);
            double theta_ij = buses[i].theta - buses[j].theta;

            buses[i].p_calc += buses[i].v * buses[j].v * (g_ij * cos(theta_ij) + b_ij * sin(theta_ij));
            buses[i].q_calc += buses[i].v * buses[j].v * (g_ij * sin(theta_ij) - b_ij * cos(theta_ij));
        }

        if (buses[i].type != SLACK) {
            delta_p[i] = buses[i].p_spec - buses[i].p_calc;
            if (fabs(delta_p[i]) > max_error) max_error = fabs(delta_p[i]);
        } else {
            delta_p[i] = 0.0;
        }

        if (buses[i].type == PQ) {
            delta_q[i] = buses[i].q_spec - buses[i].q_calc;
            if (fabs(delta_q[i]) > max_error) max_error = fabs(delta_q[i]);
        } else {
            delta_q[i] = 0.0;
        }
    }

    return max_error;
}

void run_power_flow(Bus *buses, int num_buses, Ybus *ybus, double tolerance, int max_iterations) {
    double *delta_p = (double*) calloc(num_buses, sizeof(double));
    double *delta_q = (double*) calloc(num_buses, sizeof(double));

    // Mapeamento dos índices das variáveis no vetor do sistema linear
    int *map_theta = (int*) malloc(num_buses * sizeof(int));
    int *map_v = (int*) malloc(num_buses * sizeof(int));

    int dim_theta = 0, dim_v = 0;

    for (int i = 0; i < num_buses; i++) {
        map_theta[i] = (buses[i].type != SLACK) ? dim_theta++ : -1;
    }

    for (int i = 0; i < num_buses; i++) {
        map_v[i] = (buses[i].type == PQ) ? (dim_theta + dim_v++) : -1;
    }

    int dim = dim_theta + dim_v;

    // Alocação da Jacobiana J, vetor b e vetor de correções x
    double **J = (double**) malloc(dim * sizeof(double*));
    for (int i = 0; i < dim; i++) J[i] = (double*) calloc(dim, sizeof(double));
    double *b = (double*) calloc(dim, sizeof(double));
    double *x = (double*) calloc(dim, sizeof(double));

    printf("\n===========================================================\n");
    printf("         INICIANDO ITERAÇÕES DO NEWTON-RAPHSON              \n");
    printf("===========================================================\n");

    for (int iter = 1; iter <= max_iterations; iter++) {
        double max_err = calculate_power_mismatches(buses, ybus, delta_p, delta_q);

        printf("Iteração %2d: Maior Mismatch = %.8f pu\n", iter, max_err);

        if (max_err < tolerance) {
            printf("\n Conversão atingida com sucesso em %d iterações!\n", iter);
            break;
        }

        // Vetor de resíduos b = [ΔP; ΔQ]
        for (int i = 0; i < num_buses; i++) {
            if (map_theta[i] != -1) b[map_theta[i]] = delta_p[i];
            if (map_v[i] != -1)     b[map_v[i]]     = delta_q[i];
        }

        // Montagem da Matriz Jacobiana J
        for (int i = 0; i < num_buses; i++) {
            for (int j = 0; j < num_buses; j++) {
                double g_ij = creal(ybus->matrix[i][j]);
                double b_ij = cimag(ybus->matrix[i][j]);
                double theta_ij = buses[i].theta - buses[j].theta;

                // Bloco H = dP/dtheta
                if (map_theta[i] != -1 && map_theta[j] != -1) {
                    int r = map_theta[i], c = map_theta[j];
                    if (i != j) {
                        J[r][c] = buses[i].v * buses[j].v * (g_ij * sin(theta_ij) - b_ij * cos(theta_ij));
                    } else {
                        double b_ii = cimag(ybus->matrix[i][i]);
                        J[r][c] = -buses[i].q_calc - b_ii * buses[i].v * buses[i].v;
                    }
                }

                // Bloco N = dP/dV
                if (map_theta[i] != -1 && map_v[j] != -1) {
                    int r = map_theta[i], c = map_v[j];
                    if (i != j) {
                        J[r][c] = buses[i].v * (g_ij * cos(theta_ij) + b_ij * sin(theta_ij));
                    } else {
                        double g_ii = creal(ybus->matrix[i][i]);
                        J[r][c] = (buses[i].p_calc / buses[i].v) + g_ii * buses[i].v;
                    }
                }

                // Bloco M = dQ/dtheta
                if (map_v[i] != -1 && map_theta[j] != -1) {
                    int r = map_v[i], c = map_theta[j];
                    if (i != j) {
                        J[r][c] = -buses[i].v * buses[j].v * (g_ij * cos(theta_ij) + b_ij * sin(theta_ij));
                    } else {
                        double g_ii = creal(ybus->matrix[i][i]);
                        J[r][c] = buses[i].p_calc - g_ii * buses[i].v * buses[i].v;
                    }
                }

                // Bloco L = dQ/dV
                if (map_v[i] != -1 && map_v[j] != -1) {
                    int r = map_v[i], c = map_v[j];
                    if (i != j) {
                        J[r][c] = buses[i].v * (g_ij * sin(theta_ij) - b_ij * cos(theta_ij));
                    } else {
                        double b_ii = cimag(ybus->matrix[i][i]);
                        J[r][c] = (buses[i].q_calc / buses[i].v) - b_ii * buses[i].v;
                    }
                }
            }
        }

        // Resolver J * dx = b
        if (!solve_linear_system(dim, J, b, x)) {
            printf("Erro: Matriz Jacobiana singular!\n");
            break;
        }

        // Atualizar variáveis de estado (θ e |V|)
        for (int i = 0; i < num_buses; i++) {
            if (map_theta[i] != -1) buses[i].theta += x[map_theta[i]];
            if (map_v[i] != -1)     buses[i].v     += x[map_v[i]];
        }
    }

    // Atualiza potências finais calculadas
    calculate_power_mismatches(buses, ybus, delta_p, delta_q);

    // Liberação de memória
    free(delta_p); free(delta_q);
    free(map_theta); free(map_v);
    for (int i = 0; i < dim; i++) free(J[i]);
    free(J); free(b); free(x);
}