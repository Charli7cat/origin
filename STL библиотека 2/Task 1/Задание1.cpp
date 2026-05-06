#include <vector>
#include <unordered_set>
#include <algorithm>

std::vector<int> removeDuplicates(const std::vector<int>& input) {
    std::unordered_set<int> seen;
    std::vector<int> result;

    for (int num : input) {
        if (seen.find(num) == seen.end()) {
            seen.insert(num);
            result.push_back(num);
        }
    }

    std::sort(result.begin(), result.end());
    return result;
}