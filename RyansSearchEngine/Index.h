#pragma once

#include <unordered_map>
#include <vector>
#include <string>

std::unordered_map<std::string, std::string> createIndex(const std::vector<std::pair<std::string, std::string>>& qa_pairs);
