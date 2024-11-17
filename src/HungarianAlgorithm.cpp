#include "HungarianAlgorithm.hpp"

std::vector<int> HungarianAlgorithm::solve(const std::vector<std::vector<int>>& costMatrix) {
    int n = costMatrix.size();
    int m = costMatrix[0].size();

    // Vetores de dualidade
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
