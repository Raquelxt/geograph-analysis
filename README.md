# GeoGraph Analysis

**GeoGraph Analysis** is a project for Practical Assignment I in the Algorithms I course. The objective is to solve graph-based modeling and analysis problems applied to a fictional scenario involving state security and invasion prevention. The project involves determining the best capital, identifying secondary battalion locations, and planning patrol routes.

---

## 🌟 Features

- **Capital Selection**: Identify the best urban center to serve as the state capital.
- **Battalion Allocation**: Determine the minimum number of secondary battalions required and their optimal locations.
- **Patrol Route Planning**: Evaluate and plan patrol routes that cover all roads in the state.

---

## 📂 Project Structure

The project is modularized for clarity and efficiency. Below is an overview of the key components:

```
GeoGraph-Analysis/
│
├── include/                # Header files
│   ├── Graph.hpp           # Graph structure and algorithms
│   ├── Kosaraju.hpp        # Strongly connected components (Kosaraju's algorithm)
│   ├── EulerianCycle.hpp   # Eulerian cycle detection and generation
│   ├── HungarianAlgorithm.hpp # Minimum perfect matching (Hungarian Algorithm)
│   ├── Utils.hpp           # Utility functions for sorting and validation
│
├── src/                    # Implementation files
│   ├── Graph.cpp
│   ├── Kosaraju.cpp
│   ├── EulerianCycle.cpp
│   ├── HungarianAlgorithm.cpp
│   ├── Utils.cpp
│
├── tests/
│   ├── inputs              # Test cases
│   │   ├── testCase01.txt
│   │   ├── testCase02.txt
│   │   ├── ...
│
├── Makefile                # Compilation script
├── README.md               # Project documentation
└── vpl.py                  # Script for automated testing
```

---

## 🚀 Compilation and Execution

### Prerequisites

- **C++ Compiler**: Ensure you have a C++ compiler supporting C++11 or later.
- **Linux Environment**: This project is tested in a Linux-based environment.

### Compile the program

Run the following command to compile:

```bash
make all
```

### Execute the program

Provide input through a file using standard input redirection:

```bash
./tp1 < tests/inputs/testCase01.txt
```

---

## 📋 Input and Output Format

### Input
The program reads a single state sketch. The format is as follows:

1. The first line contains two integers:
   - `NC`: Number of urban centers.
   - `NE`: Number of roads.
2. The next `NE` lines describe the roads:
   - Each line is in the format `C1 C2`, indicating a one-way road from `C1` to `C2`.

### Output
The output consists of:

1. The name of the chosen capital.
2. The number of secondary battalions (`NB`) and their respective urban centers (one per line).
3. The number of valid patrol routes (`P`) and a valid route for each.

---

## 🛠️ Implementation Details

### Programming Language

The project is implemented in **C++** and follows modular programming practices. It uses the ISO/IEC C++11 or later standard libraries.

### Algorithms Used

- **Graph Representation**: Adjacency list.
- **Kosaraju's Algorithm**: To identify strongly connected components.
- **Hungarian Algorithm**: To solve the minimum perfect matching problem.
- **Breadth-First Search (BFS)**: To calculate distances between nodes.
- **Hierholzer’s Algorithm**: To find Eulerian cycles.

---

## 🔍 Example

### Input
```plaintext
6 8
Rhedrise Vandrad
Vandrad Benith
Khudealine Thonet
Thonet Khudealine
Rhedrise Khudealine
Benith Vandrad
Vandrad Muafland
Muafland Vandrad
```

### Output
```plaintext
Rhedrise
2
Khudealine
Vandrad
2
Khudealine Thonet
Vandrad Muafland Vandrad Benith
```

---

## 📚 Additional Resources

### References
- [Kosaraju's Algorithm](https://en.wikipedia.org/wiki/Kosaraju%27s_algorithm)
- [Hierholzer’s Algorithm](https://en.wikipedia.org/wiki/Eulerian_path#Hierholzer's_algorithm)
- [Hungarian Algorithm](https://en.wikipedia.org/wiki/Hungarian_algorithm)

---

## 🤝 Contribution

Contributions are welcome! Please follow these steps to contribute:

1. Fork the repository.
2. Create a new branch: `git checkout -b feature-name`.
3. Commit your changes: `git commit -m 'Add new feature'`.
4. Push to the branch: `git push origin feature-name`.
5. Open a Pull Request.

---

## 👩‍💻 Author

**Raquel Teodoro**  
- **Course**: Algorithms I  
- **University**: Federal University of Minas Gerais (UFMG)  
- **Submission**: November, 2024  

---

## 🧪 Testing

Run the program with the provided test cases in the `tests/` directory:

```bash
./tp1 < tests/inputs/testCase01.txt
```

