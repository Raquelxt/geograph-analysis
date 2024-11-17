#include "Graph.hpp"
#include "Utils.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <map>

int main() {
    Graph graph;
    std::map<std::string, int> nameToIndex;
    std::vector<std::string> indexToName;

    int NC, NE;
    std::cin >> NC >> NE;

    // Lê os nós e arestas do grafo
    for (int i = 0; i < NE; ++i) {
        std::string from, to;
        std::cin >> from >> to;

        if (nameToIndex.find(from) == nameToIndex.end()) {
            nameToIndex[from] = indexToName.size();
            indexToName.push_back(from);
            graph.addNode(from);
        }

        if (nameToIndex.find(to) == nameToIndex.end()) {
            nameToIndex[to] = indexToName.size();
            indexToName.push_back(to);
            graph.addNode(to);
        }

        graph.addEdge(from, to);
    }

    // Imprime a Capital
    std::string capital = graph.findCapital();
    std::cout << capital << std::endl;

    // Imprime os Batalhões Secundários
    auto secondaryBattalions = graph.findSecondaryBattalions();
    std::cout << secondaryBattalions.size() << std::endl;
    for (const auto& battalion : secondaryBattalions) {
        std::cout << battalion << std::endl;
    }

    // Imprime as Rotas de Patrulhamento
    auto patrolRoutes = graph.findPatrolRoutes();
    std::cout << patrolRoutes.size() << std::endl;
    for (const auto& route : patrolRoutes) {
        for (size_t i = 0; i < route.size(); ++i) {
            std::cout << route[i];
            if (i < route.size() - 1) std::cout << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
