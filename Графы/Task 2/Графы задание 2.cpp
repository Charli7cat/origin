#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

int main() {
    std::ifstream file("input.txt");

    if (!file.is_open()) {
        std::cerr << "Error: Failed to open input.txt file" << std::endl;
        return 1;
    }

    int N;
    file >> N;

    if (file.fail() || N <= 0) {
        std::cerr << "Error: invalid file format or N <= 0" << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> graph(N, std::vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            file >> graph[i][j];
        }
    }
    file.close();

    std::cout << "In the column " << N << " vertexes. ";

    int startVertex;
    do {
        std::cout << "Enter the vertex number from which the traversal will start: ";
        std::cin >> startVertex;

        if (startVertex < 1 || startVertex > N) {
            std::cout << "Error: the vertex must be between 1 and " << N << ". Try again." << std::endl;
        }
    } while (startVertex < 1 || startVertex > N);

    int startIndex = startVertex - 1;

    std::vector<bool> visited(N, false);
    std::vector<int> result;
    std::queue<int> q;

    visited[startIndex] = true;
    q.push(startIndex);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        result.push_back(current);

        for (int neighbor = 0; neighbor < N; neighbor++) {
            if (graph[current][neighbor] == 1 && !visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    std::cout << "Порядок обхода вершин: ";
    for (std::size_t i = 0; i < result.size(); i++) {
        std::cout << result[i] + 1;
        if (i < result.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}