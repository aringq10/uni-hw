#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "trie.h"

using namespace MyTrie;

bool PASSED = true;

void log(std::ostream& f, const std::string& msg) {
    std::cout << msg << std::endl;
    f << msg << std::endl;
}

void run_test(std::ostream& f, const std::string& testName, bool condition) {
    if (!condition) {
        PASSED = false;
    }
    log(f, "TEST: " + testName + " -> " + (condition ? "PASSED" : "FAILED"));
}

int main() {
    std::ofstream logFile("log.txt");
    if (!logFile.is_open()) return 1;

    log(logFile, "==========================================");
    log(logFile, "TRIE PROJECT DIAGNOSTIC TEST SUITE");
    log(logFile, "==========================================");

    Trie t;

    // Test 1: Insertion and Size
    t += "test";
    t += "testing";
    run_test(logFile, "Size after 2 insertions", t.size() == 2);

    // Test 2: Duplicate Prevention (Logic Check)
    try {
        t += "test";
        run_test(logFile, "Duplicate insertion protection", false);
    } catch (const TrieException& e) {
        log(logFile, "Caught expected TrieException: " + std::string(e.what()));
        run_test(logFile, "Duplicate insertion protection", true);
    }

    // Test 3: Invalid Arguments
    try {
        t += "";
        run_test(logFile, "Empty string protection", false);
    } catch (const std::invalid_argument& e) {
        log(logFile, "Caught expected invalid_argument: " + std::string(e.what()));
        run_test(logFile, "Empty string protection", true);
    }

    // Test 4: Removal Logic
    t -= "test";
    run_test(logFile, "Size after removal", t.size() == 1);
    run_test(logFile, "Contains removed word", t["test"] == false);
    t += "test";

    // Test 5: Deep Copy Independence
    Trie* original = new Trie();
    *original += "independent";
    Trie copy = *original;
    run_test(logFile, "Copy equals original", !(*original != copy));
    delete original;
    run_test(logFile, "Deep copy persistence after original deletion", copy["independent"]);

    // Test 6: Lexicographical Sorting
    Trie a, b;
    a += "apple";
    b += "banana";
    run_test(logFile, "Lexicographical comparison (apple < banana)", a < b);

    // Test 7: Edit Operation
    t.edit("test", "tempo");
    run_test(logFile, "Edit replaces old word", t["test"] == false);
    run_test(logFile, "Edit inserts new word", t["tempo"] == true);
    run_test(logFile, "Size unchanged after edit", t.size() == 2);

    // Test 8: Edit via operator%=
    t %= {"tempo", "tango"};
    run_test(logFile, "operator%= replaces old word", t["tempo"] == false);
    run_test(logFile, "operator%= inserts new word", t["tango"] == true);

    // Test 9: Edit with nonexistent old word
    try {
        t.edit("nonexistent", "something");
        run_test(logFile, "Edit nonexistent word throws", false);
    } catch (const TrieException& e) {
        run_test(logFile, "Edit nonexistent word throws", true);
    }

    // Test 10: Edit with duplicate new word
    try {
        t.edit("tango", "testing");
        run_test(logFile, "Edit to existing word throws", false);
    } catch (const TrieException& e) {
        run_test(logFile, "Edit to existing word throws", true);
    }

    log(logFile, "--- Stress Testing Internal Node Integrity ---");
    std::vector<std::string> words = {"alpha", "beta", "gamma", "delta", "epsilon", "zeta", "eta", "theta"};
    for (const auto& w : words) {
        t += w;
        log(logFile, "Logged insertion of node: " + w + " [OK]");
    }
    
    run_test(logFile, "Final state check", t.size() == 10);
    run_test(logFile, "toString returns correct contents", t.toString() == "alpha\nbeta\ndelta\nepsilon\neta\ngamma\ntango\ntesting\ntheta\nzeta\n");
    log(logFile, "Final Trie Structure Contents:");
    std::cout << t;
    logFile << t;
    
    log(logFile, "==========================================");
    if (PASSED) {
        log(logFile, "ALL TESTS PASSED");
    }
    else {
        log(logFile, "SOME TESTS FAILED");
    }
    log(logFile, "==========================================");

    logFile.close();
    return 0;
}
