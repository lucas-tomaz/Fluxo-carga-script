#include <stdio.h>
#include <stdlib.h>
#include "bus.h"

Bus* create_bus_system(int num_buses) {
    if (num_buses <= 0) return NULL;

    // Aloca a memória zera todos os bytes
    Bus *buses = (Bus*) calloc(num_buses, sizeof(Bus));
    
    if (buses == NULL) {
        printf("Erro: Falha na alocação de memória para as barras.\n");
        exit(EXIT_FAILURE);
    }

    return buses;
}

void free_bus_system(Bus *buses) {
    if (buses != NULL) {
        free(buses);
    }
}