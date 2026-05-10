#include "parser.h"
#include "utils.h"
#include "errors.h"

#include <sstream>
#include <string>

static void validate_index(const int i) {
    if (i < 0 || i > 32767) throw SyntaxError("index out of range");
}

static void validate_segment(const std::string& s) {
    if (!SEGMENT_MAP.contains(s)) throw SyntaxError("unknown segment '" + s + "'");
}

static size_t get_index(const std::string& s) {
    try {
        int arg = std::stoi(s);
        validate_index(arg);
        return arg;
    } catch (const std::invalid_argument&) {
        throw SyntaxError("expected integer");
    } catch (const std::out_of_range&) {
        throw SyntaxError("index out of range");
    }
}

parser::parser(const std::string& src) : file(src) {
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file '" + src + "'");
    }
}

Command parser::parse_command() {
    std::istringstream ss(line);
    std::string cmd, arg_1, arg_2;
    ss >> cmd >> arg_1 >> arg_2;

    if (ARITHMETIC_MAP.contains(cmd)) {
        return Command({C_ARITHMETIC, cmd, 0});
    } else if (cmd == "push") {
        validate_segment(arg_1);
        return Command({C_PUSH, arg_1, get_index(arg_2)});
    } else if (cmd == "pop") {
        validate_segment(arg_1);
        return Command({C_POP, arg_1, get_index(arg_2)});
    }

    throw SyntaxError("unknown command '" + cmd + "'");
}

const std::string& parser::get_line() {
    return line;
}

bool parser::next_command(Command& c, size_t& line_num, std::string& line_local) {
    char buf[LINE_LIMIT];
    while (file.getline(buf, LINE_LIMIT)) {
        line_num++;
        line = buf;
        line_local = line;
        remove_comment(line);
        trim(line);
        if (!line.empty()) {
            c = parse_command();
            return true;
        }
    }

    if (file.fail() && !file.eof()) {
        line_local = buf;
        throw LineTooLongError("line is too long, limit is " + std::to_string(LINE_LIMIT));
    }

    if (file.bad()) {
        throw std::runtime_error("I/O error");
    }

    return false;
}


