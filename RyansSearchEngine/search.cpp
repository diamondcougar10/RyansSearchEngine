#include "search.h"

std::string search(const std::unordered_map<std::string, std::string>& index, const std::string& query) {
    if (index.find(query) != index.end()) {
        return index.at(query);
    }
    else {
        return "";
    }
}
