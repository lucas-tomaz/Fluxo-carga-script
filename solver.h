#ifndef SOLVER_H
#define SOLVER_H

#include "bus.h"
#include "branch.h"
#include "ybus.h"

// Calcula P_calc e Q_calc para todas as barras e retorna o erro máximo (|ΔP|, |ΔQ|)
double calculate_power_mismatches(Bus *buses, Ybus *ybus, double *delta_p, double *delta_q);

// solver.h (Adicionar ao arquivo existente)
void run_power_flow(Bus *buses, int num_buses, Ybus *ybus, double tolerance, int max_iterations);

#endif // SOLVER_H