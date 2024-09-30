#include "graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <climits>
#include <set>
#include <queue>
#include <functional>

using namespace std;

Graph::Graph(int nodes) : V(nodes), adj(nodes) {}

void Graph::addEdge(int u, int v, int cost, int time)
{
    if (u < 0 || u >= V || v < 0 || v >= V) {
        throw std::out_of_range("Índice de nodo fuera de rango");
    }
    adj[u].emplace_back(v, cost, time);
}

Graph Graph::fromFile(const string &filename)
{
    ifstream infile(filename);
    if (!infile)
    {
        throw runtime_error("No se pudo abrir el archivo " + filename);
    }

    string line;
    if (!getline(infile, line))
    {
        throw runtime_error("El archivo está vacío o no se pudo leer la primera línea");
    }

    istringstream iss(line);
    int num_nodes, num_edges;
    if (!(iss >> num_nodes >> num_edges))
    {
        throw runtime_error("Formato inválido en la primera línea del archivo");
    }

    Graph graph(num_nodes);

    int edge_count = 0;
    while (getline(infile, line))
    {
        istringstream iss(line);
        int u, v, cost, time;
        if (!(iss >> u >> v >> cost >> time))
        {
            throw runtime_error("Formato inválido en la línea " + to_string(edge_count + 2) + " del archivo");
        }
        graph.addEdge(u, v, cost, time);
        edge_count++;
    }

    if (edge_count != num_edges)
    {
        throw runtime_error("El número de aristas no coincide con lo especificado en la primera línea");
    }

    infile.close();
    return graph;
}

bool Graph::removeNegativeCycles()
{
    vector<long long> dist_cost(V, 0);
    vector<long long> dist_time(V, 0);
    vector<int> predecessor(V, -1);
    vector<bool> inNegativeCycle(V, false);
    vector<bool> in_queue(V, false);
    vector<int> count(V, 0);

    deque<int> q;
    for (int i = 0; i < V; ++i)
    {
        q.push_back(i);
        in_queue[i] = true;
    }

    bool hasNegativeCycle = false;
    int iterations = 0;
    const int MAX_ITERATIONS = V * 2;

    while (!q.empty() && iterations < MAX_ITERATIONS)
    {
        int u = q.front();
        q.pop_front();
        in_queue[u] = false;

        for (const auto &edge : adj[u])
        {
            int v = edge.dest;
            long long new_cost = dist_cost[u] + edge.cost;
            long long new_time = dist_time[u] + edge.time;

            if (new_cost < dist_cost[v] || (new_cost == dist_cost[v] && new_time < dist_time[v]))
            {
                dist_cost[v] = new_cost;
                dist_time[v] = new_time;
                predecessor[v] = u;

                if (!in_queue[v])
                {
                    if (!q.empty() && (new_cost < dist_cost[q.front()] ||
                                       (new_cost == dist_cost[q.front()] && new_time < dist_time[q.front()])))
                    {
                        q.push_front(v);
                    }
                    else
                    {
                        q.push_back(v);
                    }
                    in_queue[v] = true;
                }

                if (++count[v] > V)
                {
                    hasNegativeCycle = true;
                    markNegativeCycleNodes(v, predecessor, inNegativeCycle);
                }
            }
        }

        iterations++;
    }

    // Verificación adicional para asegurar la detección de todos los ciclos negativos
    for (int u = 0; u < V; ++u)
    {
        for (const auto &edge : adj[u])
        {
            int v = edge.dest;
            if (dist_cost[u] + edge.cost < dist_cost[v] ||
                (dist_cost[u] + edge.cost == dist_cost[v] && dist_time[u] + edge.time < dist_time[v]))
            {
                hasNegativeCycle = true;
                markNegativeCycleNodes(v, predecessor, inNegativeCycle);
            }
        }
    }

    if (hasNegativeCycle)
    {
        cout << "Se detectaron ciclos negativos. Eliminando nodos y aristas involucrados." << endl;

        for (int u = 0; u < V; ++u)
        {
            if (inNegativeCycle[u])
            {
                adj[u].clear();
                cout << "Nodo " << u << " eliminado del grafo." << endl;
            }
            else
            {
                adj[u].erase(remove_if(adj[u].begin(), adj[u].end(),
                                       [&inNegativeCycle](const Edge &edge)
                                       { return inNegativeCycle[edge.dest]; }),
                             adj[u].end());
            }
        }
    }
    else
    {
        cout << "No se encontraron ciclos negativos." << endl;
    }

    return hasNegativeCycle;
}

void Graph::markNegativeCycleNodes(int startNode, const vector<int> &predecessor, vector<bool> &inNegativeCycle)
{
    vector<bool> visited(V, false);
    vector<int> cycle;
    int curr = startNode;

    while (!visited[curr])
    {
        visited[curr] = true;
        cycle.push_back(curr);
        curr = predecessor[curr];
        if (curr == -1 || curr == startNode)
            break;
    }

    if (curr != -1 && curr == startNode)
    {
        for (int node : cycle)
        {
            inNegativeCycle[node] = true;
        }
    }
}

void Graph::multiObjectiveShortestPath(int src, int maxShow)
{
    if (src < 0 || src >= V) {
        throw std::out_of_range("Nodo de origen fuera de rango");
    }
    if (maxShow < 0) {
        throw std::invalid_argument("maxShow no debe ser negativo");
    }
    vector<vector<pair<long long, long long>>> solutions(V);
    solutions[src].emplace_back(0, 0);

    priority_queue<Label, vector<Label>, greater<Label>> pq;
    pq.emplace(src, 0, 0);

    vector<bool> visited(V, false);

    while (!pq.empty())
    {
        Label current = pq.top();
        pq.pop();

        int u = current.node;
        long long current_cost = current.cost;
        long long current_time = current.time;

        if (visited[u])
            continue;
        visited[u] = true;

        for (const Edge &edge : adj[u])
        {
            int v = edge.dest;
            long long new_cost = current_cost + edge.cost;
            long long new_time = current_time + edge.time;

            bool is_dominated = false;
            for (const auto &sol : solutions[v])
            {
                if (sol.first <= new_cost && sol.second <= new_time)
                {
                    is_dominated = true;
                    break;
                }
            }

            if (!is_dominated)
            {
                solutions[v].emplace_back(new_cost, new_time);
                pq.emplace(v, new_cost, new_time);

                auto it = remove_if(solutions[v].begin(), solutions[v].end(),
                                    [new_cost, new_time](const pair<long long, long long> &sol)
                                    {
                                        return new_cost <= sol.first && new_time <= sol.second &&
                                               (new_cost < sol.first || new_time < sol.second);
                                    });
                solutions[v].erase(it, solutions[v].end());
            }
        }
    }

    cout << "Resultados de optimización (Costo, Tiempo) desde el nodo " << src << ":\n";
    for (int i = 0; i < min(maxShow, V); i++)
    {
        cout << "Ruta hacia el nodo " << i << ":\n";
        if (solutions[i].empty())
        {
            cout << "  No hay ruta\n";
        }
        else
        {
            sort(solutions[i].begin(), solutions[i].end());

            // Find the solution with the lowest cost and the solution with the lowest time
            auto lowest_cost = solutions[i].front();
            auto lowest_time = *min_element(solutions[i].begin(), solutions[i].end(),
                                            [](const auto &a, const auto &b)
                                            { return a.second < b.second; });

            // Display the lowest cost solution
            cout << "  (Costo: " << lowest_cost.first << ", Tiempo: " << lowest_cost.second << ")   - Menor costo\n";
            if (lowest_time != lowest_cost)
            {
                cout << "  (Costo: " << lowest_time.first << ", Tiempo: " << lowest_time.second << ")   - Menor tiempo\n";
            }
        }
    }
}