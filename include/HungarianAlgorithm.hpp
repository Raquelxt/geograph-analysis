#ifndef HUNGARIAN_ALGORITHM_HPP
#define HUNGARIAN_ALGORITHM_HPP

#include <vector>
#include <limits>

class HungarianAlgorithm {
public:
    // Resolve o problema de emparelhamento perfeito com base em uma matriz de custos
    std::vector<int> solve(const std::vector<std::vector<int>>& costMatrix);
};

#endif // HUNGARIAN_ALGORITHM_HPP
