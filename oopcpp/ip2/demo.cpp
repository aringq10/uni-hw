#include <iostream>
#include "trie.h"

using namespace MyTrie;

int main() {
    std::cout << "--- MyTrie Library Demonstration ---\n";

    Trie myTrie;

    // CRUD Operations with Operators
    myTrie += "apple";
    myTrie += "apply";
    myTrie += "banana";
    std::cout << "Inserted: apple, apply, banana\n";

    // Size and Contains
    std::cout << "Current Size: " << myTrie.size() << "\n";
    std::cout << "Contains 'apple'? " << std::boolalpha << myTrie["apple"] << "\n";

    // Subscript and Search
    if (myTrie["apply"]) {
        std::cout << "Search for 'apply' successful using [] operator.\n";
    }

    // toString and Stream Output
    std::cout << "\nFull Trie Contents (toString):\n" << myTrie.toString();
    std::cout << "\nDirect Stream Output:\n" << myTrie << "\n";

    // Comparison Logic
    Trie otherTrie;
    otherTrie += "apple";
    std::cout << "Other trie created, contents:\n" << otherTrie << "\n";
    std::cout << "Comparison: (myTrie == otherTrie) is " << (myTrie == otherTrie) << "\n";
    std::cout << "Comparison: (otherTrie < myTrie) is " << (otherTrie < myTrie) << "\n";

    // Copying and RAII
    Trie copy = myTrie;
    std::cout << "Deep copy created. Copy size: " << copy.size() << ", myTrie size: " << myTrie.size() << "\n";
    std::cout << "Comparison: (copy == myTrie) is " << (copy == myTrie)<< "\n";

    // Removal and Clearing
    myTrie -= "apple";
    std::cout << "After removing 'apple', size: " << myTrie.size() << "\n";

    !myTrie; // Clear using operator!
    std::cout << "After operator!, size: " << myTrie.size() << "\n";

    std::cout << "\nDemo completed successfully.\n";
    return 0;
}
