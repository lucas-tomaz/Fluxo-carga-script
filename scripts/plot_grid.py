import sys

try:
    import pandas as pd
    import matplotlib.pyplot as plt
    import networkx as nx
except ImportError as e:
    print(f"\n[ERRO PYTHON] Biblioteca não encontrada: {e}")
    print("Instale executando: pip install pandas matplotlib networkx\n")
    sys.exit(1)

def plot_power_flow():
    try:
        buses_df = pd.read_csv("./data/buses.csv")
        branches_df = pd.read_csv("./data/branches.csv")
    except Exception as e:
        print(f"\n[ERRO PYTHON] Não foi possível ler os arquivos CSV: {e}\n")
        sys.exit(1)

    color_map = {"SLACK": "#e74c3c", "PV": "#2ecc71", "PQ": "#3498db"}
    G = nx.Graph()

    for _, row in buses_df.iterrows():
        bus_id = int(row["id"])
        G.add_node(bus_id,
                   type=row["type"],
                   v=row["v"],
                   theta=row["theta_deg"],
                   color=color_map.get(row["type"], "#95a5a6"))

    for _, row in branches_df.iterrows():
        G.add_edge(int(row["from"]), int(row["to"]))

    pos = nx.spring_layout(G, seed=42, k=1.2)

    plt.figure(figsize=(10, 8))

    node_colors = [G.nodes[n]["color"] for n in G.nodes()]
    nx.draw_networkx_nodes(G, pos, node_size=2500, node_color=node_colors, edgecolors="black", linewidths=2)
    nx.draw_networkx_edges(G, pos, width=2, edge_color="#34495e")

    labels = {n: f"Barra {n}\n({G.nodes[n]['type']})" for n in G.nodes()}
    nx.draw_networkx_labels(G, pos, labels=labels, font_size=9, font_weight="bold", font_color="white")

    for node, (x, y) in pos.items():
        v_val = G.nodes[node]["v"]
        t_val = G.nodes[node]["theta"]
        info = f"|V| = {v_val:.4f} pu\nθ = {t_val:.2f}°"
        plt.text(x, y + 0.12, info, fontsize=8, ha="center", va="center",
                 bbox=dict(boxstyle="round,pad=0.3", facecolor="#f8f9fa", edgecolor="#b2bec3", alpha=0.9))

    plt.title("Diagrama Dinâmico do Fluxo de Carga", fontsize=14, fontweight="bold")
    plt.axis("off")
    plt.tight_layout()

    # Salva o arquivo de imagem no disco
    output_file = "./scripts/grid_diagram.png"
    plt.savefig(output_file, dpi=300)
    print(f"\n[SUCESSO] Imagem gerada com sucesso: {output_file}\n")

if __name__ == "__main__":
    plot_power_flow()