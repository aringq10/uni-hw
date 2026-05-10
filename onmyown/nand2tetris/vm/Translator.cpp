#include "Translator.h"
#include "utils.h"

#include <cstdio>
#include <cstring>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

constexpr char output_file[] = "out.asm";

static const std::unordered_set<std::string> ARITHMETIC = {
    "add", "sub", "neg", "eq", "gt", "lt", "and", "or", "not"
};

static const std::unordered_set<std::string> SEGMENTS = {
    "argument", "local", "static", "constant", "this", "that",
    "pointer", "temp"
};

int get_index(const std::string& s) {
    try {
        int arg = std::stoi(s);
        if (arg < 0 || arg > 32767) throw std::runtime_error("argument out of range");
        return arg;
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("expected integer");
    } catch (const std::out_of_range&) {
        throw std::runtime_error("argument out of range");
    }
}

void validate_segment(const std::string& s) {
    if (!SEGMENTS.count(s)) throw std::runtime_error("Invalid segment");
}

parser::parser(const std::string& src) : file(src) {
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + src);
    }
}

Command parser::parse_command() {
    std::istringstream ss(line);
    std::string cmd, arg_1, arg_2;
    ss >> cmd >> arg_1 >> arg_2;

    if (ARITHMETIC.count(cmd)) {
        return Command({C_ARITHMETIC, cmd, 0});
    } else if (cmd == "push") {
        validate_segment(arg_1);
        return Command({C_PUSH, arg_1, get_index(arg_2)});
    } else if (cmd == "pop") {
        validate_segment(arg_1);
        return Command({C_PUSH, arg_1, get_index(arg_2)});
    }

    throw std::runtime_error("Unknown command");
}

bool parser::next_command(Command& c, size_t& line_num) {
    char buf[LINE_LIMIT];
    while (file.getline(buf, LINE_LIMIT)) {
        line_num++;
        line = buf;
        remove_comment(line);
        trim(line);
        if (!line.empty()) {
            c = parse_command();
            return true;
        }
    }

    if (file.fail() && !file.eof()) {
        throw std::runtime_error("Line too long");
    }

    if (file.bad()) {
        throw std::runtime_error("I/O error");
    }

    return false;
}

code_writer::code_writer(const std::string& dst) : file(dst) {
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + dst);
    }
}

void code_writer::write_arithmetic(const std::string& command) {
    if (command == "add") {
    } else if (command == "sub") {
    } else if (command == "neg") {
    } else if (command == "eq") {
    } else if (command == "gt") {
    } else if (command == "lt") {
    } else if (command == "and") {
    } else if (command == "or") {
    } else if (command == "not") {
    } else {
        throw std::runtime_error("Unknown arithmetic command: " + command);
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [path/to/src.vm | path/to/src/dir]\n", argv[0]);
        return 2;
    }

    std::vector<std::string> files;

    try {
         files = gather_files(argv[1]);
    } catch (std::runtime_error e) {
        fprintf(stderr, "%s\n", e.what());
        return 1;
    } catch (...) {
        fprintf(stderr, "Unknown error while gathering filenames\n");
        return 1;
    }

    try {
        code_writer code = code_writer(output_file);
        for (const auto& file : files) {
            size_t line_num = 0;
            try {
                parser p = parser(argv[1]);

                Command c;

                while (p.next_command(c, line_num)) {
                    switch (c.type) {
                        case C_ARITHMETIC:
                            code.write_arithmetic(c.arg_1);
                            break;
                        case C_PUSH:
                            code.write_push_pop(c.type, c.arg_1, c.arg_2);
                            break;
                        case C_POP:
                            code.write_push_pop(c.type, c.arg_1, c.arg_2);
                            break;
                        default:
                            throw std::runtime_error("Unknown command type returned from parser");
                            break;
                    }
                }
            } catch (std::runtime_error e) {
                throw std::runtime_error("Line " + std::to_string(line_num) + ": " + e.what());
            }
        }
    } catch (std::runtime_error e) {
        fprintf(stderr, "%s\n", e.what());
        return 1;
    } catch (...) {
        fprintf(stderr, "Unknown error");
        return 1;
    }

    return 0;
}
