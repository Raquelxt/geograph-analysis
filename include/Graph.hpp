#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class Graph {
public:
    // Adiciona um nó ao grafo
    void addNode(const std::string& location);

    // Adiciona uma aresta entre dois nós
    void addEdge(const std::string& from, const std::string& to);

    // Determina a capital do grafo
    std::string findCapital();

    // Encontra os batalhões secundários necessários
    std::vector<std::string> findSecondaryBattalions();

    // Encontra as rotas de patrulha
    std::vector<std::vector<std::string>> findPatrolRoutes();

    // Exibe a lista de adjacência para depuração
    void display();

private:
    // Lista de adjacência
    std::unordered_map<std::string, std::vector<std::string>> adjacencyList;

    // Encontra componentes fortemente conectados (algoritmo de Kosaraju)
    std::vector<std::vector<std::string>> findConnectedComponents();

    // Gera o grafo transposto
    std::unordered_map<std::string, std::vector<std::string>> transposeGraph();

    // Verifica e transforma um componente em Euleriano
    void ensureEulerian(std::unordered_map<std::string, std::vector<std::string>>& subgraph);

    // Encontra o ciclo Euleriano em um subgrafo
    std::vector<std::string> findEulerianCycle(const std::unordered_map<std::string, std::vector<std::string>>& subgraph);
};

#endif // GRAPH_HPP
