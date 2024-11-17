# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -Iinclude

# Pastas
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

# Arquivos fontes e objetos
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

# Nome do executável
TARGET = tp1

# Regras
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
