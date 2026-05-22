#pragma once

#include "parser.h"
#include <span>
#include <string>
#include <fstream>

class code_writer {
    private:
        std::ofstream file;
        std::string src_filename;
        size_t compare_label_count = 0;
        size_t retaddr_label_count = 0;
        size_t func_loop_label_count = 0;

        void write_lines(std::span<const char*> lines);
        void check_write();
        void write_compare(const char* jump);
        void write_load_base_address(const std::string& segment, const size_t index);

    public:
        code_writer(const std::string& dst);
        void set_file_name(const std::string& dst);
        void write_init();
        void write_arithmetic(const Command& command);
        void write_push_pop(const Command& command);
        void write_label(const Command& command);
        void write_goto(const Command& command);
        void write_if(const Command& command);
        void write_call(const Command& command);
        void write_function(const Command& command);
        void write_return();
};
