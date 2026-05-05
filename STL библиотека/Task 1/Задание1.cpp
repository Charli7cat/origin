#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int main() {
    string text;

    cout << "[IN]: ";
    getline(cin, text);

    map<char, int> freq;
    for (char c : text) {
        freq[c]++;
    }

    vector<pair<char, int>> freq_vec(freq.begin(), freq.end());

    sort(freq_vec.begin(), freq_vec.end(),
        [](const pair<char, int>& a, const pair<char, int>& b) {
            if (a.second != b.second) {
                return a.second > b.second;
            }
            return a.first < b.first;
        });

    cout << "[OUT]:" << endl;
    for (const auto& p : freq_vec) {
        cout << p.first << ": " << p.second << endl;
    }

    return 0;
}