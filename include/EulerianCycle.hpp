#ifndef EULERIAN_CYCLE_HPP
#define EULERIAN_CYCLE_HPP

#include <string>
#include <vector>
#include <unordered_map>

class EulerianCycle {
public:
    // Encontra um ciclo Euleriano a partir de um nó inicial
    static std::vector<std::string> findCycleFromStart(
        const std::unordered_map<std::string, std::vector<std::string>>& subgraph,
        const std::string& startNode);

    // Verifica e ajusta um subgrafo para torná-lo Euleriano
    static void transformToEulerian(
        std::unordered_map<std::string, std::vector<std::string>>& subgraph,
        const std::unordered_map<std::string, int>& balance);

    // Calcula o balanceamento de graus dos vértices no subgrafo
    static std::unordered_map<std::string, int> calculateBalance(
        const std::unordered_map<std::string, std::vector<std::string>>& subgraph);
};

#endif // EULERIAN_CYCLE_HPP
