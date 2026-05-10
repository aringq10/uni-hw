#include "code_writer.h"
#include "parser.h"
#include "errors.h"

void code_writer::set_file_name(const std::string& dst) {
    src_filename = dst;
}

void code_writer::check_write() {
    if (file.fail()) throw std::runtime_error("Write failed");
}

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

void code_writer::write_compare(const char* jump) {
    int id = compare_label_count++;
    write_lines(LOAD_2_STACK_ASM);
    write_lines(SUB_ASM);
    file << "D=M\n"
         << "@TRUE_" << id << "\n"
         << "D;" << jump << "\n"
         << "@SP\n" << "A=M-1\n" << "M=0\n"
         << "@END_" << id << "\n"
         << "0;JMP\n"
         << "(TRUE_" << id << ")\n"
         << "@SP\n" << "A=M-1\n" << "M=-1\n"
         << "(END_" << id << ")\n";
}

code_writer::code_writer(const std::string& dst) : file(dst) {
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file '" + dst + "'");
    }
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
