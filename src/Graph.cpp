#include "Graph.hpp"
#include "Kosaraju.hpp"
#include "EulerianCycle.hpp"
#include "Utils.hpp"
#include <queue>
#include <unordered_set>
#include <limits>

// Adiciona um nó ao grafo, caso não exista
void Graph::addNode(const std::string& location) {
    if (adjacencyList.find(location) == adjacencyList.end()) {
        adjacencyList[location] = {};
    }
}

// Adiciona uma aresta unidirecional ao grafo
void Graph::addEdge(const std::string& from, const std::string& to) {
    adjacencyList[from].push_back(to);
}

// Identifica a capital como o nó que minimiza a distância total para todos os outros
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

// Encontra os batalhões secundários para garantir conectividade de retorno
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

// Encontra rotas de patrulhamento que percorrem todas as arestas a partir de batalhões
std::vector<std::vector<std::string>> Graph::findPatrolRoutes() {
    std::vector<std::vector<std::string>> routes; // Armazena todas as rotas
    auto components = Kosaraju::findConnectedComponents(adjacencyList); // Obter as CFCs

    // Identificar os batalhões (capital e secundários)
    std::string capital = findCapital();
    std::vector<std::string> secondaryBattalions = findSecondaryBattalions();
    std::unordered_set<std::string> allBattalions(secondaryBattalions.begin(), secondaryBattalions.end());
    allBattalions.insert(capital);

    for (const auto& component : components) {
        if (component.size() <= 1) continue; // Ignorar CFCs com apenas 1 nó

        // Gerar subgrafo para a CFC atual
        auto subgraph = generateSubgraph(component);

        // Verificar balanceamento dos vértices
        auto balance = EulerianCycle::calculateBalance(subgraph);

        // Transformar em Euleriano, se necessário
        bool isEulerian = true;
        for (const auto& pair : balance) {
            if (pair.second != 0) { // Balanceamento diferente de 0
                isEulerian = false;
                break;
            }
        }

        if (!isEulerian) {
            EulerianCycle::transformToEulerian(subgraph, balance); // Tornar Euleriano
        }

        // Escolher o nó inicial para o ciclo Euleriano
        std::string startNode;
        for (const auto& node : component) {
            if (allBattalions.count(node)) {
                startNode = node; // Priorizar um batalhão
                break;
            }
        }

        // Se nenhum nó do batalhão estiver presente, use o primeiro nó da componente
        if (startNode.empty()) {
            startNode = component.front();
        }

        // Criar rota Euleriana usando Hierholzer a partir de startNode
        auto cycle = EulerianCycle::findCycleFromStart(subgraph, startNode);
        if (!cycle.empty()) {
            routes.push_back(cycle);
        }
    }

    // Ordenar rotas lexicograficamente para saída correta
    sortRoutes(routes);
    return routes;
}

// Gera um subgrafo baseado em um conjunto de vértices
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
