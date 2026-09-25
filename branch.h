#ifndef BRANCH_H
#define BRANCH_H

typedef struct {
    int from;        // Ímã/Índice da barra de origem
    int to;          // Índiceda barra de destino
    
    double r;        // Resistência série em pu
    double x;        // Reatância série em pu
    double b_sh;     // Susceptância shunt total em pu
    double tap;      // Relatório de transformação (1.0 para linhas comuns)
} Branch;

Branch* create_branch_system(int num_branches);
void free_branch_system(Branch *branches);

#endif // BRANCH_H