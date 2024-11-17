#include "EulerianCycle.hpp"
#include "HungarianAlgorithm.hpp"
#include <queue>
#include <stack>
#include <unordered_map>
#include <limits>

// Encontra um ciclo Euleriano a partir de um nó inicial
std::vector<std::string> EulerianCycle::findCycleFromStart(
    const std::unordered_map<std::string, std::vector<std::string>>& subgraph,
    const std::string& startNode) {

    std::unordered_map<std::string, std::vector<std::string>> tempGraph = subgraph;
    std::stack<std::string> currentPath;
    std::vector<std::string> eulerianCycle;

    currentPath.push(startNode);

    // Algoritmo de Hierholzer para encontrar o ciclo
    while (!currentPath.empty()) {
        std::string current = currentPath.top();

        if (!tempGraph[current].empty()) {
            currentPath.push(tempGraph[current].back());
            tempGraph[current].pop_back();
        } else {
            eulerianCycle.push_back(current);
            currentPath.pop();
        }
    }

    // Garante que o ciclo feche no nó inicial
    if (eulerianCycle.front() != eulerianCycle.back()) {
        eulerianCycle.push_back(eulerianCycle.front());
    }
    return eulerianCycle;
}

// Transforma o grafo em Euleriano adicionando arestas necessárias
void EulerianCycle::transformToEulerian(
    std::unordered_map<std::string, std::vector<std::string>>& subgraph,
    const std::unordered_map<std::string, int>& balance) {

    std::vector<std::string> positiveBalance, negativeBalance;

    // Separa nós com balanço positivo e negativo
    for (const auto& pair : balance) {
        if (pair.second > 0) {
            positiveBalance.insert(positiveBalance.end(), pair.second, pair.first);
        } else if (pair.second < 0) {
            negativeBalance.insert(negativeBalance.end(), -pair.second, pair.first);
        }
    }

    // Cria a matriz de custo baseada em distâncias
    std::vector<std::vector<int>> costMatrix(negativeBalance.size(),
                                             std::vector<int>(positiveBalance.size(),
                                                              std::numeric_limits<int>::max()));

    for (size_t i = 0; i < negativeBalance.size(); ++i) {
        for (size_t j = 0; j < positiveBalance.size(); ++j) {
            costMatrix[i][j] = subgraph.at(negativeBalance[i]).size();
        }
    }

    // Resolve o emparelhamento mínimo usando o Algoritmo Húngaro
    HungarianAlgorithm hungarian;
    auto matching = hungarian.solve(costMatrix);

    // Adiciona as arestas necessárias para balancear
    for (size_t i = 0; i < matching.size(); ++i) {
        if (matching[i] != -1) {
            subgraph[negativeBalance[i]].push_back(positiveBalance[matching[i]]);
        }
    }
}

// Calcula o balanço dos graus de entrada e saída de cada nó
std::unordered_map<std::string, int> EulerianCycle::calculateBalance(
    const std::unordered_map<std::string, std::vector<std::string>>& subgraph) {

    std::unordered_map<std::string, int> balance;

    // Inicializa o balanço para cada nó
    for (const auto& pair : subgraph) {
        balance[pair.first] = 0;
    }

    // Atualiza o balanço de acordo com as arestas
    for (const auto& pair : subgraph) {
        for (const auto& neighbor : pair.second) {
            balance[pair.first]++;
            balance[neighbor]--;
        }
    }
    return balance;
}
