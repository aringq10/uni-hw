# MyTrie Library

This project is a **Prefix Tree (Trie)** implementation for C++17. Implements the Pimpl idiom. Supports basic string operations, deep copying and lexicographical comparison between different Trie instances.

## Project Structure

* **`trie.h`, `trie.cpp`**: The public interface and implementation.
* **`demo.cpp`**: A demonstration showing standard library usage.
* **`test.cpp`**: A diagnostic suite that tests edge cases and logs results to `log.txt`.
* **`Makefile`**: Project Makefile

## How to Build and Run

1.  **Build All**:
    ```bash
    make [all]
    ```
2.  **Run Demonstration**:
    ```bash
    make run_demo
    ```
3.  **Run tests** (Generates `log.txt`):
    ```bash
    make run_test
    ```
4.  **Clean binaries and log file**:
    ```bash
    make clean
    ```
5.  **Rebuild (clean and build all)**:
    ```bash
    make rebuild
    ```

This library was developed with the assistance of **Gemini** AI.

