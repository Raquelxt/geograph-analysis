#ifndef KOSARAJU_HPP
#define KOSARAJU_HPP

#include <string>
#include <vector>
#include <unordered_map>

class Kosaraju {
public:
    // Encontra componentes fortemente conectadas em um grafo
    static std::vector<std::vector<std::string>> findConnectedComponents(
        const std::unordered_map<std::string, std::vector<std::string>>& adjacencyList);

    // Gera a transposição do grafo
    static std::unordered_map<std::string, std::vector<std::string>> transposeGraph(
        const std::unordered_map<std::string, std::vector<std::string>>& adjacencyList);
};

#endif // KOSARAJU_HPP
