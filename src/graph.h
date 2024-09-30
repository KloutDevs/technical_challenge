#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

struct Edge {
    int dest;
    int cost;
    int time;
    Edge(int d, int c, int t) : dest(d), cost(c), time(t) {}
};

struct Label
{
    int node;
    long long cost;
    long long time;
    Label(int n, long long c, long long t) : node(n), cost(c), time(t) {}
    bool operator>(const Label &other) const
    {
        if (cost != other.cost)
            return cost > other.cost;
        return time > other.time;
    }
};

class Graph {
public:
    int V;
    std::vector<std::vector<Edge>> adj;

    Graph(int nodes);
    void addEdge(int u, int v, int cost, int time);
    static Graph fromFile(const std::string &filename);
    bool removeNegativeCycles();
    void multiObjectiveShortestPath(int src, int maxShow);

private:
    void markNegativeCycleNodes(int startNode, const std::vector<int> &predecessor, std::vector<bool> &inNegativeCycle);
};

#endif