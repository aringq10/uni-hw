#pragma once

#include <string>
#include <vector>

std::vector<std::string> gather_files(const char* path);
void remove_comment(std::string& line);
void trim(std::string& line);
