#include <bits/stdc++.h>
using namespace std;

//////////////////////////////////////////////////////
// TRIE for Autocomplete
//////////////////////////////////////////////////////

struct TrieNode {
    bool isEnd;
    map<char, TrieNode*> children; // map instead of unordered_map (easier to read)
    TrieNode() : isEnd(false) {}
};

class Trie {
    TrieNode* root;

    // helper to collect words for suggestions
    void collect(TrieNode* node, string prefix, vector<string>& result) {
        if (node->isEnd) result.push_back(prefix);
        for (auto& child : node->children) {
            collect(child.second, prefix + child.first, result);
        }
    }

public:
    Trie() { root = new TrieNode(); }

    void insert(string word) {
        TrieNode* curr = root;
        for (char c : word) {
            if (!curr->children[c]) curr->children[c] = new TrieNode();
            curr = curr->children[c];
        }
        curr->isEnd = true;
    }

    vector<string> getSuggestions(string prefix) {
        TrieNode* curr = root;
        for (char c : prefix) {
            if (!curr->children[c]) return {}; // prefix not found
            curr = curr->children[c];
        }
        vector<string> result;
        collect(curr, prefix, result);
        return result;
    }
};

//////////////////////////////////////////////////////
// MINI SEARCH ENGINE
//////////////////////////////////////////////////////

class MiniSearchEngine {
    Trie trie; 
    // word -> (docName -> frequency)
    unordered_map<string, unordered_map<string, int>> index;  

    // clean words: lowercase, remove punctuation
    string normalize(string word) {
        string clean = "";
        for (char c : word) {
            if (isalnum(c)) clean += tolower(c);
        }
        return clean;
    }

public:
    // add document content into index + trie
    void addDocument(string docName, string text) {
        string word;
        stringstream ss(text);
        while (ss >> word) {
            word = normalize(word);
            if (word.empty()) continue;
            index[word][docName]++;
            trie.insert(word);
        }
    }

    // search a single word
    void search(string query) {
    query = normalize(query);
    if (index.find(query) == index.end()) {
        cout << "No results found for \"" << query << "\"\n";
        return;
    }

    cout << "Results for \"" << query << "\":\n";
    // rank by frequency
    vector<pair<string,int>> results;
    for (auto& p : index[query]) results.push_back({p.first, p.second});
    sort(results.begin(), results.end(),
         [](const pair<string,int>& a, const pair<string,int>& b) {
             return a.second > b.second;
         });

    for (auto &p : results) {
        cout << " - " << p.first << " (frequency: " << p.second << ")\n";
    }
}


    // autocomplete
    void autocomplete(string prefix) {
        prefix = normalize(prefix);
        auto suggestions = trie.getSuggestions(prefix);
        if (suggestions.empty()) {
            cout << "No suggestions for \"" << prefix << "\"\n";
            return;
        }
        cout << "Suggestions: ";
        for (string s : suggestions) cout << s << " ";
        cout << "\n";
    }
};

//////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////

int main() {
    MiniSearchEngine engine;

    // Sample "documents"
    engine.addDocument("doc1.txt", "C++ programming with DSA and algorithms");
    engine.addDocument("doc2.txt", "Data Structures and Algorithms are essential for programming");
    engine.addDocument("doc3.txt", "Programming in C++ is fun and powerful");

    cout << "=== Mini Search Engine ===\n";
    while (true) {
        cout << "\n1. Search\n2. Autocomplete\n3. Exit\n> ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string query;
            cout << "Enter word: ";
            getline(cin, query);
            engine.search(query);
        }
        else if (choice == 2) {
            string prefix;
            cout << "Enter prefix: ";
            getline(cin, prefix);
            engine.autocomplete(prefix);
        }
        else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
