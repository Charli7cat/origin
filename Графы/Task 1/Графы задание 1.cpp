#include <iostream>
#include <fstream>
#include <vector>

void dfs(const std::vector<std::vector<int>>& graph, std::vector<bool>& visited, int vertex, std::vector<int>& result) {
    visited[vertex] = true;
    result.push_back(vertex);

    for (int neighbor = 0; neighbor < (int)graph.size(); neighbor++) {
        if (graph[vertex][neighbor] == 1 && !visited[neighbor]) {
            dfs(graph, visited, neighbor, result);
        }
    }
}

int main() {
    std::ifstream file("input.txt");

    int N;
    file >> N;

    std::vector<std::vector<int>> graph(N, std::vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            file >> graph[i][j];
        }
    }
    file.close();

    std::vector<bool> visited(N, false);

    std::vector<int> result;

    dfs(graph, visited, 0, result);

    std::cout << "Vertex traversal procedure: ";
    for (std::size_t i = 0; i < result.size(); i++) {
        std::cout << result[i] + 1;
        if (i < result.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}