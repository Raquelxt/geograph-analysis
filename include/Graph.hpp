#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <unordered_map>

class Graph {
private:
    std::unordered_map<std::string, std::vector<std::string>> adjacencyList;

    std::unordered_map<std::string, std::vector<std::string>> generateSubgraph(const std::vector<std::string>& vertices);
    std::unordered_map<std::string, int> calculateBalance(const std::unordered_map<std::string, std::vector<std::string>>& subgraph);
    void transformToEulerian(std::unordered_map<std::string, std::vector<std::string>>& subgraph, std::unordered_map<std::string, int>& balance);
    int bfsDistance(const std::unordered_map<std::string, std::vector<std::string>>& graph, const std::string& source, const std::string& target);
    std::vector<std::string> findEulerianCycle(const std::unordered_map<std::string, std::vector<std::string>>& subgraph);

public:
    void addNode(const std::string& location);
    void addEdge(const std::string& from, const std::string& to);
    std::string findCapital();
    std::vector<std::string> findSecondaryBattalions();
    std::vector<std::vector<std::string>> findPatrolRoutes();
    std::vector<std::vector<std::string>> findConnectedComponents();
    std::unordered_map<std::string, std::vector<std::string>> transposeGraph();
};

class HungarianAlgorithm {
public:
    std::vector<int> solve(const std::vector<std::vector<int>>& costMatrix);
};

#endif
