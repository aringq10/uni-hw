#include "utils.h"
#include <cstddef>
#include <filesystem>

std::vector<std::string> gather_files(const char* path) {
    namespace fs = std::filesystem;

    std::vector<std::string> files;
    fs::path p(path);

    if (fs::is_regular_file(p)) {
        if (p.extension() != ".vm")
            throw std::runtime_error("File must have .vm extension");
        files.push_back(p.string());
    } else if (fs::is_directory(p)) {
        for (const auto& entry : fs::directory_iterator(p)) {
            if (entry.is_regular_file() && entry.path().extension() == ".vm")
                files.push_back(entry.path().string());
        }
        if (files.empty())
            throw std::runtime_error("No .vm files found in directory");
    } else {
        throw std::runtime_error("Path does not exist");
    }

    return files;
}

void remove_comment(std::string& line) {
    size_t pos = line.find("//");
    if (pos != std::string::npos) line.erase(pos);
}

void trim(std::string& line) {
    size_t start = line.find_first_not_of(" \t\r");
    if (start == std::string::npos) { line.clear(); return; }
    size_t end = line.find_last_not_of(" \t\r");
    line = line.substr(start, end - start + 1);
}
