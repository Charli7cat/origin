#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;

    set<int, greater<int>> numbers;

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        numbers.insert(x);
    }

    for (int num : numbers) {
        cout << num << endl;
    }

    return 0;
}