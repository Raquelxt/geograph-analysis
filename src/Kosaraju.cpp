#include "Kosaraju.hpp"
#include <stack>
#include <unordered_map>

// Encontra as componentes fortemente conectadas utilizando o algoritmo de Kosaraju
std::vector<std::vector<std::string>> Kosaraju::findConnectedComponents(
    const std::unordered_map<std::string, std::vector<std::string>>& adjacencyList) {

    std::unordered_map<std::string, bool> visited;
    std::stack<std::string> order;
    std::vector<std::vector<std::string>> components;

    // Realiza DFS para preencher a pilha com a ordem de finalização
    auto dfsOrder = [&](const std::string& node, auto& dfsOrderRef) -> void {
        visited[node] = true;
        for (const auto& neighbor : adjacencyList.at(node)) {
            if (!visited[neighbor]) {
                dfsOrderRef(neighbor, dfsOrderRef);
            }
        }
        order.push(node);
    };

    // Passo 1: Preenche a pilha com a ordem de finalização
    for (const auto& pair : adjacencyList) {
        if (!visited[pair.first]) {
            dfsOrder(pair.first, dfsOrder);
        }
    }

    // Passo 2: Obtém o grafo transposto
    auto transposedGraph = transposeGraph(adjacencyList);

    // Realiza DFS no grafo transposto para encontrar componentes
    visited.clear();
    auto dfsComponent = [&](const std::string& node, std::vector<std::string>& component, auto& dfsComponentRef) -> void {
        visited[node] = true;
        component.push_back(node);
        for (const auto& neighbor : transposedGraph[node]) {
            if (!visited[neighbor]) {
                dfsComponentRef(neighbor, component, dfsComponentRef);
            }
        }
    };

    // Passo 3: Identifica as componentes fortemente conectadas
    while (!order.empty()) {
        std::string node = order.top();
        order.pop();
        if (!visited[node]) {
            std::vector<std::string> component;
            dfsComponent(node, component, dfsComponent);
            components.push_back(component);
        }
    }

    return components;
}

// Transpõe o grafo (inverte as arestas)
std::unordered_map<std::string, std::vector<std::string>> Kosaraju::transposeGraph(
    const std::unordered_map<std::string, std::vector<std::string>>& adjacencyList) {

    std::unordered_map<std::string, std::vector<std::string>> transposed;
    for (const auto& pair : adjacencyList) {
        for (const auto& neighbor : pair.second) {
            transposed[neighbor].push_back(pair.first);
        }
    }
    return transposed;
}
