#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class Graph {
public:
    // Métodos básicos de construção do grafo
    void addNode(const std::string& location);
    void addEdge(const std::string& from, const std::string& to);

    // Algoritmos e funcionalidades específicas
    std::string findCapital();
    std::vector<std::string> findSecondaryBattalions();
    std::vector<std::vector<std::string>> findPatrolRoutes();
    std::vector<std::vector<std::string>> findConnectedComponents();

private:
    // Estrutura principal de representação do grafo
    std::unordered_map<std::string, std::vector<std::string>> adjacencyList;

    // Métodos auxiliares
    std::unordered_map<std::string, std::vector<std::string>> transposeGraph();
    std::unordered_map<std::string, int> calculateBalance(const std::unordered_map<std::string, std::vector<std::string>>& subgraph);
    std::unordered_map<std::string, std::vector<std::string>> generateSubgraph(const std::vector<std::string>& vertices);
    int bfsDistance(const std::unordered_map<std::string, std::vector<std::string>>& graph, const std::string& source, const std::string& target);
    void transformToEulerian(std::unordered_map<std::string, std::vector<std::string>>& subgraph, std::unordered_map<std::string, int>& balance);
    std::vector<std::string> findEulerianCycleFromStart(const std::unordered_map<std::string, std::vector<std::string>>& subgraph, const std::string& startNode);
    std::vector<std::string> findEulerianCycle(const std::unordered_map<std::string, std::vector<std::string>>& subgraph);
};

#endif // GRAPH_HPP
