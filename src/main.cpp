#include "Graph.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <map>

int main() {
    Graph graph; // Instância do grafo
    std::map<std::string, int> nameToIndex; // Mapeia nomes de centros urbanos para índices
    std::vector<std::string> indexToName;   // Mapeia índices para os nomes de centros urbanos

    int numCenters, numEdges; // Número de centros urbanos e estradas
    std::cin >> numCenters >> numEdges;

    // Lê os centros urbanos e as estradas do grafo
    for (int i = 0; i < numEdges; ++i) {
        std::string from, to;
        std::cin >> from >> to;

        // Adiciona novos centros urbanos ao grafo, se ainda não existem
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

        graph.addEdge(from, to); // Adiciona a estrada direcionada
    }

    // Encontra e imprime a capital do estado
    std::cout << graph.findCapital() << std::endl;

    // Encontra e imprime os batalhões secundários necessários
    auto secondaryBattalions = graph.findSecondaryBattalions();
    std::cout << secondaryBattalions.size() << std::endl;
    for (const auto& battalion : secondaryBattalions) {
        std::cout << battalion << std::endl;
    }

    // Encontra e imprime as rotas de patrulhamento
    auto patrolRoutes = graph.findPatrolRoutes();
    std::cout << patrolRoutes.size() << std::endl;
    for (const auto& route : patrolRoutes) {
        for (size_t i = 0; i < route.size(); ++i) {
            std::cout << route[i];
            if (i < route.size() - 1) std::cout << " "; // Evita espaço extra no final da linha
        }
        std::cout << std::endl;
    }

    return 0;
}
