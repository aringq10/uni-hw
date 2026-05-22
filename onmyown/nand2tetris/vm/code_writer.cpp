#include "code_writer.h"
#include "parser.h"

// M and D hold operators, M holds result
static const char* LOAD_2_STACK_ASM[] = {
    "@SP",
    "AM=M-1",
    "D=M",
    "A=A-1",
};

// M holds operator and final result
static const char* LOAD_1_STACK_ASM[] = {
    "@SP",
    "A=M-1",
};

// pushes D
static const char* PUSH_STACK_ASM[] = {
    "@SP",
    "M=M+1",
    "A=M-1",
    "M=D",
};

// pops to M
static const char* POP_STACK_ASM[] = {
    "D=A",
    "@R13",
    "M=D",
    "@SP",
    "M=M-1",
    "A=M",
    "D=M",
    "@R13",
    "A=M",
    "M=D",
};

static const char* ADD_ASM[] = {
    "M=D+M",
};

static const char* SUB_ASM[] = {
    "M=M-D",
};

static const char* NEG_ASM[] = {
    "M=-M",
};

static const char* AND_ASM[] = {
    "M=D&M",
};

static const char* OR_ASM[] = {
    "M=D|M",
};

static const char* NOT_ASM[] = {
    "M=!M",
};

void code_writer::write_lines(std::span<const char*> lines) {
    for (auto& l : lines) {
        file << l << "\n";
    }
}

void code_writer::check_write() {
    if (file.fail()) throw std::runtime_error("Write failed");
}

void code_writer::write_compare(const char* jump) {
    size_t id = compare_label_count++;
    write_lines(LOAD_2_STACK_ASM);
    write_lines(SUB_ASM);
    file << "D=M\n"
         << "@TRUE_" << id << "\n"
         << "D;" << jump << "\n"
         << "@SP\n" << "A=M-1\n" << "M=0\n"
         << "@FALSE_" << id << "\n"
         << "0;JMP\n"
         << "(TRUE_" << id << ")\n"
         << "@SP\n" << "A=M-1\n" << "M=-1\n"
         << "(FALSE_" << id << ")\n";
}

// loads address of push/pop operation into register A
void code_writer::write_load_base_address(const std::string& segment, const size_t index) {
    auto it = SEGMENT_MAP.find(segment);
    if (it == SEGMENT_MAP.end())
        throw SyntaxError("unknown segment '" + segment + "'");

    switch (it->second) {
        case SegmentType::LOCAL:
            file << "@LCL\n" << "D=M\n" << "@" << index << "\n" << "A=D+A\n";
            break;
        case SegmentType::ARGUMENT:
            file << "@ARG\n" << "D=M\n" << "@" << index << "\n" << "A=D+A\n";
            break;
        case SegmentType::THIS:
            file << "@THIS\n" << "D=M\n" << "@" << index << "\n" << "A=D+A\n";
            break;
        case SegmentType::THAT:
            file << "@THAT\n" << "D=M\n" << "@" << index << "\n" << "A=D+A\n";
            break;
        case SegmentType::TEMP:
            file << "@R5\n" << "D=A\n" << "@" << index << "\n" << "A=D+A\n";
            break;
        case SegmentType::POINTER:
            file << "@THIS\n" << "D=A\n" << "@" << index << "\n" << "A=D+A\n";
            break;
        case SegmentType::STATIC:
            file << "@" << src_filename << "." << index << "\n";
            break;
        case SegmentType::CONSTANT:
            throw std::runtime_error("Constant has no base address");
    }
}

/* PUBLIC METHODS */

code_writer::code_writer(const std::string& dst) : file(dst) {
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file '" + dst + "'");
    }
}

void code_writer::set_file_name(const std::string& dst) {
    src_filename = dst;
}

void code_writer::write_init() {
    file << "@256\n"
         << "D=A\n"
         << "@SP\n"
         << "M=D\n";
    write_call({C_CALL, "Sys.init", 0});
    check_write();
}

void code_writer::write_arithmetic(const Command& command) {
    auto it = ARITHMETIC_MAP.find(command.arg_1);
    if (it == ARITHMETIC_MAP.end())
        throw SyntaxError("unknown arithmetic command '" + command.arg_1 + "'");

    switch (it->second) {
        case ArithmeticType::ADD: write_lines(LOAD_2_STACK_ASM); write_lines(ADD_ASM); break;
        case ArithmeticType::SUB: write_lines(LOAD_2_STACK_ASM); write_lines(SUB_ASM); break;
        case ArithmeticType::NEG: write_lines(LOAD_1_STACK_ASM); write_lines(NEG_ASM); break;
        case ArithmeticType::EQ:  write_compare("JEQ"); break;
        case ArithmeticType::GT:  write_compare("JGT"); break;
        case ArithmeticType::LT:  write_compare("JLT"); break;
        case ArithmeticType::AND: write_lines(LOAD_2_STACK_ASM); write_lines(AND_ASM); break;
        case ArithmeticType::OR:  write_lines(LOAD_2_STACK_ASM); write_lines(OR_ASM);  break;
        case ArithmeticType::NOT: write_lines(LOAD_1_STACK_ASM); write_lines(NOT_ASM); break;
    }

    check_write();
}

void code_writer::write_push_pop(const Command& command) {
    std::string segment = command.arg_1;
    size_t index = command.arg_2;

    switch (command.type) {
        case C_PUSH:
            if (segment == "constant") {
                file << "@" << index << "\n"
                     << "D=A" << "\n";
                write_lines(PUSH_STACK_ASM);
                break;
            }

            write_load_base_address(segment, index);
            file << "D=M" << "\n";
            write_lines(PUSH_STACK_ASM);
            break;
        case C_POP:
            if (segment == "constant") {
                throw std::runtime_error("Cannot pop to constant segment");
            }

            write_load_base_address(segment, index);
            write_lines(POP_STACK_ASM);
            break;
        default:
            throw std::runtime_error("Unknown command type passed to write_push_pop");
            break;
    }

    check_write();
}

void code_writer::write_label(const Command& command) {
    file << "(" << command.arg_1 << ")\n";
    check_write();
}

// Limitation of "goto" and "if-goto" commands: if this is the label's first
// definition, the assembler sees it as a variable and doesn't throw an error
void code_writer::write_goto(const Command& command) {
    file << "@" << command.arg_1 << "\n" << "0;JMP\n";
    check_write();
}

void code_writer::write_if(const Command& command) {
    file << "@SP\n"
         << "AM=M-1\n"
         << "D=M\n"
         << "@" << command.arg_1 << "\n"
         << "D;JNE\n";
    check_write();
}

void code_writer::write_call(const Command& command) {
    size_t id = retaddr_label_count++;
    file << "@RETURN_" << id << "\n"
         << "D=A\n";
    write_lines(PUSH_STACK_ASM);
    file << "@LCL\n"
         << "D=M\n";
    write_lines(PUSH_STACK_ASM);
    file << "@ARG\n"
         << "D=M\n";
    write_lines(PUSH_STACK_ASM);
    file << "@THIS\n"
         << "D=M\n";
    write_lines(PUSH_STACK_ASM);
    file << "@THAT\n"
         << "D=M\n";
    write_lines(PUSH_STACK_ASM);
    file << "@SP\n"
         << "D=M\n"
         << "@5\n"
         << "D=D-A\n"
         << "@" << command.arg_2 << "\n"
         << "D=D-A\n"
         << "@ARG\n"
         << "M=D\n"; // new ARG value is set up
    file << "@SP\n"
         << "D=M\n"
         << "@LCL\n"
         << "M=D\n"; // new LCL value is set up
    file << "@" << command.arg_1 << "\n"
         << "0;JMP\n";
    file << "(RETURN_" << id << ")" << "\n";
    check_write();
}

void code_writer::write_function(const Command& command) {
    size_t id = func_loop_label_count++;
    file << "(" << command.arg_1 << ")\n";
    file << "@" << command.arg_2 << "\n"
         << "D=A\n"
         << "(FUNC_LOOP_" << id << ")\n"
         << "@FUNC_LOOP_END_" << id << "\n"
         << "D;JEQ\n"
         << "@SP\n"
         << "A=M\n"
         << "M=0\n"
         << "@SP\n"
         << "M=M+1\n"
         << "D=D-1\n"
         << "@FUNC_LOOP_" << id << "\n"
         << "0;JMP\n"
         << "(FUNC_LOOP_END_" << id << ")\n";
    check_write();
}

void code_writer::write_return() {
    // save LCL value to R15, return address to R14,
    // reposition return value and fix SP for caller
    file << "@LCL\n"
         << "D=M\n"
         << "@R15\n" // R15 holds pointer to LCL
         << "M=D\n";
    file << "@LCL\n"
         << "D=M\n"
         << "@5\n"
         << "A=D-A\n"
         << "D=M\n"
         << "@R14\n"
         << "M=D\n" // R14 holds return address
         << "@ARG\n"
         << "A=M\n";
    write_lines(POP_STACK_ASM); // reposition return value for caller
    file << "@ARG\n"
         << "D=M+1\n"
         << "@SP\n"
         << "M=D\n"; // SP = ARG + 1

    // restore segments
    file << "@R15\n"
         << "D=M\n"
         << "@1\n"
         << "A=D-A\n"
         << "D=M\n" // D holds THAT
         << "@THAT\n"
         << "M=D\n"; // THAT restored
    file << "@R15\n"
         << "D=M\n"
         << "@2\n"
         << "A=D-A\n"
         << "D=M\n" // D holds THIS
         << "@THIS\n"
         << "M=D\n"; // THIS restored
    file << "@R15\n"
         << "D=M\n"
         << "@3\n"
         << "A=D-A\n"
         << "D=M\n" // D holds ARG
         << "@ARG\n"
         << "M=D\n"; // ARG restored
    file << "@R15\n"
         << "D=M\n"
         << "@4\n"
         << "A=D-A\n"
         << "D=M\n" // D holds LCL
         << "@LCL\n"
         << "M=D\n"; // LCL restored

    // goto return address
    file << "@R14\n"
         << "A=M\n" // A holds return address
         << "0;JMP\n";
    check_write();
}
