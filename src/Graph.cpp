#include "Graph.hpp"
#include <queue>
#include <limits>
#include <unordered_map>
#include <stack>
#include <iostream>

// Adiciona um nó ao grafo
void Graph::addNode(const std::string& location) {
    if (adjacencyList.find(location) == adjacencyList.end()) {
        adjacencyList[location] = {};
    }
}

// Adiciona uma aresta entre dois nós (direção única)
void Graph::addEdge(const std::string& from, const std::string& to) {
    adjacencyList[from].push_back(to);
}

// Encontra a capital do grafo
std::string Graph::findCapital() {
    std::string bestNode;
    int minTotalDistance = std::numeric_limits<int>::max();

    for (const auto& pair : adjacencyList) {
        const std::string& start = pair.first;
        std::unordered_map<std::string, int> distances;

        for (const auto& node : adjacencyList) {
            distances[node.first] = std::numeric_limits<int>::max();
        }
        distances[start] = 0;

        std::queue<std::string> q;
        q.push(start);

        while (!q.empty()) {
            std::string current = q.front();
            q.pop();

            for (const auto& neighbor : adjacencyList[current]) {
                if (distances[neighbor] == std::numeric_limits<int>::max()) {
                    distances[neighbor] = distances[current] + 1;
                    q.push(neighbor);
                }
            }
        }

        int totalDistance = 0;
        for (const auto& dist : distances) {
            if (dist.second == std::numeric_limits<int>::max()) {
                totalDistance = std::numeric_limits<int>::max();
                break;
            }
            totalDistance += dist.second;
        }

        if (totalDistance < minTotalDistance || 
           (totalDistance == minTotalDistance && start < bestNode)) {
            minTotalDistance = totalDistance;
            bestNode = start;
        }
    }
    return bestNode;
}

// Função para encontrar elemento em vetor
bool contains(const std::vector<std::string>& vec, const std::string& value) {
    for (const auto& item : vec) {
        if (item == value) return true;
    }
    return false;
}

// Função para ordenar vetor de strings
void sortStrings(std::vector<std::string>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        for (size_t j = i + 1; j < vec.size(); ++j) {
            if (vec[i] > vec[j]) {
                std::swap(vec[i], vec[j]);
            }
        }
    }
}

// Função para ordenar vetor de vetores de strings
void sortRoutes(std::vector<std::vector<std::string>>& routes) {
    for (size_t i = 0; i < routes.size(); ++i) {
        for (size_t j = i + 1; j < routes.size(); ++j) {
            if (routes[i] > routes[j]) {
                std::swap(routes[i], routes[j]);
            }
        }
    }
}

// Função para reverter vetor
void reverseVector(std::vector<std::string>& vec) {
    size_t start = 0, end = vec.size() - 1;
    while (start < end) {
        std::swap(vec[start], vec[end]);
        ++start;
        --end;
    }
}

// Algoritmo de Kosaraju para encontrar componentes fortemente conectadas
std::vector<std::vector<std::string>> Graph::findConnectedComponents() {
    std::unordered_map<std::string, bool> visited;
    std::stack<std::string> order;
    std::vector<std::vector<std::string>> components;

    auto dfsOrder = [&](const std::string& node, auto& dfsOrderRef) -> void {
        visited[node] = true;
        for (const auto& neighbor : adjacencyList[node]) {
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

    auto transposedGraph = transposeGraph();
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

// Gera o grafo transposto
std::unordered_map<std::string, std::vector<std::string>> Graph::transposeGraph() {
    std::unordered_map<std::string, std::vector<std::string>> transposed;
    for (const auto& pair : adjacencyList) {
        for (const auto& neighbor : pair.second) {
            transposed[neighbor].push_back(pair.first);
        }
    }
    return transposed;
}

// Verifica e transforma o componente para Euleriano
void Graph::ensureEulerian(std::unordered_map<std::string, std::vector<std::string>>& subgraph) {
    std::unordered_map<std::string, int> inDegree, outDegree;

    for (const auto& pair : subgraph) {
        outDegree[pair.first] = pair.second.size();
        for (const auto& neighbor : pair.second) {
            inDegree[neighbor]++;
        }
    }

    std::vector<std::pair<std::string, std::string>> additions;
    for (const auto& pair : outDegree) {
        if (inDegree[pair.first] < pair.second) {
            for (const auto& neighbor : subgraph) {
                if (outDegree[neighbor.first] < inDegree[neighbor.first]) {
                    additions.emplace_back(pair.first, neighbor.first);
                    outDegree[pair.first]--;
                    inDegree[neighbor.first]++;
                    break;
                }
            }
        }
    }

    for (const auto& edge : additions) {
        subgraph[edge.first].push_back(edge.second);
    }
}

// Encontra os batalhões secundários
std::vector<std::string> Graph::findSecondaryBattalions() {
    std::vector<std::string> secondaryBattalions;
    std::string capital = findCapital();

    auto components = findConnectedComponents();
    std::unordered_map<std::string, int> distances;

    for (const auto& pair : adjacencyList) {
        distances[pair.first] = std::numeric_limits<int>::max();
    }
    distances[capital] = 0;

    std::queue<std::string> q;
    q.push(capital);

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        for (const auto& neighbor : adjacencyList[current]) {
            if (distances[neighbor] == std::numeric_limits<int>::max()) {
                distances[neighbor] = distances[current] + 1;
                q.push(neighbor);
            }
        }
    }

    for (const auto& component : components) {
        if (contains(component, capital)) {
            continue; // Ignorar componente que contém a capital.
        }

        if (!component.empty()) {
            std::string closestNode;
            int minDistance = std::numeric_limits<int>::max();
            for (const auto& node : component) {
                if (distances[node] < minDistance) {
                    minDistance = distances[node];
                    closestNode = node;
                }
            }
            secondaryBattalions.push_back(closestNode);
        }
    }

    sortStrings(secondaryBattalions);
    return secondaryBattalions;
}

// Encontra o ciclo Euleriano
std::vector<std::string> Graph::findEulerianCycle(const std::unordered_map<std::string, std::vector<std::string>>& subgraph) {
    std::unordered_map<std::string, std::vector<std::string>> tempGraph = subgraph;
    std::stack<std::string> currentPath;
    std::vector<std::string> eulerianCycle;

    std::string startNode = tempGraph.begin()->first;
    currentPath.push(startNode);

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

    reverseVector(eulerianCycle);
    return eulerianCycle;
}

// Encontra as rotas de patrulha
std::vector<std::vector<std::string>> Graph::findPatrolRoutes() {
    std::vector<std::vector<std::string>> routes;
    auto components = findConnectedComponents();

    for (const auto& component : components) {
        if (component.size() <= 1) continue;

        std::unordered_map<std::string, std::vector<std::string>> subgraph;
        for (const auto& node : component) {
            subgraph[node] = adjacencyList[node];
        }

        // Verificar se o grafo é Euleriano
        std::unordered_map<std::string, int> inDegree, outDegree;
        for (const auto& pair : subgraph) {
            outDegree[pair.first] = pair.second.size();
            for (const auto& neighbor : pair.second) {
                inDegree[neighbor]++;
            }
        }

        bool isEulerian = true;
        for (const auto& pair : subgraph) {
            if (inDegree[pair.first] != outDegree[pair.first]) {
                isEulerian = false;
                break;
            }
        }

        // Transformar em Euleriano, se necessário
        if (!isEulerian) {
            ensureEulerian(subgraph);
        }

        // Gerar o ciclo Euleriano
        auto cycle = findEulerianCycle(subgraph);
        if (!cycle.empty()) {
            routes.push_back(cycle);
        }
    }

    sortRoutes(routes);
    return routes;
}
