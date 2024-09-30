import random
import os
import sys

def generate_large_graph(num_nodes, num_edges, filename, minWeight, maxWeight, negative_cycle=False):
    with open(filename, 'w') as f:
        f.write(f"{num_nodes} {num_edges}\n")
        
        edges = set()
        
        if negative_cycle:
            while len(edges) < num_edges - 3: 
                u = random.randint(0, num_nodes - 1)
                v = random.randint(0, num_nodes - 1)
                if u != v and (u, v) not in edges:
                    weight = random.randint(minWeight, maxWeight)
                    edges.add((u, v))
                    f.write(f"{u} {v} {weight}\n")
            
            cycle_nodes = random.sample(range(num_nodes), 3)
            weight1 = random.randint(minWeight, maxWeight)
            weight2 = random.randint(minWeight, maxWeight)
            total_weight = weight1 + weight2 - 1
            weight3 = -total_weight
            edges.update([
                (cycle_nodes[0], cycle_nodes[1]),
                (cycle_nodes[1], cycle_nodes[2]),
                (cycle_nodes[2], cycle_nodes[0]),
            ])
            weight4 = random.randint(minWeight, maxWeight)
            f.write(f"{cycle_nodes[0]} {cycle_nodes[1]} {weight1} {weight4}\n")
            f.write(f"{cycle_nodes[1]} {cycle_nodes[2]} {weight2} {weight4}\n")
            f.write(f"{cycle_nodes[2]} {cycle_nodes[0]} {weight3} {weight4}\n")
        else:
            order = list(range(num_nodes))
            random.shuffle(order)
            for i in range(num_nodes):
                u = order[i]
                num_edges_from_u = random.randint(1, min(num_nodes - i - 1, num_edges - len(edges)))
                targets = random.sample(order[i+1:], num_edges_from_u)
                for v in targets:
                    if (u, v) not in edges and u != v:
                        weight = random.randint(minWeight, maxWeight)
                        weight2 = random.randint(minWeight, maxWeight)
                        edges.add((u, v))
                        f.write(f"{u} {v} {weight} {weight2}\n")
                    if len(edges) >= num_edges:
                        break
                if len(edges) >= num_edges:
                    break

    print(f"Grafo generado y guardado en {filename}")
        
if len(sys.argv) < 7:
    print("Uso: python generateGraph.py num_nodos num_aristas peso_min peso_max ruta_completa ciclos_negativos")
    print("ciclos_negativos: 'True' o 'False'")
    print("Ejemplo: python generateGraph.py 10 100 -10 10 ../graphs/generated_graph.txt True")
    sys.exit(1)
    
nodos = int(sys.argv[1])
aristas = int(sys.argv[2])
minWeight = int(sys.argv[3])
maxWeight = int(sys.argv[4])
filename = sys.argv[5]
directory = os.path.dirname(filename)
if directory and not os.path.exists(directory):
    print(f"El directorio {directory} no existe. Por favor, créalo antes de ejecutar el script.")
    sys.exit(1)
negative_cycle_input = sys.argv[6].lower()

if negative_cycle_input == 'true':
    negative_cycle = True
elif negative_cycle_input == 'false':
    negative_cycle = False
else:
    print(f"Grafo generado y guardado en {os.path.abspath(filename)}")
    sys.exit(1)

generate_large_graph(nodos, aristas, filename, minWeight, maxWeight, negative_cycle)