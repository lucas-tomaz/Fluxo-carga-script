#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bus.h"
#include "branch.h"
#include "ybus.h"
#include "linear_solver.h"
#include "solver.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    int num_buses = 3;
    int num_branches = 3;

    // 1. Instanciação das Barras
    Bus *buses = create_bus_system(num_buses);

    // parâmetros: .v(tensão em pu); .theta(fase da barra); .p_spec(potencia ativa da barra); .q_spec(potencia reativa da barra)

    // Barra 1: SLACK (Gerador 1 + T1)
    buses[0] = (Bus){.id = 1, .type = SLACK, .v = 1.05, .theta = 0.0, .p_spec = 0.0, .q_spec = 0.0};

    // Barra 2: PV (Gerador 2 + T2 - Injetando P = 0.80 pu com |V| = 1.03 pu)
    buses[1] = (Bus){.id = 2, .type = PV,    .v = 1.03, .theta = 0.0, .p_spec = 0.80, .q_spec = 0.0};

    // Barra 3: PQ (Barra de Carga alimentada pelo sistema - P = -1.20 pu, Q = -0.50 pu)
    buses[2] = (Bus){.id = 3, .type = PQ,    .v = 1.00, .theta = 0.0, .p_spec = -1.20, .q_spec = -0.50};

    // 2. Instanciação das Linhas de Transmissão (1-2, 1-3, 2-3)
    Branch *branches = create_branch_system(num_branches);

    branches[0] = (Branch){.from = 1, .to = 2, .r = 0.02, .x = 0.08, .b_sh = 0.020, .tap = 1.0};
    branches[1] = (Branch){.from = 1, .to = 3, .r = 0.01, .x = 0.05, .b_sh = 0.015, .tap = 1.0};
    branches[2] = (Branch){.from = 2, .to = 3, .r = 0.03, .x = 0.10, .b_sh = 0.025, .tap = 1.0};

    // 3. Resolução do Fluxo de Carga
    Ybus *ybus = create_ybus(num_buses);
    build_ybus(ybus, branches, num_branches);
    run_power_flow(buses, num_buses, ybus, 1e-6, 20);



    // 4. Exibição no Console
    printf("\n=========================================================================\n");
    printf("                  RESULTADOS DO FLUXO DE CARGA (6 BARRAS)               \n");
    printf("=========================================================================\n");
    printf(" Barra | Tipo  | V (pu)   | Ângulo (deg) | P_calc (pu)  | Q_calc (pu)   \n");
    printf("-------------------------------------------------------------------------\n");

    double rad2deg = 180.0 / M_PI;
    for (int i = 0; i < num_buses; i++) {
        const char* type_str = (buses[i].type == SLACK) ? "SLACK" :
                               (buses[i].type == PV)    ? "PV   " : "PQ   ";
        printf("  %2d   | %s | %8.4f | %12.4f | %12.4f | %12.4f \n",
               buses[i].id, type_str, buses[i].v, buses[i].theta * rad2deg,
               buses[i].p_calc, buses[i].q_calc);
    }
    printf("=========================================================================\n");

    // 5. Exportação das BARRAS (Feita ANTES da liberação da memória)
    FILE *fp_buses = fopen("./data/buses.csv", "w");
    if (fp_buses != NULL) {
        fprintf(fp_buses, "id,type,v,theta_deg,p,q\n");
        for (int i = 0; i < num_buses; i++) {
            const char* type_str = (buses[i].type == SLACK) ? "SLACK" :
                                   (buses[i].type == PV)    ? "PV"    : "PQ";
            fprintf(fp_buses, "%d,%s,%.4f,%.4f,%.4f,%.4f\n",
                    buses[i].id, type_str, buses[i].v,
                    buses[i].theta * (180.0 / M_PI),
                    buses[i].p_calc, buses[i].q_calc);
        }
        fclose(fp_buses);
    }

    // 6. Exportação das LINHAS (Para o Python ligar as barras automaticamente)
    FILE *fp_branches = fopen("./data/branches.csv", "w");
    if (fp_branches != NULL) {
        fprintf(fp_branches, "from,to\n");
        for (int i = 0; i < num_branches; i++) {
            fprintf(fp_branches, "%d,%d\n", branches[i].from, branches[i].to);
        }
        fclose(fp_branches);
    }

    // 6. Liberação da Memória
    free_bus_system(buses);
    free_branch_system(branches);
    free_ybus(ybus);

    // 7. Chamada do Script Python
    #ifdef _WIN32
        system("python scripts/plot_grid.py");
    #else
        system("python3 scripts/plot_grid.py");
    #endif

    return 0;
}