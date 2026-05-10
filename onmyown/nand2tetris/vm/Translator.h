#pragma once

#include <string>
#include <fstream>
#include <unordered_set>

enum CommandType {
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL,
};

struct Command {
    CommandType type;
    std::string arg_1;
    int arg_2;
};

class parser {
    private:
        static constexpr size_t LINE_LIMIT = 256;
        std::string line;
        std::ifstream file;
        Command parse_command();

    public:
        parser(const std::string& src);
        bool next_command(Command& c, size_t& line_num);
};

class code_writer {
    private:
        std::ofstream file;

    public:
        code_writer(const std::string& dst);
        void set_file_name(const std::string& dst);
        void write_arithmetic(const std::string& command);
        void write_push_pop(const CommandType command_type, const std::string& segment, const int index);
};
