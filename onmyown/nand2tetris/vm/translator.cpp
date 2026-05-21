#include "parser.h"
#include "code_writer.h"
#include "utils.h"
#include "errors.h"

#include <cstdio>
#include <cstring>
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

    try {
        code_writer code = code_writer(output_file);
        for (const auto& filepath : files) {
            auto filename = std::filesystem::path(filepath).filename().string();
            auto basename = std::filesystem::path(filepath).stem().string();

            code.set_file_name(basename);
            size_t line_num = 0;
            std::string line;

            try {
                parser p = parser(filepath);

                Command c;

                while (p.next_command(c, line_num, line)) {
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
                        default:
                            throw std::runtime_error("Unknown command type returned from parser");
                            break;
                    }
                }
            } catch (LineTooLongError& e) {
                throw ParseError(filename + ":" + std::to_string(line_num) + ": error: " + e.what());
            } catch (ParseError& e) {
                throw ParseError(filename + ":" + std::to_string(line_num) + ": error: " + e.what() + "\n    " + line);
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
