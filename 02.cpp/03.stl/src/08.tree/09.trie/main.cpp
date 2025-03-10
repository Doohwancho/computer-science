#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class TrieNode {
public:
    bool isEndOfWord;
    std::unordered_map<char, TrieNode*> children;

    TrieNode() : isEndOfWord(false) {}

    ~TrieNode() {
        for(auto& pair: children) {
            delete pair.second;
        }
    }
};

class Trie {
private: 
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        delete root;
    }

    // insert a word into the trie 
    void insert(const std::string& word) {
        TrieNode* current = root;

        for(char c : word) {
            if(current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }

        current->isEndOfWord = true;
    }

    // search for a word in the trie
    bool search(const std::string& word) {
        TrieNode* node = findNode(word);
        return node != nullptr && node->isEndOfWord;
    }

    // check if there is any word in the trie that starts with the given prefix 
    bool startsWith(const std::string& prefix) {
        return findNode(prefix) != nullptr;
    }

    // get all words with the given prefix
    std::vector<std::string> getAllWithPrefix(const std::string& prefix) {
        std::vector<std::string> results;
        TrieNode* prefixNode = findNode(prefix);

        if(prefixNode != nullptr) {
            std::string currentWord = prefix;
            collectWords(prefixNode, currentWord, results);
        }

        return results;
    }


private:
    TrieNode* findNode(const std::string& prefix) {
        TrieNode* current = root;
        
        for(char c : prefix) {
            if(current->children.find(c) == current->children.end()) {
                return nullptr;
            }
            current = current->children[c];
        }

        return current;
    }

    void collectWords(TrieNode* node, std::string& currentWord, std::vector<std::string>& results) {
        if(node->isEndOfWord) {
            results.push_back(currentWord);
        }

        for(const auto& pair : node->children) {
            char c = pair.first;
            TrieNode* child = pair.second;

            currentWord.push_back(c);
            collectWords(child, currentWord, results);
            currentWord.pop_back(); //backtrace 
        }
    }
};

int main() {
    Trie trie;
    
    trie.insert("hello");
    trie.insert("world");
    trie.insert("hell");
    trie.insert("hi");
    trie.insert("her");

    // Search for words
    std::cout << "Search 'hello': " << (trie.search("hello") ? "Found" : "Not found") << std::endl;
    std::cout << "Search 'help': " << (trie.search("help") ? "Found" : "Not found") << std::endl;
    std::cout << "Search 'he': " << (trie.search("he") ? "Found" : "Not found") << std::endl;
    
    // Check prefixes
    std::cout << "Starts with 'he': " << (trie.startsWith("he") ? "Yes" : "No") << std::endl;
    std::cout << "Starts with 'hat': " << (trie.startsWith("hat") ? "Yes" : "No") << std::endl;
    
    // Get all words with prefix
    std::cout << "Words starting with 'h': ";
    auto words = trie.getAllWithPrefix("h");
    for (const auto& word : words) {
        std::cout << word << " ";
    }
    std::cout << std::endl;

    return 0;
}
