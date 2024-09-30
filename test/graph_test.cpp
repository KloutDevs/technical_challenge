#include <gtest/gtest.h>
#include "../src/graph.h"
#include <fstream>
#include <cstdio>
#include <string>
#include <stdexcept>

// Prueba del constructor
TEST(GraphTest, Constructor) {
    Graph g(5);
    EXPECT_EQ(g.V, 5);
    EXPECT_EQ(g.adj.size(), 5);
    for (const auto& edges : g.adj) {
        EXPECT_TRUE(edges.empty());
    }
}

// Prueba del método addEdge
TEST(GraphTest, AddEdge) {
    Graph g(3);
    g.addEdge(0, 1, 10, 5);
    g.addEdge(1, 2, 20, 8);
    
    EXPECT_EQ(g.adj[0].size(), 1);
    EXPECT_EQ(g.adj[1].size(), 1);
    EXPECT_EQ(g.adj[2].size(), 0);
    
    EXPECT_EQ(g.adj[0][0].dest, 1);
    EXPECT_EQ(g.adj[0][0].cost, 10);
    EXPECT_EQ(g.adj[0][0].time, 5);
    
    EXPECT_EQ(g.adj[1][0].dest, 2);
    EXPECT_EQ(g.adj[1][0].cost, 20);
    EXPECT_EQ(g.adj[1][0].time, 8);
}

// Prueba del método fromFile con archivo válido
TEST(GraphTest, FromFileValid) {
    std::ofstream testFile("test_graph.txt");
    testFile << "3 2\n";
    testFile << "0 1 10 5\n";
    testFile << "1 2 20 8\n";
    testFile.close();
    
    Graph g = Graph::fromFile("test_graph.txt");
    
    EXPECT_EQ(g.V, 3);
    EXPECT_EQ(g.adj[0].size(), 1);
    EXPECT_EQ(g.adj[1].size(), 1);
    EXPECT_EQ(g.adj[2].size(), 0);
    
    EXPECT_EQ(g.adj[0][0].dest, 1);
    EXPECT_EQ(g.adj[0][0].cost, 10);
    EXPECT_EQ(g.adj[0][0].time, 5);
    
    EXPECT_EQ(g.adj[1][0].dest, 2);
    EXPECT_EQ(g.adj[1][0].cost, 20);
    EXPECT_EQ(g.adj[1][0].time, 8);
    
    std::remove("test_graph.txt");
}

// Prueba del método fromFile con archivo inválido
TEST(GraphTest, FromFileInvalid) {
    std::ofstream testFile("invalid_graph.txt");
    testFile << "Archivo de grafo inválido";
    testFile.close();
    
    EXPECT_THROW({
        Graph g = Graph::fromFile("invalid_graph.txt");
    }, std::runtime_error);
    
    // Limpia el archivo temporal
    std::remove("invalid_graph.txt");
}

// Prueba del método fromFile con archivo inexistente
TEST(GraphTest, FromFileNonexistent) {
    EXPECT_THROW({
        Graph g = Graph::fromFile("nonexistent_file.txt");
    }, std::runtime_error);
}

TEST(GraphTest, RemoveNegativeCycles) {
    Graph g(4);
    g.addEdge(0, 1, 1, 1);
    g.addEdge(1, 2, -3, 1);
    g.addEdge(2, 3, 1, 1);
    g.addEdge(3, 1, 1, 1);
    
    bool hasNegativeCycles = g.removeNegativeCycles();
    EXPECT_TRUE(hasNegativeCycles);
    EXPECT_EQ(g.adj[1].size(), 0);  // Nodo 1 debería haber sido eliminado
}

TEST(GraphTest, MultiObjectiveShortestPath) {
    Graph g(3);
    g.addEdge(0, 1, 2, 3);
    g.addEdge(0, 2, 3, 2);
    g.addEdge(1, 2, 1, 1);
    
    testing::internal::CaptureStdout();
    g.multiObjectiveShortestPath(0, 3);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("(Costo: 2, Tiempo: 3)") != std::string::npos);
    EXPECT_TRUE(output.find("(Costo: 3, Tiempo: 2)") != std::string::npos);
}

TEST(GraphTest, LargeGraph) {
    Graph g(1000);
    for (int i = 0; i < 999; i++) {
        g.addEdge(i, i+1, 1, 1);
    }
    g.addEdge(999, 0, 1, 1);
    
    EXPECT_NO_THROW({
        bool hasNegativeCycles = g.removeNegativeCycles();
        EXPECT_FALSE(hasNegativeCycles);
        g.multiObjectiveShortestPath(0, 10);
    });
}

TEST(GraphTest, DisconnectedGraph) {
    Graph g(4);
    g.addEdge(0, 1, 1, 1);
    g.addEdge(2, 3, 1, 1);
    
    testing::internal::CaptureStdout();
    g.multiObjectiveShortestPath(0, 4);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("No hay ruta") != std::string::npos);
}

TEST(GraphTest, InvalidInputs) {
    Graph g(5);
    EXPECT_THROW(g.addEdge(-1, 0, 1, 1), std::out_of_range);
    EXPECT_THROW(g.addEdge(0, 5, 1, 1), std::out_of_range);
    EXPECT_THROW(g.multiObjectiveShortestPath(-1, 5), std::out_of_range);
    EXPECT_THROW(g.multiObjectiveShortestPath(5, 5), std::out_of_range);
    EXPECT_THROW(g.multiObjectiveShortestPath(0, -1), std::invalid_argument);
}