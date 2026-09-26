# ⚡ Fluxo de Carga - Análise de Sistemas Elétricos de Potência

Algoritmo desenvolvido para a execução e cálculo de **Fluxo de Carga** em sistemas elétricos de potência, permitindo a análise do perfil de tensão nas barras, trânsito de potência ativa e reativa, e perdas na rede.

---

## 📌 Sobre o Projeto

O estudo de fluxo de carga é fundamental para o planejamento e operação de sistemas elétricos de potência. Este script realiza a formulação das equações de rede e aplica métodos numéricos para determinar o estado do sistema em regime permanente.

### ✨ Funcionalidades Principais
- [x] Montagem automática da **Matriz de Admitância Nodal ($Y_{bus}$)**
- [x] Classificação e tratamento das barras do sistema (**Slack/V-θ**, **PV**, **PQ**)
- [x] Resolução do fluxo de potência via métodos iterativos (*Newton-Raphson*)
- [x] Cálculo do fluxo de potência ativa ($P$) e reativa ($Q$) nos ramos/linhas de transmissão
- [x] Levantamento de perdas elétricas na rede
- [ ] Exportação ou visualização gráfica dos resultados (módulos de tensão e ângulos)

---

## 🛠️ Tecnologias Utilizadas

* **Linguagem Principal:** C e Python
* **Bibliotecas / Bibliotecas de Cálculo:** `NumPy`, `Pandas` , `math`.
* **Visualização:** `Matplotlib`

---

## 📊 Estrutura de Entrada e Saída

### Dados do Sistema (Exemplo)
O script processa tabelas contendo as barras e linhas do sistema elétrico:

| Barra | Tipo | $V$ (p.u.) | $\theta$ (°) | $P_g$ (p.u.) | $Q_g$ (p.u.) | $P_d$ (p.u.) | $Q_d$ (p.u.) |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 1 | Slack | 1.00 | 0.0 | - | - | 0.0 | 0.0 |
| 2 | PV | 1.02 | - | 0.5 | - | 0.0 | 0.0 |
| 3 | PQ | - | - | 0.0 | 0.0 | 0.6 | 0.25 |

---

## 🚀 Como Executar o Projeto

### Pré-requisitos
* Python 3.8+ instalado (ou o ambiente de execução correspondente)

### Passo a passo

1. **Clone o repositório:**
   ```bash
   git clone [https://github.com/lucas-tomaz/Fluxo-carga-script.git](https://github.com/lucas-tomaz/Fluxo-carga-script.git)

2. **Compilando o exemplo `main.c`**
    Compile todos os arquivos:
    ```bash
      gcc *.c -o fluxo_carga -lm

3. **Acesse o resultado do diagrama**
   Acesse a pasta: /scripts 
