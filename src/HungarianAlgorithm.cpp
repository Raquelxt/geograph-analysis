#include "../include/HungarianAlgorithm.hpp"

// Resolve o problema de emparelhamento perfeito de custo mínimo utilizando o Algoritmo Húngaro
std::vector<int> HungarianAlgorithm::solve(const std::vector<std::vector<int>>& costMatrix) {
    int numRows = costMatrix.size(); // Número de linhas na matriz de custo
    int numCols = costMatrix[0].size(); // Número de colunas na matriz de custo

    // Variáveis de dualidade para linhas e colunas
    std::vector<int> u(numRows + 1, 0), v(numCols + 1, 0);

    // Vetores para rastrear o emparelhamento e o caminho atual
    std::vector<int> match(numCols + 1, 0), path(numCols + 1, 0);

    // Itera sobre cada linha para calcular o emparelhamento de custo mínimo
    for (int row = 1; row <= numRows; ++row) {
        std::vector<int> minValues(numCols + 1, std::numeric_limits<int>::max());
        std::vector<bool> visited(numCols + 1, false);
        int col0 = 0; // Começa pela coluna fictícia
        match[0] = row; // Emparelha a coluna fictícia com a linha atual

        // Processo iterativo para melhorar o emparelhamento
        do {
            visited[col0] = true;
            int currentRow = match[col0];
            int minDelta = std::numeric_limits<int>::max();
            int nextCol = 0;

            // Atualiza os valores mínimos para colunas ainda não visitadas
            for (int col = 1; col <= numCols; ++col) {
                if (!visited[col]) {
                    int currentCost = costMatrix[currentRow - 1][col - 1] - u[currentRow] - v[col];

                    // Atualiza o menor custo e rastreia o caminho
                    if (currentCost < minValues[col]) {
                        minValues[col] = currentCost;
                        path[col] = col0;
                    }

                    // Determina a próxima coluna com o menor delta
                    if (minValues[col] < minDelta) {
                        minDelta = minValues[col];
                        nextCol = col;
                    }
                }
            }

            // Atualiza as variáveis de dualidade e reduz os valores mínimos restantes
            for (int col = 0; col <= numCols; ++col) {
                if (visited[col]) {
                    u[match[col]] += minDelta;
                    v[col] -= minDelta;
                } else {
                    minValues[col] -= minDelta;
                }
            }

            col0 = nextCol;
        } while (match[col0] != 0); // Continua até encontrar uma coluna sem emparelhamento

        // Realiza a atualização do emparelhamento
        do {
            int nextCol = path[col0];
            match[col0] = match[nextCol];
            col0 = nextCol;
        } while (col0 != 0);
    }

    // Constrói o resultado com o emparelhamento final
    std::vector<int> result(numRows, -1);
    for (int col = 1; col <= numCols; ++col) {
        if (match[col] != 0) {
            result[match[col] - 1] = col - 1;
        }
    }

    return result;
}
