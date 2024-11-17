#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>

// Verifica se um vetor contém um elemento
bool contains(const std::vector<std::string>& vec, const std::string& value);

// Função para ordenar um vetor de strings
void sortStrings(std::vector<std::string>& vec);

// Função para inverter um vetor
void reverseVector(std::vector<std::string>& vec);

// Função para ordenar rotas lexicograficamente
void sortRoutes(std::vector<std::vector<std::string>>& routes);

#endif // UTILS_HPP
