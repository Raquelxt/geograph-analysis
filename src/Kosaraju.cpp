#include "Kosaraju.hpp"
#include <stack>
#include <unordered_map>

std::vector<std::vector<std::string>> Kosaraju::findConnectedComponents(
    const std::unordered_map<std::string, std::vector<std::string>>& adjacencyList) {

    std::unordered_map<std::string, bool> visited;
    std::stack<std::string> order;
    std::vector<std::vector<std::string>> components;

    // Função auxiliar para DFS de preenchimento da pilha
    auto dfsOrder = [&](const std::string& node, auto& dfsOrderRef) -> void {
        visited[node] = true;
        for (const auto& neighbor : adjacencyList.at(node)) {
            if (!visited[neighbor]) {
                dfsOrderRef(neighbor, dfsOrderRef);
            }
        }
        order.push(node);
    };

    for (const auto& pair : adjacencyList) {
        if (!visited[pair.first]) {
            dfsOrder(pair.first, dfsOrder);
        }
    }

    // Transposição do grafo
    auto transposedGraph = transposeGraph(adjacencyList);

    // Função auxiliar para DFS de componentes
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

    // Encontrar as componentes
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
