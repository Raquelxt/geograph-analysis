#include "Graph.hpp"
#include "Utils.hpp"
#include <queue>
#include <stack>
#include <limits>
#include <iostream>
#include <unordered_set>

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

// Gera a transposição do grafo
std::unordered_map<std::string, std::vector<std::string>> Graph::transposeGraph() {
    // Inverte as arestas de direção no grafo
    std::unordered_map<std::string, std::vector<std::string>> transposed;
    for (const auto& pair : adjacencyList) {
        for (const auto& neighbor : pair.second) {
            transposed[neighbor].push_back(pair.first);
        }
    }
    return transposed;
}

// Algoritmo Húngaro para resolver emparelhamento perfeito mínimo
std::vector<int> HungarianAlgorithm::solve(const std::vector<std::vector<int>>& costMatrix) {
    // Resolve o problema de emparelhamento com base em uma matriz de custos
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
    // Calcula as distâncias de cada nó para todos os outros usando BFS
    // Retorna o nó com menor soma de distâncias
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
    // Usa componentes fortemente conectadas para determinar batalhões necessários
    // Verifica a conectividade de cada componente a partir da capital
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

// Encontra rotas de patrulhamento que percorrem todas as arestas a partir de batalhões
std::vector<std::vector<std::string>> Graph::findPatrolRoutes() {
    std::vector<std::vector<std::string>> routes; // Armazena todas as rotas
    auto components = findConnectedComponents(); // Obter as CFCs

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
        auto balance = calculateBalance(subgraph);

        // Transformar em Euleriano, se necessário
        bool isEulerian = true;
        for (const auto& pair : balance) {
            if (pair.second != 0) { // Balanceamento diferente de 0
                isEulerian = false;
                break;
            }
        }

        if (!isEulerian) {
            transformToEulerian(subgraph, balance); // Tornar Euleriano
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
        auto cycle = findEulerianCycleFromStart(subgraph, startNode);
        if (!cycle.empty()) {
            routes.push_back(cycle);
        }
    }

    // Ordenar rotas lexicograficamente para saída correta
    sortRoutes(routes);
    return routes;
}

// Encontra ciclo Euleriano a partir de um nó específico
std::vector<std::string> Graph::findEulerianCycleFromStart(
    // Gera ciclo Euleriano começando pelo nó startNode
    const std::unordered_map<std::string, std::vector<std::string>>& subgraph,
    const std::string& startNode) {
    std::unordered_map<std::string, std::vector<std::string>> tempGraph = subgraph;
    std::stack<std::string> currentPath;
    std::vector<std::string> eulerianCycle;

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

    if (eulerianCycle.front() != eulerianCycle.back()) {
        eulerianCycle.push_back(eulerianCycle.front()); // Fechar ciclo
    }
    return eulerianCycle;
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

// Calcula o balanceamento de graus dos vértices no subgrafo
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

// Ajusta o grafo para torná-lo Euleriano
void Graph::transformToEulerian(std::unordered_map<std::string, std::vector<std::string>>& subgraph, 
                                std::unordered_map<std::string, int>& balance) {
    std::vector<std::string> positiveBalance, negativeBalance;

    // Separar os vértices com balanço positivo e negativo
    for (const auto& pair : balance) {
        if (pair.second > 0) {
            positiveBalance.insert(positiveBalance.end(), pair.second, pair.first);
        } else if (pair.second < 0) {
            negativeBalance.insert(negativeBalance.end(), -pair.second, pair.first);
        }
    }

    // Criar a matriz de custo baseada nas distâncias BFS
    std::vector<std::vector<int>> costMatrix(negativeBalance.size(), 
                                             std::vector<int>(positiveBalance.size(), 
                                             std::numeric_limits<int>::max()));
    for (size_t i = 0; i < negativeBalance.size(); ++i) {
        for (size_t j = 0; j < positiveBalance.size(); ++j) {
            costMatrix[i][j] = bfsDistance(subgraph, negativeBalance[i], positiveBalance[j]);
        }
    }

    // Resolver emparelhamento perfeito usando o Algoritmo Húngaro
    HungarianAlgorithm hungarian;
    auto matching = hungarian.solve(costMatrix);

    // Adicionar arestas extras ao subgrafo
    for (size_t i = 0; i < matching.size(); ++i) {
        if (matching[i] != -1) {
            subgraph[negativeBalance[i]].push_back(positiveBalance[matching[i]]);
        }
    }
}

// Executa BFS para calcular a distância entre dois nós
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

// Encontra ciclo Euleriano em um subgrafo usando Hierholzer
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

    if (eulerianCycle.front() != eulerianCycle.back()) {
        eulerianCycle.push_back(eulerianCycle.front()); // Fechar ciclo
    }
    return eulerianCycle;
}
