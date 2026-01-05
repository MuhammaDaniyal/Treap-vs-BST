#ifndef TREAP_H
#define TREAP_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <unistd.h>
#include <zstd.h>

using namespace std;

class Treap {
private:
    struct TreapNode {
        string postId;       // Unique identifier for the post
        long long timestamp; // Key for BST ordering
        int score;           // Priority for max-heap ordering
        int priority;        // Random priority for treap randomization
        TreapNode* left;
        TreapNode* right;
        
        TreapNode(string id, long long ts, int sc) 
        : postId(id), timestamp(ts), score(sc), priority(rand()), left(nullptr), right(nullptr) {}
    };

    TreapNode* root;
    long long nodeCount;
    long long rotationCount;

    int calculateMinHeightHelper(TreapNode* node) {
        if (!node) return 0;
        return 1 + min(calculateMinHeightHelper(node->left), calculateMinHeightHelper(node->right));
    }

    ///////////////////////////////////////////////////////
    ///////////////////// Rotations ///////////////////////
    ///////////////////////////////////////////////////////

    // Left rotation to maintain heap property
    TreapNode* leftRotate(TreapNode* node) {
        if (!node || !node->right) return node;
        
        rotationCount++;
        TreapNode* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        
        return newRoot;
    }

    // Right rotation to maintain heap property
    TreapNode* rightRotate(TreapNode* node) {
        if (!node || !node->left) return node;
        
        rotationCount++;
        TreapNode* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        
        return newRoot;
    }

    ///////////////////////////////////////////////////////
    /////////////////////// Insert ////////////////////////
    ///////////////////////////////////////////////////////

    // Helper function to insert a post while maintaining both BST and heap properties
    TreapNode* insert(TreapNode* node, const string& postId, long long timestamp, int score) {
        if (!node) {
            return new TreapNode(postId, timestamp, score);
        }
        
        // BST ordering by timestamp
        if (timestamp < node->timestamp) {
            node->left = insert(node->left, postId, timestamp, score);
            
            // Maintain max-heap property: rotate if child's score > parent's score
            if (node->left && node->left->score > node->score) {
                node = rightRotate(node);
            }
        } else if (timestamp > node->timestamp) {
            node->right = insert(node->right, postId, timestamp, score);
            
            // Maintain max-heap property: rotate if child's score > parent's score
            if (node->right && node->right->score > node->score) {
                node = leftRotate(node);
            }
        } else {
            // Handle duplicate timestamps - insert in right subtree
            node->right = insert(node->right, postId, timestamp, score);
            
            // Maintain max-heap property
            if (node->right && node->right->score > node->score) {
                node = leftRotate(node);
            }
        }
        
        return node;
    }

    ///////////////////////////////////////////////////////
    /////////////////////// Search ////////////////////////
    ///////////////////////////////////////////////////////

    // Search for a post by ID
    TreapNode* searchById(TreapNode* node, const string& postId) {
        if (!node) return nullptr;
        
        if (node->postId == postId) {
            return node;
        }
        
        TreapNode* leftResult = searchById(node->left, postId);
        if (leftResult) return leftResult;
        
        return searchById(node->right, postId);
    }

    ///////////////////////////////////////////////////////
    ///////////////////// Re-heapify //////////////////////
    ///////////////////////////////////////////////////////

    // Find node by ID and perform rotations if needed after score update
    TreapNode* updateAndReheapify(TreapNode* node, const string& postId) {
        if (!node) return nullptr;
        
        if (node->postId == postId) {
            // Found the node - now reheapify upward
            return node;
        }
        
        if (node->timestamp > 0) {
            // Try left subtree
            TreapNode* leftResult = updateAndReheapify(node->left, postId);
            if (leftResult && leftResult->postId == postId) {
                node->left = leftResult;
                // Check if rotation needed
                if (node->left && node->left->score > node->score) {
                    node = rightRotate(node);
                }
                return leftResult;
            }
        }
        
        // Try right subtree
        TreapNode* rightResult = updateAndReheapify(node->right, postId);
        if (rightResult && rightResult->postId == postId) {
            node->right = rightResult;
            // Check if rotation needed
            if (node->right && node->right->score > node->score) {
                node = leftRotate(node);
            }
            return rightResult;
        }
        
        return nullptr;
    }
    
    ///////////////////////////////////////////////////////
    ///////////////////// Deletion ////////////////////////
    ///////////////////////////////////////////////////////
    
    // Delete a post by ID
    TreapNode* deleteById(TreapNode* node, const string& postId) {
        if (!node) return nullptr;
        
        if (node->postId == postId) {
            // Found the node to delete
            if (!node->left) {
                TreapNode* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                TreapNode* temp = node->left;
                delete node;
                return temp;
            }
            
            // Node with two children: rotate the child with lower score to leaf
            // Then delete the leaf
            if (node->left->score > node->right->score) {
                node = rightRotate(node);
                node->right = deleteById(node->right, postId);
            } else {
                node = leftRotate(node);
                node->left = deleteById(node->left, postId);
            }
        } else {
            // Search in both subtrees
            node->left = deleteById(node->left, postId);
            node->right = deleteById(node->right, postId);
        }
        
        return node;
    }

 
    ///////////////////////////////////////////////////////
    ////////////////////// Utilities //////////////////////
    ///////////////////////////////////////////////////////

    // Find minimum node (used in deletion)
    TreapNode* findMin(TreapNode* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    // Reverse inorder traversal to get most recent posts
    void reverseInorder(TreapNode* node, int k, vector<string>& result) {
        if (!node || (int)result.size() >= k) return;
        
        // Right, Root, Left (for descending order of timestamps)
        reverseInorder(node->right, k, result);
        if ((int)result.size() < k) {
            result.push_back(node->postId + " (TS: " + to_string(node->timestamp) + 
                           ", Score: " + to_string(node->score) + ")");
        }
        reverseInorder(node->left, k, result);
    }

    // Calculate height of the tree
    int getHeightHelper(TreapNode* node) {
        if (!node) return 0;
        return 1 + max(getHeightHelper(node->left), getHeightHelper(node->right));
    }

    // Inorder traversal for debugging
    void inorderHelper(TreapNode* node) {
        if (node) {
            inorderHelper(node->left);
            cout << "[" << node->postId << ": T=" << node->timestamp 
                 << ", S=" << node->score << "] ";
            inorderHelper(node->right);
        }
    }

    // Clear the treap
    void clear(TreapNode* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    ////////////////////////////////////////////////
    /////////// Vertical Structure Print ////////////
    ///////////////////////////////////////////////

    void printTreapStructure_helper(TreapNode* node, const string& prefix, bool isTail, const string& branchLabel) {
        if (node == nullptr) {
            return;
        }

        // Determine the connector string for the current node
        cout << prefix;
        if (isTail) {
            // L-shaped corner: '└── '
            cout << "\u2514\u2500\u2500 "; 
        } else {
            // T-shaped connector: '├── '
            cout << "\u251C\u2500\u2500 "; 
        }

        // Print the node's data: Timestamp (BST Key), Priority (Heap Key), and Branch Label
        cout << "TS: " << node->timestamp << branchLabel 
            << " | Prio: " << node->priority 
            << " | ID: " << node->postId 
            << " | Score: " << node->score << endl;

        // The new prefix for the children
        string newPrefix = prefix + (isTail ? "    " : "\u2502   "); // '|   ' or '    '

        // Recursive calls for children
        
        // 1. Process the Left child
        // Left child is the 'tail' only if the right child is null.
        if (node->left != nullptr) {
            printTreapStructure_helper(node->left, newPrefix, node->right == nullptr, " (L)");
        }
        
        // 2. Process the Right child
        // Right child is ALWAYS the tail.
        if (node->right != nullptr) {
            printTreapStructure_helper(node->right, newPrefix, true, " (R)");
        }
    }

    void printTreapStructure(TreapNode* root) {
        if (root == nullptr) {
            cout << "The treap is empty." << endl;
            return;
        }
        cout << "--- Treap Structure (Vertical View) ---" << endl;
        
        // Print the root node
        cout << "ROOT: TS: " << root->timestamp 
            << " | Prio: " << root->priority 
            << " | ID: " << root->postId 
            << " | Score: " << root->score << endl;
        
        string prefix = ""; 
        
        // 1. Left child
        if (root->left != nullptr) {
            printTreapStructure_helper(root->left, prefix, root->right == nullptr, " (L)");
        }
        // 2. Right child
        if (root->right != nullptr) {
            printTreapStructure_helper(root->right, prefix, true, " (R)");
        }
        cout << "---------------------------------------" << endl;
    }


public:
    Treap() : root(nullptr), nodeCount(0), rotationCount(0) {
        srand(time(0));
    }
    
    void printPostById(const string& postId)
    {
        cout << "[TREAP] ";
        TreapNode* node = searchById(root, postId);
        if (node) {
            cout << "Post Found: [" << node->postId << ": T=" << node->timestamp 
                 << ", S=" << node->score << "]" << endl;
        } else {
            cout << "Post ID " << postId << " not found in Treap." << endl;
        }
    }


    int calculateMinHeight() {
        return calculateMinHeightHelper(root);
    }

    // Add a post to the treap
    void addPost(const string& postId, long long timestamp, int score) {
        root = insert(root, postId, timestamp, score);
        nodeCount++;
    }
    
    // Delete a post from the treap
    void deletePost(const string& postId) {
        root = deleteById(root, postId);
    }
    
    // Increment score and reheapify
    void likePost(const string& postId) {
        TreapNode* node = searchById(root, postId);
        if (node) {
            node->score++;
            // Reheapify: bubble up if needed
            root = reheapifyUp(root, postId);
        }
    }

    // Get number of rotations performed
    long long getRotationCount() const {
        return rotationCount;
    }

    // reset rotation count
    void resetRotationCount() {
        rotationCount = 0;
    }

    // Reheapify after score update - bubble up the node
    TreapNode* reheapifyUp(TreapNode* node, const string& postId) {
        if (!node) return nullptr;
        
        if (node->postId == postId) {
            return node;
        }
        
        // Recursively fix left subtree
        if (node->left) {
            node->left = reheapifyUp(node->left, postId);
            if (node->left->score > node->score) {
                node = rightRotate(node);
            }
        }
        
        // Recursively fix right subtree
        if (node->right) {
            node->right = reheapifyUp(node->right, postId);
            if (node->right->score > node->score) {
                node = leftRotate(node);
            }
        }
        
        return node;
    }
    
    // Get the most popular post (highest score) - O(1)
    string getMostPopular() {
        if (root) {
            return root->postId + " (Score: " + to_string(root->score) + 
                   ", Timestamp: " + to_string(root->timestamp) + ")";
        }
        return "No posts found";
    }
    
    // Get k most recent posts
    vector<string> getMostRecent(int k) {
        vector<string> result;
        reverseInorder(root, k, result);
        return result;
    }
    
    // Get tree height
    int getHeight() {
        return getHeightHelper(root);
    }
    
    // get number of nodes
    long long getNodeCount() const {
        return nodeCount;
    }
    

    // Destructor
    ~Treap() {
        clear(root);
    }
    
    ///////////////////////////////////////////////////////
    ///////////////////// Data Loading ////////////////////
    ///////////////////////////////////////////////////////

    // Load posts from JSON file
    void loadFromJSON(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Unable to open file: " << filename << endl;
            return;
        }
        
        string line;
        
        // Skip first line "["
        getline(file, line);
        
        while (getline(file, line)) {
            if (line == "]") break;
            
            // Extract data
            size_t id_start = line.find("\"id\": \"") + 7;
            size_t id_end = line.find("\"", id_start);
            string id = line.substr(id_start, id_end - id_start);
            
            size_t ts_start = line.find("\"created_utc\": ") + 14;
            size_t ts_end = line.find(",", ts_start);
            long long timestamp = stoll(line.substr(ts_start, ts_end - ts_start));
            
            size_t score_start = line.find("\"score\": ") + 9;
            size_t score_end = line.find("}");
            int score = stoi(line.substr(score_start, score_end - score_start));
            
            addPost(id, timestamp, score);
        }
        
        file.close();
    }

    // Load posts from ZST compressed JSON file
    // Decompresses on-the-fly using Zstandard (zstd) library
    // Does NOT create temporary decompressed file on disk
    

    double loadFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "[Treap] Unable to open CSV file: " << filename << std::endl;
            return 0.0;
        }
        
        std::string line;
        long long lineCount = 0;
        
        std::cout << "[Treap] Starting CSV load from: " << filename << std::endl;
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // Skip header line if exists
        std::getline(file, line);
        
        while (std::getline(file, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);
            
            if (pos1 == std::string::npos || pos2 == std::string::npos) {
                continue; // Skip invalid lines
            }
            
            std::string id = line.substr(0, pos1);
            long long timestamp = std::stoll(line.substr(pos1 + 1, pos2 - pos1 - 1));
            int score = std::stoi(line.substr(pos2 + 1));
            
            addPost(id, timestamp, score);
            lineCount++;
            
            // Progress update every 100,000 posts
            if (lineCount % 100 == 0) {
                break;
                auto currentTime = std::chrono::high_resolution_clock::now();
                double elapsedSec = std::chrono::duration<double>(currentTime - startTime).count();
                double rate = lineCount / elapsedSec;
                std::cout << "\r[Treap] Posts: " << lineCount << " | Time: " << std::fixed << std::setprecision(2) 
                        << elapsedSec << "s | Rate: " << std::fixed << std::setprecision(0) << rate << " posts/s" 
                        << std::string(20, ' ') << std::flush;
            }
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        double totalTime = std::chrono::duration<double>(endTime - startTime).count();
        
        std::cout << "\n[Treap] CSV LOAD COMPLETE" << std::endl;
        std::cout << "[Treap] Posts: " << lineCount << " | Time: " << std::fixed << std::setprecision(3) << totalTime 
                  << "s | Height: " << getHeight() << std::endl;
        
        file.close();
        return totalTime;
    }


    double loadFromTGZ(const std::string& tgzFilename) {
        std::cout << "[Treap] Loading from compressed dataset: " << tgzFilename << std::endl;
        
        // Use streaming approach - create a pipe that decompresses on demand
        std::string cmd = "zstd -dc '" + tgzFilename + "' 2>/dev/null";
        FILE* f = popen(cmd.c_str(), "r");
        if (!f) {
            std::cerr << "[Treap] Failed to open pipe" << std::endl;
            return 0.0;
        }
        
        char buffer[65536];
        long long lineCount = 0;
        auto startTime = std::chrono::high_resolution_clock::now();
        
        std::cout << "[Treap] Starting streaming load..." << std::endl;
        
        std::string fullLine;
        while (fgets(buffer, sizeof(buffer), f) != nullptr) {
            fullLine += buffer;
            
            // Check if we have a complete JSON object (ends with })
            size_t pos = fullLine.rfind("}");
            if (pos != std::string::npos && (pos + 1 >= fullLine.length() || fullLine[pos + 1] == '\n')) {
                std::string line = fullLine.substr(0, pos + 1);
                fullLine = (pos + 1 < fullLine.length()) ? fullLine.substr(pos + 1) : "";
                
                if (line.empty()) continue;
                
                // Parse JSON
                std::string id, timestamp_str, score_str;
                
                size_t id_pos = line.find("\"id\":\"");
                if (id_pos != std::string::npos) {
                    size_t id_start = id_pos + 6;
                    size_t id_end = line.find("\"", id_start);
                    if (id_end != std::string::npos) {
                        id = line.substr(id_start, id_end - id_start);
                    }
                }
                
                size_t ts_pos = line.find("\"created_utc\":");
                if (ts_pos != std::string::npos) {
                    size_t ts_start = ts_pos + 14;
                    size_t ts_end = line.find(",", ts_start);
                    if (ts_end != std::string::npos) {
                        timestamp_str = line.substr(ts_start, ts_end - ts_start);
                    }
                }
                
                size_t score_pos = line.find("\"score\":");
                if (score_pos != std::string::npos) {
                    size_t score_start = score_pos + 8;
                    size_t score_end = line.find(",", score_start);
                    if (score_end == std::string::npos) {
                        score_end = line.find("}", score_start);
                    }
                    if (score_end != std::string::npos) {
                        score_str = line.substr(score_start, score_end - score_start);
                    }
                }
                
                if (!id.empty() && !timestamp_str.empty() && !score_str.empty()) {
                    try {
                        long long timestamp = std::stoll(timestamp_str);
                        int score = std::stoi(score_str);
                        
                        addPost(id, timestamp, score);
                        lineCount++;
                        
                        if (lineCount % 10000 == 0) {
                            auto currentTime = std::chrono::high_resolution_clock::now();
                            double elapsedSec = std::chrono::duration<double>(currentTime - startTime).count();
                            double rate = lineCount / elapsedSec;
                            std::cout << "\r[Treap] Posts: " << lineCount << " | Time: " << std::fixed << std::setprecision(2) 
                                      << elapsedSec << "s | Rate: " << std::fixed << std::setprecision(0) << rate << " posts/s" 
                                      << std::flush;
                        }
                    } catch (...) {
                        continue;
                    }
                }
            }
        }
        
        pclose(f);
        
        auto endTime = std::chrono::high_resolution_clock::now();
        double totalTime = std::chrono::duration<double>(endTime - startTime).count();
        
        std::cout << "\n[Treap] LOAD COMPLETE" << std::endl;
        std::cout << "[Treap] Posts Loaded: " << lineCount << " | Time: " << std::fixed << std::setprecision(3) 
                  << totalTime << "s | Tree Height: " << getHeight() << std::endl;
        return totalTime;
    }


    void loadFromCSVWithTimeout(const std::string& filename, int timeoutSeconds = 30) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "[Treap] Unable to open CSV file: " << filename << std::endl;
            return;
        }

        std::string line;
        long long lineCount = 0;

        std::cout << "[Treap] Starting CSV load from: " << filename << std::endl;
        std::cout << "[Treap] Timeout: " << timeoutSeconds << " seconds" << std::endl;

        auto startTime = std::chrono::high_resolution_clock::now();

        // Skip header line if exists
        std::getline(file, line);

        while (std::getline(file, line)) {
            // Check timeout
            auto currentTime = std::chrono::high_resolution_clock::now();
            double elapsedSec = std::chrono::duration<double>(currentTime - startTime).count();
            if (elapsedSec >= timeoutSeconds) {
                std::cout << "\n[Treap] TIMEOUT after " << timeoutSeconds << " seconds" << std::endl;
                break;
            }
            
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);
            
            if (pos1 == std::string::npos || pos2 == std::string::npos) {
                continue;
            }
            
            std::string id = line.substr(0, pos1);
            long long timestamp = std::stoll(line.substr(pos1 + 1, pos2 - pos1 - 1));
            int score = std::stoi(line.substr(pos2 + 1));
            
            addPost(id, timestamp, score);
            lineCount++;
            
            if (lineCount % 100 == 0) {
                double rate = lineCount / elapsedSec;
                std::cout << "\r[Treap] Posts: " << lineCount << " | Time: " << std::fixed << std::setprecision(2) 
                        << elapsedSec << "s | Rate: " << std::fixed << std::setprecision(0) << rate << " posts/s" 
                        << std::string(20, ' ') << std::flush;
            }
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        double totalTime = std::chrono::duration<double>(endTime - startTime).count();

        std::cout << "\n --------- [Treap] CSV LOAD COMPLETE ----------- " << std::endl;
        std::cout << "[Treap] Posts: " << lineCount << " | Time: " << std::fixed << std::setprecision(3) << totalTime 
                    << "s | Height: " << getHeight() << std::endl << std::endl;

        file.close();
        }


    void loadFromTGZWithTimeout(const std::string& tgzFilename, int timeoutSeconds = 30) {
        // std::cout << "[Treap] Loading from compressed dataset: " << tgzFilename << std::endl;
        // std::cout << "[Treap] Timeout: " << timeoutSeconds << " seconds" << std::endl;
        
        std::string cmd = "zstd -dc '" + tgzFilename + "' 2>/dev/null";
        FILE* f = popen(cmd.c_str(), "r");
        if (!f) {
            std::cerr << "[Treap] Failed to open pipe" << std::endl;
            return;
        }
        
        char buffer[65536];
        long long lineCount = 0;
        auto startTime = std::chrono::high_resolution_clock::now();
        
        std::cout << "[Treap] Starting streaming load..." << std::endl;
        
        std::string fullLine;
        while (fgets(buffer, sizeof(buffer), f) != nullptr) {
            // Check timeout
            auto currentTime = std::chrono::high_resolution_clock::now();
            double elapsedSec = std::chrono::duration<double>(currentTime - startTime).count();
            if (elapsedSec >= timeoutSeconds) {
                std::cout << "\n[Treap] TIMEOUT after " << timeoutSeconds << " seconds" << std::endl;
                break;
            }
            
            fullLine += buffer;
            
            size_t pos = fullLine.rfind("}");
            if (pos != std::string::npos && (pos + 1 >= fullLine.length() || fullLine[pos + 1] == '\n')) {
                std::string line = fullLine.substr(0, pos + 1);
                fullLine = (pos + 1 < fullLine.length()) ? fullLine.substr(pos + 1) : "";
                
                if (line.empty()) continue;
                
                std::string id, timestamp_str, score_str;
                
                size_t id_pos = line.find("\"id\":\"");
                if (id_pos != std::string::npos) {
                    size_t id_start = id_pos + 6;
                    size_t id_end = line.find("\"", id_start);
                    if (id_end != std::string::npos) {
                        id = line.substr(id_start, id_end - id_start);
                    }
                }
                
                size_t ts_pos = line.find("\"created_utc\":");
                if (ts_pos != std::string::npos) {
                    size_t ts_start = ts_pos + 14;
                    size_t ts_end = line.find(",", ts_start);
                    if (ts_end != std::string::npos) {
                        timestamp_str = line.substr(ts_start, ts_end - ts_start);
                    }
                }
                
                size_t score_pos = line.find("\"score\":");
                if (score_pos != std::string::npos) {
                    size_t score_start = score_pos + 8;
                    size_t score_end = line.find(",", score_start);
                    if (score_end == std::string::npos) {
                        score_end = line.find("}", score_start);
                    }
                    if (score_end != std::string::npos) {
                        score_str = line.substr(score_start, score_end - score_start);
                    }
                }
                
                if (!id.empty() && !timestamp_str.empty() && !score_str.empty()) {
                    try {
                        long long timestamp = std::stoll(timestamp_str);
                        int score = std::stoi(score_str);
                        
                        addPost(id, timestamp, score);
                        lineCount++;
                        
                        if (lineCount % 100 == 0) {
                            double rate = lineCount / elapsedSec;
                            std::cout << "\r[Treap] Posts: " << lineCount << " | Time: " << std::fixed << std::setprecision(2) 
                                    << elapsedSec << "s | Rate: " << std::fixed << std::setprecision(0) << rate << " posts/s | "
                                    << "Remaining: " << (timeoutSeconds - (int)elapsedSec) << "s" << std::flush;
                        }
                    } catch (...) {
                        continue;
                    }
                }
            }
        }
        
        pclose(f);
        
        auto endTime = std::chrono::high_resolution_clock::now();
        double totalTime = std::chrono::duration<double>(endTime - startTime).count();
        
        std::cout << "\n --------- [Treap] LOAD COMPLETE ---------- " << std::endl;
        std::cout << "[Treap] Posts Loaded: " << lineCount << " | Time: " << std::fixed << std::setprecision(3) 
                << totalTime << "s | Tree Height: " << getHeight() << std::endl << std::endl;
    }


    void printTreapStructure() {
        printTreapStructure(root);
    }

    // Print tree inorder for debugging
    void printTreeInorder() {
        cout << "Treap Inorder: ";
        inorderHelper(root);
        cout << endl;
    }


};

#endif // TREAP_H