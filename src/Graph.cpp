#include "Graph.hpp"
#include "Kosaraju.hpp"
#include "EulerianCycle.hpp"
#include <queue>
#include <unordered_set>
#include <limits>

// Função Auxiliar: Verifica se um vetor contém um elemento específico
bool contains(const std::vector<std::string>& vec, const std::string& value) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == value) {
            return true;
        }
    }
    return false;
}

// Função Auxiliar: Ordena um vetor de strings em ordem lexicográfica
void sortStrings(std::vector<std::string>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        for (size_t j = i + 1; j < vec.size(); ++j) {
            if (vec[i] > vec[j]) {
                // Troca manual dos elementos
                std::string temp = vec[i];
                vec[i] = vec[j];
                vec[j] = temp;
            }
        }
    }
}

// Função Auxiliar: Inverte a ordem dos elementos de um vetor
void reverseVector(std::vector<std::string>& vec) {
    size_t start = 0;
    size_t end = vec.size() - 1;
    while (start < end) {
        // Troca manual dos elementos
        std::string temp = vec[start];
        vec[start] = vec[end];
        vec[end] = temp;
        ++start;
        --end;
    }
}

// Função Auxiliar: Ordena um vetor de rotas (vetores de strings) em ordem lexicográfica
void sortRoutes(std::vector<std::vector<std::string>>& routes) {
    for (size_t i = 0; i < routes.size(); ++i) {
        for (size_t j = i + 1; j < routes.size(); ++j) {
            if (routes[i] > routes[j]) {
                std::swap(routes[i], routes[j]);
            }
        }
    }
}


// Adiciona um nó ao grafo, caso ele não exista
void Graph::addNode(const std::string& location) {
    if (adjacencyList.find(location) == adjacencyList.end()) {
        adjacencyList[location] = {};
    }
}

// Adiciona uma aresta direcionada ao grafo
void Graph::addEdge(const std::string& from, const std::string& to) {
    adjacencyList[from].push_back(to);
}

// Encontra a capital como o nó que minimiza a soma das distâncias para todos os outros
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

// Determina os batalhões secundários necessários para garantir conectividade de retorno
std::vector<std::string> Graph::findSecondaryBattalions() {
    std::vector<std::string> secondaryBattalions;
    std::string capital = findCapital();

    auto components = Kosaraju::findConnectedComponents(adjacencyList);
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

// Encontra rotas de patrulhamento Eulerianas que cobrem todas as arestas
std::vector<std::vector<std::string>> Graph::findPatrolRoutes() {
    std::vector<std::vector<std::string>> routes;
    auto components = Kosaraju::findConnectedComponents(adjacencyList);

    std::string capital = findCapital();
    std::vector<std::string> secondaryBattalions = findSecondaryBattalions();
    std::unordered_set<std::string> allBattalions(secondaryBattalions.begin(), secondaryBattalions.end());
    allBattalions.insert(capital);

    for (const auto& component : components) {
        if (component.size() <= 1) continue;

        auto subgraph = generateSubgraph(component);
        auto balance = EulerianCycle::calculateBalance(subgraph);

        bool isEulerian = true;
        for (const auto& pair : balance) {
            if (pair.second != 0) {
                isEulerian = false;
                break;
            }
        }

        if (!isEulerian) {
            EulerianCycle::transformToEulerian(subgraph, balance);
        }

        std::string startNode;
        for (const auto& node : component) {
            if (allBattalions.count(node)) {
                startNode = node;
                break;
            }
        }

        if (startNode.empty()) {
            startNode = component.front();
        }

        auto cycle = EulerianCycle::findCycleFromStart(subgraph, startNode);
        if (!cycle.empty()) {
            routes.push_back(cycle);
        }
    }

    sortRoutes(routes);
    return routes;
}

// Gera um subgrafo contendo apenas os nós e arestas de um conjunto de vértices
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
