#ifndef LINEAR_SOLVER_H
#define LINEAR_SOLVER_H

// Resolve Ax = b via Eliminação Gaussiana com Pivoteamento Parcial.
// Retorna 1 se tiver sucesso ou 0 se a matriz for singular.
int solve_linear_system(int n, double **A, double *b, double *x);

#endif // LINEAR_SOLVER_H