#include <stdexcept>

struct ParseError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct SyntaxError : ParseError {
    using ParseError::ParseError;
};

struct LineTooLongError : ParseError {
    using ParseError::ParseError;
};
