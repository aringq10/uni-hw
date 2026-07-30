#include <map>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include "trie.h"

namespace MyTrie {
    TrieException::TrieException(const std::string& message)
        : std::runtime_error("Trie Error: " + message) {}

    class Trie::Impl {
        private:
            friend class Trie;

            struct Node {
                private:
                    friend class Trie::Impl;

                    std::map<char, std::unique_ptr<Node>> children;
                    bool isEndOfWord = false;

                public:
                    std::unique_ptr<Node> clone() const {
                        auto newNode = std::make_unique<Node>();
                        newNode->isEndOfWord = this->isEndOfWord;

                        for (auto const& [ch, childPtr] : children) {
                            newNode->children[ch] = childPtr->clone();
                        }
                        return newNode;
                    }

                    bool isEqual(const Node* other) const {
                        if (!other || isEndOfWord != other->isEndOfWord || children.size() != other->children.size()) {
                            return false;
                        }

                        for (auto const& [ch, childPtr] : children) {
                            auto it = other->children.find(ch);
                            if (it == other->children.end() || !childPtr->isEqual(it->second.get())) {
                                return false;
                            }
                        }
                        return true;
                    }
            };

            int size = 0;
            std::unique_ptr<Node> root;

            bool removeRecursive(Node* current, const std::string& word, const size_t index, bool& removed) {
                if (index == word.length()) {
                    if (!current->isEndOfWord) {
                        removed = false;
                        return false;
                    }
                    else {
                        removed = true;
                    }

                    current->isEndOfWord = false;
                    return current->children.empty();
                }

                char ch = word[index];
                if (current->children.find(ch) == current->children.end()) {
                    removed = false;
                    return false;
                }

                bool shouldDeleteChild = removeRecursive(current->children[ch].get(), word, index + 1, removed);
                if (shouldDeleteChild) {
                    current->children.erase(ch);
                    return current->children.empty() && !current->isEndOfWord;
                }
                return false;
            }

        public:
            Impl() : root(std::make_unique<Node>()) {}

            bool insertHelper(Node* current, const std::string& word, const size_t index) {
                bool inserted = false;

                for (size_t i = index; i < word.length(); i++) {
                    char ch = word[i];
                    if (current->children.find(ch) == current->children.end()) {
                        current->children[ch] = std::make_unique<Node>();
                    }
                    current = current->children[ch].get();
                }

                if (!current->isEndOfWord) {
                    inserted = true;
                }
                current->isEndOfWord = true;
                return inserted;
            }

            bool containsHelper(const Node* current, const std::string& word, const size_t index) const {
                for (size_t i = index; i < word.length(); i++) {
                    char ch = word[i];
                    auto it = current->children.find(ch);
                    if (it == current->children.end()) {
                        return false;
                    }
                    current = it->second.get();
                }
                return current->isEndOfWord;
            }

            bool removeHelper(Node* current, const std::string& word, const size_t index) {
                bool removed;
                removeRecursive(current, word, index, removed);
                return removed;
            }

            void clearTrie() {
                root = std::make_unique<Node>();
                size = 0;
            }

            static int compare(const Node* a, const Node* b) {
                if (a == b) {
                    return 0;
                }
                if (a == nullptr) {
                    return -1;
                }
                if (b == nullptr) {
                    return 1;
                }
                if (a->isEndOfWord != b->isEndOfWord) {
                    return a->isEndOfWord ? -1 : 1; 
                }

                auto itA = a->children.begin();
                auto itB = b->children.begin();

                while (itA != a->children.end() && itB != b->children.end()) {
                    if (itA->first < itB->first) {
                        return -1;
                    }
                    if (itA->first > itB->first) {
                        return 1;
                    }

                    int res = compare(itA->second.get(), itB->second.get());
                    if (res != 0) {
                        return res;
                    }

                    ++itA; ++itB;
                }

                if (a->children.size() < b->children.size()) {
                    return -1;
                }
                if (a->children.size() > b->children.size()) {
                    return 1;
                }

                return 0;
            }

            void printRecursive(const Node* node, const std::string& prefix, std::ostream& os) const {
                if (node->isEndOfWord) {
                    os << prefix << "\n";
                }

                for (auto const& [ch, child] : node->children) {
                    printRecursive(child.get(), prefix + ch, os);
                }
            }
    };

    Trie::Trie() : pImpl(std::make_unique<Impl>()) {}

    Trie::~Trie() = default;

    Trie::Trie(const Trie& other) : pImpl(std::make_unique<Impl>()) {
        if (other.pImpl->root) {
            pImpl->root = other.pImpl->root->clone();
            pImpl->size = other.pImpl->size;
        }
    }

    Trie& Trie::operator=(const Trie& other) {
        if (this != &other) {
            auto newRoot = other.pImpl->root->clone(); 
            pImpl->root = std::move(newRoot);
            pImpl->size = other.pImpl->size;
        }
        return *this;
    }

    int Trie::size() const noexcept {
        return pImpl->size;
    }

    std::string Trie::toString() const {
        std::stringstream ss;
        pImpl->printRecursive(pImpl->root.get(), "", ss);
        return ss.str();
    }

    void Trie::toStream(std::ostream& os) const {
        pImpl->printRecursive(pImpl->root.get(), "", os);
    }

    std::ostream& operator<<(std::ostream& os, const Trie& trie) {
        trie.toStream(os);
        return os;
    }

    void Trie::insert(const std::string& word) {
        if (word.empty()) {
            throw std::invalid_argument("Trie does not support empty strings.");
        }

        if (pImpl->insertHelper(pImpl->root.get(), word, 0)) {
            pImpl->size++;
        }
        else {
            throw TrieException("Attempted to insert existing word '" + word + "'.");
        }
    }

    bool Trie::contains(const std::string& word) const noexcept {
        return pImpl->containsHelper(pImpl->root.get(), word, 0);
    }

    void Trie::remove(const std::string& word) {
        if (word.empty()) {
            throw std::invalid_argument("Word cannot be empty.");
        }

        if (pImpl->removeHelper(pImpl->root.get(), word, 0)) {
            pImpl->size--;
        }
        else {
            throw TrieException("Attempted to remove word '" + word + "' which does not exist.");
        }
    }

    void Trie::edit(const std::string& oldWord, const std::string& newWord) {
        if (oldWord.empty() || newWord.empty()) {
            throw std::invalid_argument("Trie does not support empty strings.");
        }

        if (!contains(oldWord)) {
            throw TrieException("Attempted to edit word '" + oldWord + "' which does not exist.");
        }

        if (contains(newWord)) {
            throw TrieException("Attempted to edit to word '" + newWord + "' which already exists.");
        }

        remove(oldWord);
        insert(newWord);
    }

    void Trie::clear() {
        pImpl->clearTrie();
    }

    Trie& Trie::operator+=(const std::string& word) {
        insert(word);
        return *this;
    }

    Trie& Trie::operator-=(const std::string& word) {
        remove(word);
        return *this;
    }

    bool Trie::operator[](const std::string& word) const noexcept {
        return contains(word);
    }

    Trie& Trie::operator%=(const std::pair<std::string, std::string>& e) {
        edit(e.first, e.second);
        return *this;
    }

    void Trie::operator!() {
        clear();
    }

    bool Trie::operator==(const Trie& other) const noexcept {
        return Impl::compare(pImpl->root.get(), other.pImpl->root.get()) == 0;
    }

    bool Trie::operator!=(const Trie& other) const noexcept {
        return (*this == other);
    }

    bool Trie::operator<(const Trie& other) const noexcept {
        return Impl::compare(pImpl->root.get(), other.pImpl->root.get()) < 0;
    }

    bool Trie::operator<=(const Trie& other) const noexcept {
        return Impl::compare(pImpl->root.get(), other.pImpl->root.get()) <= 0;
    }

    bool Trie::operator>(const Trie& other) const noexcept {
        return Impl::compare(pImpl->root.get(), other.pImpl->root.get()) > 0;
    }

    bool Trie::operator>=(const Trie& other) const noexcept {
        return Impl::compare(pImpl->root.get(), other.pImpl->root.get()) >= 0;
    }
}
