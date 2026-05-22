#include "parser.h"
#include "code_writer.h"
#include "utils.h"

#include <cstdio>
#include <exception>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

constexpr char output_file[] = "out.asm";

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [path/to/src.vm | path/to/src/dir]\n", argv[0]);
        return 2;
    }

    std::vector<std::string> files;

    try {
         files = gather_files(argv[1]);
    } catch (std::exception& e) {
        fprintf(stderr, "%s\n", e.what());
        return 1;
    } catch (...) {
        fprintf(stderr, "Unknown error while gathering filenames\n");
        return 1;
    }

    std::filesystem::path p(argv[1]);
    bool is_dir = std::filesystem::is_directory(p);

    try {
        code_writer code = code_writer(output_file);

        if (is_dir) {
            code.write_init();
        }

        for (const auto& filepath : files) {
            auto filename = std::filesystem::path(filepath).filename().string();
            auto basename = std::filesystem::path(filepath).stem().string();

            code.set_file_name(basename);

            parser p = parser(filepath);

            Command c;

            try {
                while (p.next_command(c)) {
                    switch (c.type) {
                        case C_ARITHMETIC:
                            code.write_arithmetic(c);
                            break;
                        case C_PUSH:
                            code.write_push_pop(c);
                            break;
                        case C_POP:
                            code.write_push_pop(c);
                            break;
                        case C_LABEL:
                            code.write_label(c);
                            break;
                        case C_GOTO:
                            code.write_goto(c);
                            break;
                        case C_IF:
                            code.write_if(c);
                            break;
                        case C_CALL:
                            code.write_call(c);
                            break;
                        case C_FUNCTION:
                            code.write_function(c);
                            break;
                        case C_RETURN:
                            code.write_return();
                            break;
                        default:
                            throw std::runtime_error("Unknown command type returned from parser");
                            break;
                    }
                }
            } catch (LineTooLongError& e) {
                throw ParseError(filename + ":" + std::to_string(p.get_line_num()) + ": error: " + e.what());
            } catch (ParseError& e) {
                throw ParseError(filename + ":" + std::to_string(p.get_line_num()) + ": error: " + e.what() + "\n    " + p.get_line());
            }
        }
    } catch (ParseError& e) {
        fprintf(stderr, "%s\n", e.what());
        return 1;
    } catch (std::exception& e) {
        fprintf(stderr, "Exception thrown: %s\n", e.what());
        return 1;
    } catch (...) {
        fprintf(stderr, "Unknown error");
        return 1;
    }

    return 0;
}
