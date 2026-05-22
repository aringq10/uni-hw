#include "parser.h"
#include "utils.h"

#include <sstream>
#include <string>
#include <regex>

static void validate_number(const int i) {
    if (i < 0 || i > 32767) throw SyntaxError("number out of range");
}

static void validate_symbol(const std::string& s) {
    static const std::regex pattern(R"(^[A-Za-z_.:][A-Za-z0-9_.:]*$)");
    if (!std::regex_match(s, pattern)) throw SyntaxError("invalid symbol '" + s + "'");
}

static void validate_segment(const std::string& s) {
    if (!SEGMENT_MAP.contains(s)) throw SyntaxError("unknown segment '" + s + "'");
}

static size_t get_number(const std::string& s) {
    try {
        int arg = std::stoi(s);
        validate_number(arg);
        return arg;
    } catch (const std::invalid_argument&) {
        throw SyntaxError("expected integer");
    } catch (const std::out_of_range&) {
        throw SyntaxError("number out of range");
    }
}

Command parser::parse_command() {
    std::istringstream ss(line);
    std::string cmd, arg_1, arg_2, extra;
    ss >> cmd >> arg_1 >> arg_2;

    if (ss >> extra) {
        throw TooManyArguments();
    }

    if (ARITHMETIC_MAP.contains(cmd)) {
        if (!arg_1.empty() || !arg_2.empty()) {
            throw TooManyArguments();
        }
        return Command({C_ARITHMETIC, cmd, 0});
    }
    else if (cmd == "push") {
        if (arg_1.empty() || arg_2.empty()) {
            throw MissingArguments();
        }
        validate_segment(arg_1);
        return Command({C_PUSH, arg_1, get_number(arg_2)});
    } else if (cmd == "pop") {
        if (arg_1.empty() || arg_2.empty()) {
            throw MissingArguments();
        }
        validate_segment(arg_1);
        return Command({C_POP, arg_1, get_number(arg_2)});
    } else if (cmd == "label") {
        if (!arg_2.empty()) {
            throw TooManyArguments();
        }
        if (arg_1.empty()) {
            throw MissingArguments();
        }
        validate_symbol(arg_1);
        std::string label = current_function.empty() ? arg_1 : current_function + "$" + arg_1;
        return Command({C_LABEL, label, 0});
    } else if (cmd == "goto") {
        if (!arg_2.empty()) {
            throw TooManyArguments();
        }
        if (arg_1.empty()) {
            throw MissingArguments();
        }
        std::string label = current_function.empty() ? arg_1 : current_function + "$" + arg_1;
        return Command({C_GOTO, label, 0});
    } else if (cmd == "if-goto") {
        if (!arg_2.empty()) {
            throw TooManyArguments();
        }
        if (arg_1.empty()) {
            throw MissingArguments();
        }
        std::string label = current_function.empty() ? arg_1 : current_function + "$" + arg_1;
        return Command({C_IF, label, 0});
    } else if (cmd == "function") {
        if (arg_1.empty() || arg_2.empty()) {
            throw MissingArguments();
        }
        validate_symbol(arg_1);
        current_function = arg_1;
        return Command({C_FUNCTION, arg_1, get_number(arg_2)});
    } else if (cmd == "call") {
        if (arg_1.empty() || arg_2.empty()) {
            throw MissingArguments();
        }
        return Command({C_CALL, arg_1, get_number(arg_2)});
    } else if (cmd == "return") {
        if (!arg_1.empty() || !arg_2.empty()) {
            throw TooManyArguments();
        }
        current_function = "";
        return Command({C_RETURN, "", 0});
    }

    throw SyntaxError("unknown command '" + cmd + "'");
}

parser::parser(const std::string& src) : line_num(0), file(src) {
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file '" + src + "'");
    }
}

const std::string& parser::get_line() {
    return line;
}

size_t parser::get_line_num() {
    return line_num;
}

bool parser::next_command(Command& c) {
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
        line_num++;
        line = buf;
        throw LineTooLongError(LINE_LIMIT);
    }

    if (file.bad()) {
        throw std::runtime_error("I/O error");
    }

    return false;
}
