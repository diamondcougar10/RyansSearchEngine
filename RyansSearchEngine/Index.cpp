#include "index.h"

std::unordered_map<std::string, std::string> createIndex(const std::vector<std::pair<std::string, std::string>>& qa_pairs) {
    std::unordered_map<std::string, std::string> index;
    for (const auto& pair : qa_pairs) {
        index[pair.first] = pair.second;
    }
    return index;
}
