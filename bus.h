#ifndef BUS_H
#define BUS_H

// 1. Definição do tipo da barra
typedef enum {
    SLACK = 0,
    PV = 1,
    PQ = 2
} BusType;

// 2. Estrutura da barra
typedef struct {
    int id;             // Identificador da barra
    BusType type;       // Tipo da barra (SLACK, PV ou PQ)
    
    double v;           // Magnitude da tensão (pu)
    double theta;       // Ângulo da tensão (rad)
    
    double p_spec;      // Potência ativa especificada (pu)
    double q_spec;      // Potência reativa especificada (pu)
    
    double p_calc;      // Potência ativa calculada (pu)
    double q_calc;      // Potência reativa calculada (pu)
} Bus;

// 3. Protótipos das funções que usam Bus
Bus* create_bus_system(int num_buses);
void free_bus_system(Bus *buses);

#endif // BUS_H