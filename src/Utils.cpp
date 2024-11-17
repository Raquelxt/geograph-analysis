#include "Utils.hpp"

// Verifica se um vetor contém um elemento
bool contains(const std::vector<std::string>& vec, const std::string& value) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == value) {
            return true;
        }
    }
    return false;
}

// Função para ordenar um vetor de strings
void sortStrings(std::vector<std::string>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        for (size_t j = i + 1; j < vec.size(); ++j) {
            if (vec[i] > vec[j]) {
                // Troca manual
                std::string temp = vec[i];
                vec[i] = vec[j];
                vec[j] = temp;
            }
        }
    }
}

// Função para inverter um vetor
void reverseVector(std::vector<std::string>& vec) {
    size_t start = 0;
    size_t end = vec.size() - 1;
    while (start < end) {
        // Troca manual
        std::string temp = vec[start];
        vec[start] = vec[end];
        vec[end] = temp;
        ++start;
        --end;
    }
}

// Ordena as rotas lexicograficamente
void sortRoutes(std::vector<std::vector<std::string>>& routes) {
    for (size_t i = 0; i < routes.size(); ++i) {
        for (size_t j = i + 1; j < routes.size(); ++j) {
            if (routes[i] > routes[j]) {
                std::swap(routes[i], routes[j]);
            }
        }
    }
}
