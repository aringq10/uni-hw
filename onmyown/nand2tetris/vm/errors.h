#include <stdexcept>
#include <string>

struct ParseError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct SyntaxError : ParseError {
    using ParseError::ParseError;
};

struct LineTooLongError : ParseError {
    LineTooLongError(size_t cap) : ParseError("line too long, cap is " + std::to_string(cap)) {}
};

struct TooManyArguments : ParseError {
    TooManyArguments() : ParseError("too many arguments") {}
};

struct MissingArguments : ParseError {
    MissingArguments() : ParseError("missing arguments") {}
};
