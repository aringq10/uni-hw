#pragma once

#include <string>
#include <fstream>
#include <unordered_map>

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

enum class ArithmeticType { ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT };

static const std::unordered_map<std::string, ArithmeticType> ARITHMETIC_MAP = {
    {"add", ArithmeticType::ADD},
    {"sub", ArithmeticType::SUB},
    {"neg", ArithmeticType::NEG},
    {"eq",  ArithmeticType::EQ},
    {"gt",  ArithmeticType::GT},
    {"lt",  ArithmeticType::LT},
    {"and", ArithmeticType::AND},
    {"or",  ArithmeticType::OR},
    {"not", ArithmeticType::NOT},
};

enum class SegmentType {
    LOCAL, ARGUMENT, THIS, THAT,
    TEMP, POINTER, STATIC, CONSTANT
};

static const std::unordered_map<std::string, SegmentType> SEGMENT_MAP = {
    {"local",    SegmentType::LOCAL},
    {"argument", SegmentType::ARGUMENT},
    {"this",     SegmentType::THIS},
    {"that",     SegmentType::THAT},
    {"temp",     SegmentType::TEMP},
    {"pointer",  SegmentType::POINTER},
    {"static",   SegmentType::STATIC},
    {"constant", SegmentType::CONSTANT},
};

struct Command {
    CommandType type;
    std::string arg_1;
    size_t arg_2;
};

class parser {
    private:
        static constexpr size_t LINE_LIMIT = 256;
        std::string line;
        std::ifstream file;
        Command parse_command();

    public:
        parser(const std::string& src);
        const std::string& get_line();
        bool next_command(Command& c, size_t& line_num, std::string& line);
};
