#include "graph.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cout << "Por favor, proporciona la opción del grafo y el nodo origen." << endl;
        cout << "Ejemplo: ./main 5 0 10" << endl;
        cout << endl;
        cout << "Opciones disponibles:" << endl;
        cout << "1) Grafo simple con tiempos" << endl;
        cout << "2) Grafo de prueba sin diferencia en tiempos" << endl;
        cout << "3) Grafo con 1 solo ciclo negativo" << endl;
        cout << "4) Grafo de ejemplo base" << endl;
        cout << "5) Grafo generado" << endl;
        cout << "6) Grafo extenso solicitado" << endl;
        return 1;
    }

    int optionArg = atoi(argv[1]);
    int src = atoi(argv[2]);
    int maxShow = atoi(argv[3]);
    Graph g(0);

    cout << "Opción: " << optionArg << endl;
    cout << "Resultados del algoritmo para el origen: " << src << endl;

    try {
        switch (optionArg)
        {
        case 1: // Grafo simple con tiempos
        {
            g = Graph(5);
            g.addEdge(0, 1, 10, 5);
            g.addEdge(0, 2, 3, 2);
            g.addEdge(0, 3, 4, 1);
            g.addEdge(1, 2, 1, 4);
            g.addEdge(1, 3, 2, 6);
            g.addEdge(2, 1, 4, 1);
            g.addEdge(2, 3, 8, 3);
            g.addEdge(2, 4, 2, 3);
            g.addEdge(3, 4, 7, 4);
            g.addEdge(3, 1, 4, 1);
            break;
        }
        case 2: // Grafo de prueba sin diferencia en tiempos
        {
            g = Graph(5);
            g.addEdge(0, 1, 2, 1);
            g.addEdge(1, 2, -1, 1);
            g.addEdge(2, 3, -2, 1);
            g.addEdge(3, 4, -3, 1);
            g.addEdge(4, 0, 5, 1);
            break;
        }
        case 3: // Grafo con 1 solo ciclo negativo
        {
            g = Graph(10);
            g.addEdge(0, 1, -1, 1);
            g.addEdge(1, 2, -2, 1);
            g.addEdge(2, 0, 2, 1);
            g.addEdge(2, 3, -3, 1);
            g.addEdge(3, 4, -1, 1);
            g.addEdge(4, 2, -4, 1);
            g.addEdge(5, 6, -1, 1);
            g.addEdge(5, 7, 4, 1);
            g.addEdge(6, 7, 3, 1);
            g.addEdge(6, 8, 2, 1);
            g.addEdge(6, 9, 2, 1);
            g.addEdge(8, 7, 5, 1);
            g.addEdge(8, 6, 1, 1);
            g.addEdge(9, 8, -3, 1);
            break;
        }
        case 4: // Grafo de ejemplo base
        {
            g = Graph(5);
            g.addEdge(0, 1, -1, 5);
            g.addEdge(0, 2, 4, 2);
            g.addEdge(1, 2, 3, 2);
            g.addEdge(1, 3, 2, 1);
            g.addEdge(1, 4, 2, 3);
            g.addEdge(3, 2, 5, 2);
            g.addEdge(3, 1, 1, 1);
            g.addEdge(4, 3, -3, 2);
            break;
        }
        case 5: // Grafo generado
        {
            g = Graph::fromFile("graphs/generated_graph.txt");
            break;
        }
        case 6: // Grafo extenso solicitado
        {
            g = Graph::fromFile("graphs/large_graph.txt");
            break;
        }
        default:
            cout << "Opción no válida" << endl;
            return 1;
        }

        bool hasNegativeCycles = g.removeNegativeCycles();
        if (hasNegativeCycles)
        {
            cout << "Se han eliminado los ciclos negativos del grafo." << endl;
        }

        clock_t start_time = clock();
        g.multiObjectiveShortestPath(src, maxShow);
        clock_t end_time = clock();
        double elapsed_time = double(end_time - start_time) / CLOCKS_PER_SEC;
        cout << "Tiempo de ejecución: " << elapsed_time << " segundos" << endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}