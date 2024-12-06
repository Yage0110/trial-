#include "FileAVL.hpp"
#include "File.hpp"
#include "FileTrie.hpp"

// ALL YOUR CODE SHOULD BE IN THIS FILE. NO MODIFICATIONS SHOULD BE MADE TO FILEAVL / FILE CLASSES
// You are permitted to make helper functions (and most likely will need to)
// You must declare them "inline" | declare & implement them at the top of this file, before query()
// Below query(), implement and document all methods declared in FileTrie.hpp


FileTrie::FileTrie() : head(new FileTrieNode{' ', {}}) {}


//Search helper fxn
inline void collection(FileTrieNode* node, std::unordered_set<File*>& result){
    for (auto& [key, child] : node->next) {
        result.insert(child->matching.begin(), child->matching.end());
        collection(child, result);
    }
}
/**
 * @brief Retrieves all files in the FileAVL whose file sizes are within [min, max]
 * 
 * @param min The min value of the file size query range.
 * @param max The max value of the file size query range.
 * @return std::vector<File*> storing pointers to all files in the tree within the given range.
 * @note If the query interval is in descending order (ie. the given parameters min >= max), 
        the interval from [max, min] is searched (since max >= min)
 */
std::vector<File*> FileAVL::query(size_t min, size_t max) {
    std::vector<File*> result;
    if (!root_) return result;
    if(min > max) std::swap(min,max);
    std::stack<Node*> queries;
    Node* curr = root_;
    while(!queries.empty() || curr){
        while(curr){
            queries.push(curr);
            curr = curr -> left_;
        }

        curr = queries.top();
        queries.pop();

        auto nodeS = curr ->files_.front() -> getSize();
        if(nodeS >= min && nodeS <= max){
            result.insert(result.end(), curr ->files_.begin(), curr -> files_.end());
        }
        if(nodeS > max) break;
        curr = curr -> right_;
    }

    return result;
}


/**
 * @brief Add file to the FileTrie
 * 
 * Converts  filename to lowercase and traverses the trie for each character in the filename. At the final node corresponding to the filename, the file pointer is added to the `matching` set of that node.
 * 
 * @param f Points to the File object to be added to the trie.
 */
void FileTrie::addFile(File* f){
    std::string names = f -> getName();
    FileTrieNode* curr = head;
    head -> matching.insert(f);
    for (char n : names){
        char lowerC = std::tolower(n);
        if(!curr ->next[lowerC]){
            curr -> next[lowerC] = new FileTrieNode{lowerC};
        }
        curr = curr -> next[lowerC];
    }
    curr -> matching.insert(f);
}

/**
 * @brief Retrieve all files in the trie whose names start with a given prefix.
 * 
 * Converts the input prefix to lowercase, then 
 * traverses the trie following the characters of the prefix. If the prefix exists, all files 
 * in `matching` are returned.
 * 
 * @param prefix Prefix to search for
 * @return A set of pointers to File objects whose names start with a given prefix.
 */
std::unordered_set<File*> FileTrie::getFilesWithPrefix(const std::string& prefix) const{
    FileTrieNode* curr = head;
    std::unordered_set<File*> result;
    if (prefix.empty()) {
        result.insert(head->matching.begin(), head->matching.end());
        return result;
    }

    std::string lowerPre;
    for(char c : prefix){
        lowerPre += std::tolower(c);
    }
    for(char c: lowerPre){
        if(!curr ->next[c]){
            return result;
        }
        curr = curr -> next[c];
    }
    result.insert(curr -> matching.begin(),curr -> matching.end());

    return result;
}


/**
 * @brief Destructor for the FileTrie.
 * 
 *  It uses a vector to manage nodes iteratively, traversing the trie and deleting each node.
 */
FileTrie::~FileTrie(){
    std::vector<FileTrieNode*> deletions;
    deletions.push_back(head);
    while(!deletions.empty()){
        FileTrieNode* node = deletions.back();
        deletions.pop_back();

        for(auto& [key,child] : node -> next){
            deletions.push_back(child);
        }
        delete node;
    }
}
