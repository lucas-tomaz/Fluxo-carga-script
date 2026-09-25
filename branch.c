#include <stdio.h>
#include <stdlib.h>
#include "branch.h"

Branch* create_branch_system(int num_branches) {
    if (num_branches <= 0) return NULL;

    Branch *branches = (Branch*) calloc(num_branches, sizeof(Branch));
    if (branches == NULL) {
        printf("Erro: Falha na alocação de memória para os ramos.\n");
        exit(EXIT_FAILURE);
    }

    return branches;
}

void free_branch_system(Branch *branches) {
    if (branches != NULL) {
        free(branches);
    }
}