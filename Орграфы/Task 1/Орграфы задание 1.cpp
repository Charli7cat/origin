#include <iostream>
#include <fstream>
#include <vector>

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

    cout << "The text view of the digraph:" << endl;

    for (int i = 0; i < N; i++) {
        cout << i + 1 << ": ";

        bool hasEdges = false;

        for (int j = 0; j < N; j++) {
            if (matrix[i][j] == 1) {
                cout << j + 1 << " ";
                hasEdges = true;
            }
        }

        if (!hasEdges) {
            cout << "No";
        }

        cout << endl;
    }

    return 0;
}