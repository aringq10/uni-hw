#pragma once

#include <string>
#include <fstream>
#include <unordered_map>

struct ParseError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct SyntaxError : ParseError {
    using ParseError::ParseError;
};

struct LineTooLongError : ParseError {
    LineTooLongError(size_t cap)
        : ParseError("line too long, cap is " + std::to_string(cap)) {}
};

struct TooManyArguments : ParseError {
    TooManyArguments() : ParseError("too many arguments") {}
};

struct MissingArguments : ParseError {
    MissingArguments() : ParseError("missing arguments") {}
};

struct MissingReturn : ParseError {
    explicit MissingReturn(const std::string& name)
        : ParseError("no return at end of subroutine '" + name + "'") {}
};

enum CommandType {
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_CALL,
    C_FUNCTION,
    C_RETURN,
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
        size_t line_num;
        std::string current_function;
        std::ifstream file;
        Command parse_command();

    public:
        parser(const std::string& src);
        const std::string& get_line();
        size_t get_line_num();
        bool next_command(Command& c);
};
