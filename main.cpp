#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Each node in the Trie
class TrieNode
{
public:
    // Each node has up to 128 children (for all ASCII)
    TrieNode *children[128];

    // Marks if this node completes a word
    bool isEndOfWord;

    // Constructor
    TrieNode()
    {
        isEndOfWord = false;
        for (int i = 0; i < 128; i++)
        {
            children[i] = nullptr;
        }
    }
};

// The Trie class
class Trie
{
private:
    TrieNode *root;
    /*count->mohammed
    longest->abdelmaseeh
    remove->ahmed-fares
    spell ->jana
    uppercase->jana
    */

    // convert to lowercase
    string toLowercase(string word)
    {
        string result = word;
        for (char &c : result)
        {
            c = tolower(c);
        }
        return result;
    }

    // Helper function to find all words from a node
    // Input: current node, current word formed so far, results vector to store words
    // Output: none (modifies results vector by reference)
    // Purpose: Recursively find all complete words starting from the given node
    bool removeword(TrieNode *node, string word, int depth)
    {
        if (!node)
            return false;
        if (depth == word.size())
        {
            if (!node->isEndOfWord)
                return false; // passed over all words and didn't find word end so false

            node->isEndOfWord = false; // this removes the end flag to disappear the funtion if it has other branches it won't delte the common letters but will remove the flag only whic make the word not found
            for (int i = 0; i < 128; i++)
            {
                if (node->children[i]) // node mustn't have children it only will remove is end of word
                    return false;
            }
            return true;
        }
        int index = word[depth];
        if (removeword(node->children[index], word, depth + 1))
        {
            delete node->children[index]; // after each delete the index recursively moves back to prev index
            node->children[index] = nullptr;
            if (!node->isEndOfWord)
            {
                for (int i = 0; i < 128; i++)
                    if (node->children[i]) // if node in the middle has branches so we will not  delete
                        return false;
                return true;
            }
        }
        return false;
    }

    void dfsLongest(const TrieNode *node, string &path, string &best) const
    { // looking for the longest word
        if (!node)
            return; // If we reach the end of a word the best happens(its longest)

        if (node->isEndOfWord)
        {
            if (path.size() > best.size() || (path.size() == best.size() && path > best))
            {
                best = path; // In the case of equal length we take the smallest linguistically
            }
        }

        for (int i = 0; i < 128; ++i)
        { // Traverse all possible children of this node
            if (node->children[i])
            {                                              // If there is a child node at index i (corresponding to some character)
                path.push_back(static_cast<char>(i));      // Add that character to the current path
                dfsLongest(node->children[i], path, best); // Recurse into the child node to continue building the word
                path.pop_back();
            }
        }
    }
    void findAllWords(TrieNode *node, string currentWord, vector<string> &results) // mohammad
    {
        if (node == nullptr)
            return;
        if (node->isEndOfWord)
        {
            results.push_back(currentWord);
        }
        for (int i = 0; i < 128; i++)
        {
            if (node->children[i] != nullptr)
            {
                char nextChar = static_cast<char>(i);
                findAllWords(node->children[i], currentWord + nextChar, results);
            }
        }
    }

    // Helper function to count all words in the Trie
    // Input: current node
    // Output: integer count of words
    // Purpose: Recursively count all nodes that mark the end of a word
    int countWords(TrieNode *node)
    {
        if (node == nullptr)
            return 0;
        
        int count = 0;
        if (node->isEndOfWord)
        {
            count = 1;
        }
        
        for (int i = 0; i < 128; i++)
        {
            if (node->children[i] != nullptr)
            {
                count += countWords(node->children[i]);
            }
        }
        
        return count;
    }

public:
    // Constructor
    // Input: none
    // Output: none
    // Purpose: Initialize the Trie with a root node
    Trie()
    {
        // TODO: Implement this function
        root = new TrieNode();
    }

    // Insert a word into the Trie
    // Input: word to insert (string)
    // Output: none
    // Purpose: Add a word to the Trie by creating nodes for each character

    void insert(string word) // abdelmaseeh

    {
        string lowerWord = toLowercase(word);
        TrieNode *node = root;
        for (char c : lowerWord)
        {
            int index = (int)c;
            if (node->children[index] == nullptr)
            {
                node->children[index] = new TrieNode();
            }
            node = node->children[index];
        }
        node->isEndOfWord = true;
    }

    // Search for a word in the Trie
    // Input: word to search for (string)
    // Output: boolean indicating if the word exists
    // Purpose: Check if the complete word exists in the Trie

    bool search(string word)
    { // jana
        // TODO: Implement this function
        string lowerWord = toLowercase(word);
        TrieNode *current = root;

        for (char c : lowerWord)
        {
            int index = (int)c;

            if (current->children[index] == nullptr)
            {
                return false;
            }

            current = current->children[index];
        }
        return current->isEndOfWord;
    }

    bool checkSpelling(string word)
    {
        return search(word);
    }

    // Check if any word starts with the given prefix
    // Input: prefix to check (string)
    // Output: boolean indicating if any word has this prefix
    // Purpose: Verify if the prefix exists in the Trie (doesn't need to be a complete word)

    bool startsWith(string prefix) // ahmed

    {
        string lowerPrefix = toLowercase(prefix);
        TrieNode *current = root;
        for (char c : lowerPrefix)
        {
            int index = (int)c; // gives the index of char.
            if (current->children[index] == nullptr)
            {
                return false;
            }
            else
            {
                current = current->children[index];
            }
        }
        // TODO: Implement this function
        return true; // placeholder
    }

    // Get all words that start with the given prefix
    // Input: prefix to complete (string)
    // Output: vector of strings that start with the prefix
    // Purpose: Find all complete words that begin with the given prefix

    vector<string> autocomplete(string prefix) // fares
    {
        vector<string> suggestions;
        // TODO: Implement this function
        string lowerPrefix = toLowercase(prefix);
        TrieNode *currentnode = root;
        for (char c : lowerPrefix)
        {
            int index = (int)c; // subtracting letter from ascii of a gives integer index
            if (currentnode->children[index] == nullptr)
            {
                return suggestions; // an empty vector
            }
            else
            {
                currentnode = currentnode->children[index];
            }
        }
        findAllWords(currentnode, lowerPrefix, suggestions); // calling find all words to complete the prefix and insert each found word with isendofword flag at its end in result vector
        return suggestions;
    }
    void remove(string word)
    {
        removeword(root, word, 0);
    }

    string longest() const
    {
        string path, best;
        dfsLongest(root, path, best);
        return best;
    }

    int count()
    {
        return countWords(root);
    }

    // ...existing code...
};

// Main function
// Input: none
// Output: integer return code
// Purpose: Program entry point, run tests and interactive demo
int main()
{
    cout << "=== TRIE DATA STRUCTURE IMPLEMENTATION ===" << endl;
    cout << "Testing all Trie functionalities..." << endl;

    Trie trie;

    // Test 1: Basic insertion and search
    cout << "\n1. Testing basic insertion and search:" << endl;
    cout << "======================================" << endl;

    vector<string> words = {"apple", "banana", "orange", "grape", "kiwi"};
    for (const string &word : words)
    {
        trie.insert(word);
        cout << "Inserted: " << word << endl;
    }

    // Test search for existing words
    for (const string &word : words)
    {
        bool found = trie.search(word);
        cout << "Search '" << word << "': " << (found ? "FOUND" : "NOT FOUND") << endl;
    }

    // Test search for non-existing words
    vector<string> nonExisting = {"app", "ban", "ora", "graph", "kiwis"};
    for (const string &word : nonExisting)
    {
        bool found = trie.search(word);
        cout << "Search '" << word << "': " << (found ? "FOUND" : "NOT FOUND") << " (expected: NOT FOUND)" << endl;
    }

    // Test 2: Prefix checking
    cout << "\n2. Testing prefix checking:" << endl;
    cout << "==========================" << endl;

    vector<string> prefixes = {"app", "ban", "ora", "grap", "k"};
    for (const string &prefix : prefixes)
    {
        bool hasPrefix = trie.startsWith(prefix);
        cout << "Prefix '" << prefix << "': " << (hasPrefix ? "EXISTS" : "DOESN'T EXIST") << endl;
    }

    vector<string> nonPrefixes = {"x", "yield", "zed", "micro", "nano"};
    for (const string &prefix : nonPrefixes)
    {
        bool hasPrefix = trie.startsWith(prefix);
        cout << "Prefix '" << prefix << "': " << (hasPrefix ? "EXISTS" : "DOESN'T EXIST") << " (expected: DOESN'T EXIST)" << endl;
    }

    // Test 3: Autocomplete functionality
    cout << "\n3. Testing autocomplete functionality:" << endl;
    cout << "======================================" << endl;

    vector<string> testPrefixes = {"a", "b", "o", "g", "k", "ap", "ban", "ora", "gr", "ki"};
    for (const string &prefix : testPrefixes)
    {
        vector<string> suggestions = trie.autocomplete(prefix);
        cout << "Autocomplete for '" << prefix << "': ";
        if (suggestions.empty())
        {
            cout << "No suggestions found";
        }
        else
        {
            for (size_t i = 0; i < suggestions.size(); i++)
            {
                if (i > 0)
                    cout << ", ";
                cout << suggestions[i];
            }
        }
        cout << endl;
    }

    // Test 4: Edge cases
    cout << "\n4. Testing edge cases:" << endl;
    cout << "======================" << endl;

    // Empty string tests
    bool emptySearch = trie.search("");
    cout << "Search empty string: " << (emptySearch ? "FOUND" : "NOT FOUND") << " (expected: NOT FOUND)" << endl;

    bool emptyPrefix = trie.startsWith("");
    cout << "Starts with empty prefix: " << (emptyPrefix ? "EXISTS" : "DOESN'T EXIST") << " (expected: EXISTS)" << endl;

    vector<string> emptySuggestions = trie.autocomplete("");
    cout << "Autocomplete for empty string: ";
    if (emptySuggestions.empty())
    {
        cout << "No suggestions found";
    }
    else
    {
        for (size_t i = 0; i < emptySuggestions.size(); i++)
        {
            if (i > 0)
                cout << ", ";
            cout << emptySuggestions[i];
        }
    }
    cout << " (expected: all words)" << endl;

    // Test 5: Adding more words and retesting
    cout << "\n5. Testing with additional words:" << endl;
    cout << "================================" << endl;

    vector<string> additionalWords = {"application", "appetizer", "banister", "bandana", "oracle", "grapefruit"};
    for (const string &word : additionalWords)
    {
        trie.insert(word);
        cout << "Inserted: " << word << endl;
    }

    // Test search for new words
    for (const string &word : additionalWords)
    {
        bool found = trie.search(word);
        cout << "Search '" << word << "': " << (found ? "FOUND" : "NOT FOUND") << endl;
    }

    // Test autocomplete with new words
    vector<string> newPrefixes = {"app", "ban", "ora", "gra"};
    for (const string &prefix : newPrefixes)
    {
        vector<string> suggestions = trie.autocomplete(prefix);
        cout << "Autocomplete for '" << prefix << "': ";
        if (suggestions.empty())
        {
            cout << "No suggestions found";
        }
        else
        {
            for (size_t i = 0; i < suggestions.size(); i++)
            {
                if (i > 0)
                    cout << ", ";
                cout << suggestions[i];
            }
        }
        cout << endl;
    }

    // Test 6: Case sensitivity (all lowercase expected)
    cout << "\n6. Testing case sensitivity:" << endl;
    cout << "============================" << endl;

    trie.insert("Hello");
    trie.insert("WORLD");

    vector<string> caseWords = {"hello", "Hello", "WORLD", "world"};
    for (const string &word : caseWords)
    {
        bool found = trie.search(word);
        cout << "Search '" << word << "': " << (found ? "FOUND" : "NOT FOUND") << endl;
    }

    // Test 7: Remove word functionality
    cout << "\n7. Testing remove word functionality:" << endl;
    cout << "===============================" << endl;

    // Insert words for removal test
    vector<string> removeWords = {"test", "testing", "tester", "tested", "toast"};
    for (const string &word : removeWords)
    {
        trie.insert(word);
        cout << "Inserted: " << word << endl;
    }

    // Remove some words
    vector<string> toRemove = {"testing", "tester", "toast"};
    for (const string &word : toRemove)
    {
        trie.remove(word);
        cout << "Removed: " << word << endl;
    }

    // Check removed words
    for (const string &word : toRemove)
    {
        bool found = trie.search(word);
        cout << "Search after remove '" << word << "': " << (found ? "FOUND" : "NOT FOUND") << " (expected: NOT FOUND)" << endl;
    }

    // Check words that should remain
    vector<string> shouldRemain = {"test", "tested"};
    for (const string &word : shouldRemain)
    {
        bool found = trie.search(word);
        cout << "Search after remove (should remain) '" << word << "': " << (found ? "FOUND" : "NOT FOUND") << " (expected: FOUND)" << endl;
    }

    // Remove a word that is a prefix of another
    trie.remove("test");
    cout << "Removed: test" << endl;
    bool foundTest = trie.search("test");
    bool foundTested = trie.search("tested");
    cout << "Search after remove 'test': " << (foundTest ? "FOUND" : "NOT FOUND") << " (expected: NOT FOUND)" << endl;
    cout << "Search after remove 'tested': " << (foundTested ? "FOUND" : "NOT FOUND") << " (expected: FOUND)" << endl;

    // Remove last remaining word
    trie.remove("tested");
    cout << "Removed: tested" << endl;
    bool foundTested2 = trie.search("tested");
    cout << "Search after remove 'tested': " << (foundTested2 ? "FOUND" : "NOT FOUND") << " (expected: NOT FOUND)" << endl;

    // Test longest function
    cout << "\nTesting longest function after removals:" << endl;
    cout << "Longest word in Trie: " << trie.longest() << endl;




    // Test 8: Count functionality
    cout << "\n8. Testing count functionality:" << endl;
    cout << "==============================" << endl;

    // Create a fresh trie for count testing
    Trie countTrie;
    cout << "Initial count (empty trie): " << countTrie.count() << " (expected: 0)" << endl;

    // Add some words and test count
    vector<string> countWords = {"cat", "cats", "dog", "dogs", "rat", "rats"};
    int expectedCount = 0;
    for (const string &word : countWords)
    {
        countTrie.insert(word);
        expectedCount++;
        cout << "After inserting '" << word << "', count: " << countTrie.count() << " (expected: " << expectedCount << ")" << endl;
    }

    // Test inserting duplicate words (should not increase count)
    cout << "\nTesting duplicate insertions:" << endl;
    int currentCount = countTrie.count();
    countTrie.insert("cat"); // duplicate
    countTrie.insert("dog"); // duplicate
    cout << "After inserting duplicates 'cat' and 'dog', count: " << countTrie.count() << " (expected: " << currentCount << ")" << endl;

    // Test removing words and checking count
    cout << "\nTesting count after removals:" << endl;
    countTrie.remove("cats");
    expectedCount--;
    cout << "After removing 'cats', count: " << countTrie.count() << " (expected: " << expectedCount << ")" << endl;

    countTrie.remove("dogs");
    expectedCount--;
    cout << "After removing 'dogs', count: " << countTrie.count() << " (expected: " << expectedCount << ")" << endl;

    // Test removing non-existent word (should not change count)
    currentCount = countTrie.count();
    countTrie.remove("elephant"); // doesn't exist
    cout << "After trying to remove non-existent 'elephant', count: " << countTrie.count() << " (expected: " << currentCount << ")" << endl;

    // Test count with complex prefix relationships
    cout << "\nTesting count with complex prefix relationships:" << endl;
    Trie complexTrie;
    vector<string> complexWords = {"a", "ab", "abc", "abcd", "abcde"};
    expectedCount = 0;
    for (const string &word : complexWords)
    {
        complexTrie.insert(word);
        expectedCount++;
        cout << "After inserting '" << word << "', count: " << complexTrie.count() << " (expected: " << expectedCount << ")" << endl;
    }

    // Remove middle word and test count
    complexTrie.remove("abc");
    expectedCount--;
    cout << "After removing 'abc', count: " << complexTrie.count() << " (expected: " << expectedCount << ")" << endl;

    // Verify other words still exist
    vector<string> shouldExist = {"a", "ab", "abcd", "abcde"};
    for (const string &word : shouldExist)
    {
        bool found = complexTrie.search(word);
        cout << "Verify '" << word << "' still exists: " << (found ? "YES" : "NO") << " (expected: YES)" << endl;
    }


    cout << "\n=== ALL TESTS COMPLETED ===" << endl;

    return 0;
}
