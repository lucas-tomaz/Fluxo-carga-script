#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "linear_solver.h"

int solve_linear_system(int n, double **A, double *b, double *x) {
    // Eliminação direta com pivoteamento
    for (int i = 0; i < n; i++) {
        // Encontra o pivô máximo na coluna i
        int max_row = i;
        for (int k = i + 1; k < n; k++) {
            if (fabs(A[k][i]) > fabs(A[max_row][i])) {
                max_row = k;
            }
        }

        // Troca de linhas na matriz A e no vetor b
        if (max_row != i) {
            double *temp_row = A[i];
            A[i] = A[max_row];
            A[max_row] = temp_row;

            double temp_b = b[i];
            b[i] = b[max_row];
            b[max_row] = temp_b;
        }

        // Verificação de singularidade
        if (fabs(A[i][i]) < 1e-12) {
            return 0; // Matriz singular
        }

        // Eliminação
        for (int k = i + 1; k < n; k++) {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j < n; j++) {
                A[k][j] -= factor * A[i][j];
            }
            b[k] -= factor * b[i];
        }
    }

    // Substituição regressiva
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }

    return 1;
}