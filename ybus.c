#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include "ybus.h"

Ybus* create_ybus(int num_buses) {
    Ybus *ybus = (Ybus*) malloc(sizeof(Ybus));
    ybus->num_buses = num_buses;
    
    // Alocação dinâmica da matriz 2D
    ybus->matrix = (double complex**) malloc(num_buses * sizeof(double complex*));
    for (int i = 0; i < num_buses; i++) {
        ybus->matrix[i] = (double complex*) calloc(num_buses, sizeof(double complex));
    }
    return ybus;
}

void build_ybus(Ybus *ybus, Branch *branches, int num_branches) {
    for (int k = 0; k < num_branches; k++) {
        int i = branches[k].from - 1; // Ajuste para índice base 0
        int j = branches[k].to - 1;   // Ajuste para índice base 0
        
        double r = branches[k].r;
        double x = branches[k].x;
        double b_sh = branches[k].b_sh;
        
        // Impedância e admitância série: y = 1 / (r + jx)
        double complex z = r + I * x;
        double complex y = 1.0 / z;
        double complex y_sh = I * (b_sh / 2.0); // Susceptância shunt metade para cada extremidade
        
        // Termos fora da diagonal principal (Yij = Yji = -y)
        ybus->matrix[i][j] -= y;
        ybus->matrix[j][i] -= y;
        
        // Termos da diagonal principal (Yii += y + y_sh)
        ybus->matrix[i][i] += y + y_sh;
        ybus->matrix[j][j] += y + y_sh;
    }
}

void free_ybus(Ybus *ybus) {
    if (ybus != NULL) {
        for (int i = 0; i < ybus->num_buses; i++) {
            free(ybus->matrix[i]);
        }
        free(ybus->matrix);
        free(ybus);
    }
}