#ifndef YBUS_H
#define YBUS_H

#include <complex.h>
#include "bus.h"
#include "branch.h"

typedef struct {
    int num_buses;
    double complex **matrix; // Matriz 2D de números complexos (G + jB)
} Ybus;

Ybus* create_ybus(int num_buses);
void build_ybus(Ybus *ybus, Branch *branches, int num_branches);
void free_ybus(Ybus *ybus);

#endif // YBUS_H