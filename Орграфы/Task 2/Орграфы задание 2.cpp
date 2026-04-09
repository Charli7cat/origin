#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    ifstream inputFile("input.txt");

    if (!inputFile.is_open()) {
        cerr << "File opening error input.txt" << endl;
        return 1;
    }

    int N;
    inputFile >> N;

    vector<vector<int>> matrix(N, vector<int>(N));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            inputFile >> matrix[i][j];
        }
    }

    inputFile.close();

    vector<int> inDegree(N, 0);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] == 1) {
                inDegree[j]++;
            }
        }
    }

    queue<int> zeroInDegree;

    for (int i = 0; i < N; i++) {
        if (inDegree[i] == 0) {
            zeroInDegree.push(i);
        }
    }

    vector<int> topologicalOrder;

    while (!zeroInDegree.empty()) {
        int vertex = zeroInDegree.front();
        zeroInDegree.pop();
        topologicalOrder.push_back(vertex);

        for (int j = 0; j < N; j++) {
            if (matrix[vertex][j] == 1) {
                inDegree[j]--;
                if (inDegree[j] == 0) {
                    zeroInDegree.push(j);
                }
            }
        }
    }

    if (topologicalOrder.size() != N) {
        cerr << "Error: The graph contains a cycle, and topological sorting is not possible" << endl;
        return 1;
    }

    cout << "Topological order of vertices: ";
    for (int i = 0; i < N; i++) {
        cout << topologicalOrder[i] + 1;
        if (i < N - 1) cout << " ";
    }
    cout << endl;

    return 0;
}