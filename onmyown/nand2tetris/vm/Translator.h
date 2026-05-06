#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <string>

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

class Parser {
    private:
        std::string fileName;
        CommandType commandType;
        std::string arg1;
        int arg2;

    public:
        Parser(const std::string& src);
        ~Parser();
        void advance();
        CommandType getCommandType();
        std::string getArg1();
        int getArg2();
};

class CodeWriter {
    private:
        std::string fileName;

    public:
        CodeWriter(const std::string& dst);
        ~CodeWriter();
        void setFileName(const std::string& dst);
        void writeArithmetic(const std::string& cmd);
        void writePushPop(const CommandType cmdType, const std::string& segment, const int index);
};

#endif
