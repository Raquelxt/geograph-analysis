#include "Graph.hpp"
#include "Utils.hpp"
#include <queue>
#include <stack>
#include <limits>
#include <iostream>

// Algoritmo de Kosaraju para encontrar componentes fortemente conectadas
std::vector<std::vector<std::string>> Graph::findConnectedComponents() {
    std::unordered_map<std::string, bool> visited;
    std::stack<std::string> order;
    std::vector<std::vector<std::string>> components;

    // Passo 1: Preencher a pilha com a ordem de finalização dos nós
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

    // Passo 2: Transpor o grafo
    auto transposedGraph = transposeGraph();

    // Passo 3: Encontrar componentes fortemente conectadas
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

// Implementação do Algoritmo Húngaro para emparelhamento perfeito
std::vector<int> HungarianAlgorithm::solve(const std::vector<std::vector<int>>& costMatrix) {
    int n = costMatrix.size();
    int m = costMatrix[0].size();
    std::vector<int> u(n + 1, 0), v(m + 1, 0), p(m + 1, 0), way(m + 1, 0);

    for (int i = 1; i <= n; ++i) {
        std::vector<int> minv(m + 1, std::numeric_limits<int>::max());
        std::vector<bool> used(m + 1, false);
        int j0 = 0;
        p[0] = i;

        do {
            used[j0] = true;
            int i0 = p[j0], delta = std::numeric_limits<int>::max(), j1 = 0;
            for (int j = 1; j <= m; ++j) {
                if (!used[j]) {
                    int cur = costMatrix[i0 - 1][j - 1] - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }
            for (int j = 0; j <= m; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0] != 0);

        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0 != 0);
    }

    std::vector<int> result(n, -1);
    for (int j = 1; j <= m; ++j) {
        if (p[j] != 0) {
            result[p[j] - 1] = j - 1;
        }
    }
    return result;
}

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

// Função para encontrar os batalhões secundários
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
            continue;
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

// Função para encontrar rotas de patrulhamento
// Função para encontrar rotas de patrulhamento
std::vector<std::vector<std::string>> Graph::findPatrolRoutes() {
    std::vector<std::vector<std::string>> routes;
    auto components = findConnectedComponents();

    for (const auto& component : components) {
        if (component.size() <= 1) continue;

        auto subgraph = generateSubgraph(component);
        auto balance = calculateBalance(subgraph);

        bool isEulerian = true;
        for (const auto& pair : balance) {
            if (pair.second != 0) {
                isEulerian = false;
                break;
            }
        }

        if (!isEulerian) {
            transformToEulerian(subgraph, balance);
        }

        auto cycle = findEulerianCycle(subgraph);
        if (!cycle.empty()) {
            // Ordenar o ciclo para atender ao formato esperado
            if (cycle.front() != cycle.back()) {
                cycle.push_back(cycle.front());
            }
            routes.push_back(cycle);
        }
    }

    // Ordenar as rotas para saída correta
    sortRoutes(routes);
    return routes;
}

// Gera o subgrafo
std::unordered_map<std::string, std::vector<std::string>> Graph::generateSubgraph(const std::vector<std::string>& vertices) {
    std::unordered_map<std::string, std::vector<std::string>> subgraph;
    for (const auto& vertex : vertices) {
        if (adjacencyList.find(vertex) != adjacencyList.end()) {
            subgraph[vertex] = {};
            for (const auto& neighbor : adjacencyList[vertex]) {
                if (contains(vertices, neighbor)) {
                    subgraph[vertex].push_back(neighbor);
                }
            }
        }
    }
    return subgraph;
}

// Calcula balanceamento de vértices
std::unordered_map<std::string, int> Graph::calculateBalance(const std::unordered_map<std::string, std::vector<std::string>>& subgraph) {
    std::unordered_map<std::string, int> balance;
    for (const auto& pair : subgraph) {
        balance[pair.first] = 0;
    }

    for (const auto& pair : subgraph) {
        for (const auto& neighbor : pair.second) {
            balance[pair.first]++;
            balance[neighbor]--;
        }
    }
    return balance;
}

// Transforma subgrafo em Euleriano
void Graph::transformToEulerian(std::unordered_map<std::string, std::vector<std::string>>& subgraph, std::unordered_map<std::string, int>& balance) {
    std::vector<std::string> positiveBalance, negativeBalance;

    for (const auto& pair : balance) {
        if (pair.second > 0) {
            for (int i = 0; i < pair.second; ++i) {
                positiveBalance.push_back(pair.first);
            }
        } else if (pair.second < 0) {
            for (int i = 0; i < -pair.second; ++i) {
                negativeBalance.push_back(pair.first);
            }
        }
    }

    std::vector<std::vector<int>> costMatrix(negativeBalance.size(), std::vector<int>(positiveBalance.size(), std::numeric_limits<int>::max()));
    for (size_t i = 0; i < negativeBalance.size(); ++i) {
        for (size_t j = 0; j < positiveBalance.size(); ++j) {
            costMatrix[i][j] = bfsDistance(subgraph, negativeBalance[i], positiveBalance[j]);
        }
    }

    HungarianAlgorithm hungarian;
    auto matching = hungarian.solve(costMatrix);

    for (size_t i = 0; i < matching.size(); ++i) {
        if (matching[i] != -1) {
            subgraph[negativeBalance[i]].push_back(positiveBalance[matching[i]]);
        }
    }
}

// BFS para calcular distâncias
int Graph::bfsDistance(const std::unordered_map<std::string, std::vector<std::string>>& graph, const std::string& source, const std::string& target) {
    std::queue<std::pair<std::string, int>> q;
    std::unordered_map<std::string, bool> visited;

    q.push({source, 0});
    visited[source] = true;

    while (!q.empty()) {
        auto [current, dist] = q.front();
        q.pop();

        if (current == target) return dist;

        for (const auto& neighbor : graph.at(current)) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push({neighbor, dist + 1});
            }
        }
    }
    return std::numeric_limits<int>::max();
}

// Encontra ciclo Euleriano
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