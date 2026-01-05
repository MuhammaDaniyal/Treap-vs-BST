#ifndef BST_H
#define BST_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stack>
#include <queue>
#include <chrono>
#include <iomanip>
#include <sys/resource.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <ctime>
#include <cstring>

using namespace std;

class BinarySearchTree {
private:
    struct PostNode {
        string postId;       // Unique identifier for the post
        long long timestamp; // Key for BST ordering
        int score;           // Popularity score
        PostNode* left;
        PostNode* right;
        
        PostNode(string id, long long ts, int sc) 
        : postId(id), timestamp(ts), score(sc), left(nullptr), right(nullptr) {}
    };

    PostNode* root;
    long long nodeCount;
    const long long MAX_NODES = 150000000LL; // 150 million safety limit

    int calculateMinHeightHelper(PostNode* node) {
        if (!node) return 0;
        return 1 + min(calculateMinHeightHelper(node->left), calculateMinHeightHelper(node->right));
    }

    // Get current memory usage in MB
    long long getMemoryUsageMB() {
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        return usage.ru_maxrss / 1024; // Convert KB to MB
    }

    // ITERATIVE insert - prevents stack overflow
    void insertIterative(const string& postId, long long timestamp, int score) {
        if (nodeCount >= MAX_NODES) {
            std::cerr << "ERROR: Node limit reached (" << MAX_NODES << " nodes)" << std::endl;
            return;
        }

        try {
            PostNode* newNode = new PostNode(postId, timestamp, score);
            
            if (!root) {
                root = newNode;
                nodeCount++;
                return;
            }
            
            PostNode* current = root;
            while (true) {
                if (timestamp < current->timestamp) {
                    if (!current->left) {
                        current->left = newNode;
                        nodeCount++;
                        break;
                    }
                    current = current->left;
                } else {
                    // timestamp >= current->timestamp, insert to right
                    if (!current->right) {
                        current->right = newNode;
                        nodeCount++;
                        break;
                    }
                    current = current->right;
                }
            }
        } catch (const std::bad_alloc& e) {
            std::cerr << "CRITICAL: Memory allocation failed! " << e.what() << std::endl;
            std::cerr << "Current memory usage: " << getMemoryUsageMB() << " MB" << std::endl;
            std::cerr << "Nodes loaded: " << nodeCount << std::endl;
            throw;
        }
    }

    // ITERATIVE search by ID
    PostNode* searchByIdIterative(const string& postId) {
        if (!root) return nullptr;
        
        queue<PostNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            PostNode* node = q.front();
            q.pop();
            
            if (node->postId == postId) {
                return node;
            }
            
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        
        return nullptr;
    }

    // ITERATIVE delete by ID
    void deleteByIdIterative(const string& postId) {
        if (!root) return;
        
        // BFS to find the node and its parent
        queue<pair<PostNode*, PostNode*>> q;
        q.push({root, nullptr});
        
        PostNode* targetNode = nullptr;
        PostNode* targetParent = nullptr;
        bool targetIsLeft = false;
        
        while (!q.empty() && !targetNode) {
            auto [node, par] = q.front();
            q.pop();
            
            if (node->postId == postId) {
                targetNode = node;
                targetParent = par;
                targetIsLeft = (par && par->left == node);
                break;
            }
            
            if (node->left) q.push({node->left, node});
            if (node->right) q.push({node->right, node});
        }
        
        if (!targetNode) return;
        
        // Case 1: No children
        if (!targetNode->left && !targetNode->right) {
            if (!targetParent) {
                delete root;
                root = nullptr;
            } else if (targetIsLeft) {
                targetParent->left = nullptr;
            } else {
                targetParent->right = nullptr;
            }
            delete targetNode;
            nodeCount--;
            return;
        }
        
        // Case 2: One child
        PostNode* child = targetNode->left ? targetNode->left : targetNode->right;
        if (!targetParent) {
            root = child;
        } else if (targetIsLeft) {
            targetParent->left = child;
        } else {
            targetParent->right = child;
        }
        delete targetNode;
        nodeCount--;
    }

    // ITERATIVE height calculation using BFS level counting
    int getHeightIterative() {
        if (!root) return 0;
        
        queue<pair<PostNode*, int>> q;
        q.push({root, 1});
        int maxHeight = 0;
        
        while (!q.empty()) {
            auto [node, height] = q.front();
            q.pop();
            
            maxHeight = max(maxHeight, height);
            
            if (node->left) q.push({node->left, height + 1});
            if (node->right) q.push({node->right, height + 1});
        }
        
        return maxHeight;
    }

    // ITERATIVE reverse inorder traversal using explicit stack
    void reverseInorderIterative(int k, vector<string>& result) {
        if (!root || k <= 0) return;
        
        stack<PostNode*> st;
        PostNode* current = root;
        
        // Find rightmost node
        while (current) {
            st.push(current);
            current = current->right;
        }
        
        while (!st.empty() && (int)result.size() < k) {
            current = st.top();
            st.pop();
            
            result.push_back(current->postId + " (TS: " + to_string(current->timestamp) + 
                           ", Score: " + to_string(current->score) + ")");
            
            // Traverse left subtree
            if (current->left) {
                current = current->left;
                while (current) {
                    st.push(current);
                    current = current->right;
                }
            }
        }
    }

    // ITERATIVE find max score post using BFS
    PostNode* findMaxScorePostIterative() {
        if (!root) return nullptr;
        
        PostNode* maxNode = root;
        queue<PostNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            PostNode* node = q.front();
            q.pop();
            
            if (node->score > maxNode->score) {
                maxNode = node;
            }
            
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        
        return maxNode;
    }

    // ITERATIVE inorder traversal helper
    void inorderIterative() {
        if (!root) return;
        
        stack<PostNode*> st;
        PostNode* current = root;
        
        while (current || !st.empty()) {
            while (current) {
                st.push(current);
                current = current->left;
            }
            
            current = st.top();
            st.pop();
            
            cout << "[" << current->postId << ": T=" << current->timestamp 
                 << ", S=" << current->score << "] ";
            
            current = current->right;
        }
    }

    // ITERATIVE clear using explicit stack to prevent recursion
    void clearIterative() {
        if (!root) return;
        
        stack<PostNode*> st;
        st.push(root);
        
        while (!st.empty()) {
            PostNode* node = st.top();
            st.pop();
            
            if (node->left) st.push(node->left);
            if (node->right) st.push(node->right);
            
            delete node;
        }
        
        root = nullptr;
        nodeCount = 0;
    }


    void printVerticalStructure_helper(PostNode* node, const string& prefix, bool isTail, const string& branchLabel) {
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

        // Print the node's data including the branch label (L) or (R)
        cout << "TS: " << node->timestamp << branchLabel << " | ID: " << node->postId 
            << " | Score: " << node->score << endl;

        // The new prefix for the children
        string newPrefix = prefix + (isTail ? "    " : "\u2502   "); // '|   ' or '    '

        // Recursive calls for children
        
        // 1. Process the Left child
        // Left child is the 'tail' only if the right child is null.
        if (node->left != nullptr) {
            printVerticalStructure_helper(node->left, newPrefix, node->right == nullptr, " (L)");
        }
        
        // 2. Process the Right child
        // Right child is ALWAYS the tail.
        if (node->right != nullptr) {
            printVerticalStructure_helper(node->right, newPrefix, true, " (R)");
        }
    }


    void printVerticalStructure(PostNode* root) {
        if (root == nullptr) {
            cout << "The tree is empty." << endl;
            return;
        }
        cout << "--- Tree Structure (Vertical View) ---" << endl;
        
        // Print the root node
        cout << "ROOT: TS: " << root->timestamp << " | ID: " << root->postId 
            << " | Score: " << root->score << endl;
        
        string prefix = ""; 
        
        // 1. Left child
        if (root->left != nullptr) {
            printVerticalStructure_helper(root->left, prefix, root->right == nullptr, " (L)");
        }
        // 2. Right child
        if (root->right != nullptr) {
            printVerticalStructure_helper(root->right, prefix, true, " (R)");
        }
        cout << "--------------------------------------" << endl;
    }


public:
    BinarySearchTree() : root(nullptr), nodeCount(0) {}
    
    // calculate minimum height
    int calculateMinHeight() {
        return calculateMinHeightHelper(root);
    }

    // Print tree vertical structure
    void printVerticalStructure() {
        printVerticalStructure(root);
    }

    // add Post
    void addPost(const string& postId, long long timestamp, int score) {
        insertIterative(postId, timestamp, score);
    }
    
    // Delete Post
    void deletePost(const string& postId) {
        deleteByIdIterative(postId);
    }
    
    // Like Post
    void likePost(const string& postId) {
        PostNode* node = searchByIdIterative(postId);
        if (node) {
            node->score++;
        }
    }
    
    // Print post by ID
    void printPostById(const string& postId)
    {
        PostNode* node = searchByIdIterative(postId);
        cout << "[BST] ";
        if (node) {
            cout << "Post Found: [" << node->postId << ": T=" << node->timestamp 
                 << ", S=" << node->score << "]" << endl;
        } else {
            cout << "Post ID " << postId << " not found in BST." << endl;
        }
    }

    // Get the most popular post (highest score) - O(n)
    string getMostPopular() {
        PostNode* maxNode = findMaxScorePostIterative();
        if (maxNode) {
            return maxNode->postId + " (Score: " + to_string(maxNode->score) + 
                   ", Timestamp: " + to_string(maxNode->timestamp) + ")";
        }
        return "No posts found";
    }
    
    // Get k most recent posts
    vector<string> getMostRecent(int k) {
        vector<string> result;
        reverseInorderIterative(k, result);
        return result;
    }
    
    // get height of the tree
    int getHeight() {
        return getHeightIterative();
    }
    
    // get number of nodes
    long long getNodeCount() const {
        return nodeCount;
    }
    
    // get memory usage in MB
    long long getMemoryUsage() {
        return getMemoryUsageMB();
    }
    
    // Utility functions
    void printInorder() {
        cout << "BST Inorder: ";
        inorderIterative();
        cout << endl;
    }

    //////////////////////////////////////////////////////////
    //////////////// Loading from Data set ///////////////////
    //////////////////////////////////////////////////////////


    double loadFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Unable to open CSV file: " << filename << std::endl;
            return 0.0;
        }
        
        std::string line;
        long long lineCount = 0;
        double estimatedTotalTime = 0.0;

        std::cout << "[BST] Starting CSV load from: " << filename << std::endl;
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // Skip header line if exists
        std::getline(file, line);
        
        try {
            while (std::getline(file, line)) {
                size_t pos1 = line.find(',');
                size_t pos2 = line.find(',', pos1 + 1);
                
                if (pos1 == std::string::npos || pos2 == std::string::npos) {
                    continue;
                }
                
                std::string id = line.substr(0, pos1);
                long long timestamp = std::stoll(line.substr(pos1 + 1, pos2 - pos1 - 1));
                int score = std::stoi(line.substr(pos2 + 1));
                
                insertIterative(id, timestamp, score);
                lineCount++;
                
                // Check if 30 seconds passed
                auto currentTime = std::chrono::high_resolution_clock::now();
                double elapsedSec = std::chrono::duration<double>(currentTime - startTime).count();
                
                if (elapsedSec >= 30.0) {
                    double rate = lineCount / elapsedSec;
                    estimatedTotalTime = (134000000.0 / rate); // Estimate for 1M posts
                    
                    std::cout << "\n[BST] 30-second sample completed" << std::endl;
                    std::cout << "[BST] Estimated time for 134M posts: " << std::fixed << std::setprecision(1) 
                            << estimatedTotalTime << " seconds" << std::endl;
                    break;
                }

                // Progress update every 100,000 posts
                if (lineCount % 100 == 0) {
                    auto currentTime = std::chrono::high_resolution_clock::now();
                    double elapsedSec = std::chrono::duration<double>(currentTime - startTime).count();
                    double rate = lineCount / elapsedSec;
                    std::cout << "\r[BST] Posts: " << lineCount << " | Time: " << std::fixed << std::setprecision(2) 
                              << elapsedSec << "s | Rate: " << std::fixed << std::setprecision(0) << rate << " posts/s | Memory: " 
                              << getMemoryUsageMB() << " MB" << std::flush;
                }
            }
        } catch (const std::bad_alloc& e) {
            std::cerr << "\n[BST] CRITICAL MEMORY ERROR" << std::endl;
            std::cerr << "Failed after " << lineCount << " posts" << std::endl;
            std::cerr << "Memory: " << getMemoryUsageMB() << " MB" << std::endl;
            file.close();
            return 0.0;
        } catch (const std::exception& e) {
            std::cerr << "\n[BST] Error parsing CSV: " << e.what() << std::endl;
            file.close();
            return 0.0;
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        double totalTime = std::chrono::duration<double>(endTime - startTime).count();
         
        std::cout << "\n[BST] Loaded " << lineCount << " posts in " << std::fixed << std::setprecision(3) << totalTime 
                << "s | Height: " << getHeight() << std::endl;
        
        file.close();

        return estimatedTotalTime;
    }


    void loadFromCSVWithTimeout(const std::string& filename, int timeoutSeconds = 30) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Unable to open CSV file: " << filename << std::endl;
            return;
        }

        std::string line;
        long long lineCount = 0;
        
        // std::cout << "[BST] Starting CSV load from: " << filename << std::endl;
        // std::cout << "[BST] Timeout: " << timeoutSeconds << " seconds" << std::endl;
        
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // Skip header line if exists
        std::getline(file, line);
        
        try {
            while (std::getline(file, line)) {
                // Check timeout
                auto currentTime = std::chrono::high_resolution_clock::now();
                double elapsedSec = std::chrono::duration<double>(currentTime - startTime).count();
                if (elapsedSec >= timeoutSeconds) {
                    std::cout << "\n[BST] TIMEOUT after " << timeoutSeconds << " seconds" << std::endl;
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
                
                insertIterative(id, timestamp, score);
                lineCount++;
                
                // Progress update every 100 posts (changed from 1 for better performance)
                if (lineCount % 100 == 0) {
                    double rate = lineCount / elapsedSec;
                    std::cout << "\r[BST] Posts: " << lineCount << " | Time: " << std::fixed << std::setprecision(2) 
                            << elapsedSec << "s | Rate: " << std::fixed << std::setprecision(0) << rate << " posts/s | " 
                            << "Remaining: " << (timeoutSeconds - (int)elapsedSec) << "s" << std::flush;
                }
            }
        } catch (const std::bad_alloc& e) {
            std::cerr << "\n[BST] CRITICAL MEMORY ERROR" << std::endl;
            std::cerr << "Failed after " << lineCount << " posts" << std::endl;
            std::cerr << "Memory: " << getMemoryUsageMB() << " MB" << std::endl;
            file.close();
            return;
        } catch (const std::exception& e) {
            std::cerr << "\n[BST] Error parsing CSV: " << e.what() << std::endl;
            file.close();
            return;
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        double totalTime = std::chrono::duration<double>(endTime - startTime).count();
        
        std::cout << "\n ---------- [BST] CSV LOAD COMPLETE ------------" << std::endl;
        std::cout << "[BST] Posts: " << lineCount << " | Time: " << std::fixed << std::setprecision(3) << totalTime 
                << "s | Memory: " << getMemoryUsageMB() << " MB | Height: " << getHeight() << std::endl << std::endl;
        
        file.close();
    }

    
    void loadFromJSON(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Unable to open JSON file: " << filename << std::endl;
            return;
        }
        
        std::string line;
        
        // Skip first line "["
        std::getline(file, line);
        
        try {
            while (std::getline(file, line)) {
                if (line == "]") break;
                
                // Extract data
                size_t id_start = line.find("\"id\": \"") + 7;
                size_t id_end = line.find("\"", id_start);
                if (id_start == std::string::npos || id_end == std::string::npos) continue;
                std::string id = line.substr(id_start, id_end - id_start);
                
                size_t ts_start = line.find("\"created_utc\": ") + 14;
                size_t ts_end = line.find(",", ts_start);
                if (ts_start == std::string::npos || ts_end == std::string::npos) continue;
                long long timestamp = std::stoll(line.substr(ts_start, ts_end - ts_start));
                
                size_t score_start = line.find("\"score\": ") + 9;
                size_t score_end = line.find("}");
                if (score_start == std::string::npos || score_end == std::string::npos) continue;
                int score = std::stoi(line.substr(score_start, score_end - score_start));
                
                insertIterative(id, timestamp, score);
            }
        } catch (const std::bad_alloc& e) {
            std::cerr << "Memory allocation failed during JSON load: " << e.what() << std::endl;
            file.close();
            return;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            file.close();
            return;
        }
        
        file.close();
    }


    void loadFromZST(const std::string& zstFilename) {
        std::cout << "[BST] Starting ZST stream from: " << zstFilename << std::endl;
        
        // Build decompression command
        std::string streamCmd = "zstd -dc '" + zstFilename + "' 2>/dev/null";
        FILE* pipe = popen(streamCmd.c_str(), "r");
        if (!pipe) {
            std::cerr << "Error: Failed to open ZST file: " << zstFilename << std::endl;
            return;
        }

        long long lineCount = 0;
        std::cout << "[BST] Processing compressed stream..." << std::endl;
        auto startTime = std::chrono::high_resolution_clock::now();
        
        char lineBuffer[65536];
        
        try {
            while (fgets(lineBuffer, sizeof(lineBuffer), pipe) != nullptr) {
                std::string line(lineBuffer);
                if (line.empty()) continue;
                
                // Parse JSON to extract id, created_utc, and score
                std::string id, timestamp_str, score_str;
                
                // Look for "id":"value"
                size_t id_pos = line.find("\"id\":\"");
                if (id_pos != std::string::npos) {
                    size_t id_start = id_pos + 6;
                    size_t id_end = line.find("\"", id_start);
                    if (id_end != std::string::npos) {
                        id = line.substr(id_start, id_end - id_start);
                    }
                }
                
                // Look for "created_utc":value (number without quotes)
                size_t ts_pos = line.find("\"created_utc\":");
                if (ts_pos != std::string::npos) {
                    size_t ts_start = ts_pos + 14;
                    size_t ts_end = line.find(",", ts_start);
                    if (ts_end != std::string::npos) {
                        timestamp_str = line.substr(ts_start, ts_end - ts_start);
                    }
                }
                
                // Look for "score":value (number without quotes)
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
                
                // Validate and insert
                if (!id.empty() && !timestamp_str.empty() && !score_str.empty()) {
                    try {
                        long long timestamp = std::stoll(timestamp_str);
                        int score = std::stoi(score_str);
                        insertIterative(id, timestamp, score);
                        lineCount++;
                        
                        // Progress every 100K posts
                        if (lineCount % 100 == 0) {
                            auto currentTime = std::chrono::high_resolution_clock::now();
                            double elapsedSec = std::chrono::duration<double>(currentTime - startTime).count();
                            double rate = lineCount / elapsedSec;
                            std::cout << "\r[BST] Posts: " << lineCount << " | Time: " << std::fixed << std::setprecision(2) 
                                      << elapsedSec << "s | Rate: " << std::fixed << std::setprecision(0) << rate << " posts/s | Memory: " 
                                      << getMemoryUsageMB() << " MB" << std::flush;
                        }
                    } catch (...) {
                        continue;
                    }
                }
            }
        } catch (const std::bad_alloc& e) {
            std::cerr << "\n[BST] MEMORY ERROR after " << lineCount << " posts" << std::endl;
            pclose(pipe);
            return;
        }
        
        pclose(pipe);
        auto endTime = std::chrono::high_resolution_clock::now();
        double totalTime = std::chrono::duration<double>(endTime - startTime).count();
        
        std::cout << "\n[BST] ZST LOAD COMPLETE" << std::endl;
        std::cout << "[BST] Posts: " << lineCount << " | Time: " << std::fixed << std::setprecision(3) << totalTime 
                  << "s | Memory: " << getMemoryUsageMB() << " MB | Height: " << getHeight() << std::endl;
    }


    double loadFromTGZ(const std::string& tgzFilename) {
        std::cout << "[BST] Loading from compressed dataset: " << tgzFilename << std::endl;
        
        // Use streaming approach - create a pipe that decompresses on demand
        std::string cmd = "zstd -dc '" + tgzFilename + "' 2>/dev/null";
        FILE* f = popen(cmd.c_str(), "r");
        if (!f) {
            std::cerr << "[BST] Failed to open pipe" << std::endl;
            return 0.0;
        }
        
        char buffer[65536];
        long long lineCount = 0;
        double estimatedTotalTime = 0.0;
        auto startTime = std::chrono::high_resolution_clock::now();
        
        std::cout << "[BST] Starting streaming load..." << std::endl;
        
        std::string fullLine;
        while (fgets(buffer, sizeof(buffer), f) != nullptr) {
            fullLine += buffer;
            
            // Check if 30 seconds passed
            auto currentTime = std::chrono::high_resolution_clock::now();
            double elapsedSec = std::chrono::duration<double>(currentTime - startTime).count();
            
            if (elapsedSec >= 30.0) {
                double rate = lineCount / elapsedSec;
                estimatedTotalTime = (134000000.0 / rate); // Estimate for 1M posts
                
                std::cout << "\n[BST] 30-second sample completed" << std::endl;
                std::cout << "[BST] Estimated time for 134M posts: " << std::fixed << std::setprecision(1) 
                        << estimatedTotalTime << " seconds" << std::endl;
                break;
            }

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
                        
                        insertIterative(id, timestamp, score);
                        lineCount++;
                        
                        if (lineCount % 1000 == 0) {
                            auto currentTime = std::chrono::high_resolution_clock::now();
                            double elapsedSec = std::chrono::duration<double>(currentTime - startTime).count();
                            double rate = lineCount / elapsedSec;
                            std::cout << "\r[BST] Posts: " << lineCount << " | Time: " << std::fixed << std::setprecision(2) 
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
        
        std::cout << "\n[BST] LOAD COMPLETE" << std::endl;
        std::cout << "[BST] Posts Loaded: " << lineCount << " | Time: " << std::fixed << std::setprecision(3) 
                  << totalTime << "s | Tree Height: " << getHeight() << std::endl;
    
        return estimatedTotalTime;
    }


    void loadFromTGZWithTimeout(const std::string& tgzFilename, int timeoutSeconds = 30) {
        // std::cout << "[BST] Loading from compressed dataset: " << tgzFilename << std::endl;
        // std::cout << "[BST] Timeout: " << timeoutSeconds << " seconds" << std::endl;
        
        // Use streaming approach - create a pipe that decompresses on demand
        std::string cmd = "zstd -dc '" + tgzFilename + "' 2>/dev/null";
        FILE* f = popen(cmd.c_str(), "r");
        if (!f) {
            std::cerr << "[BST] Failed to open pipe" << std::endl;
            return;
        }
        
        char buffer[65536];
        long long lineCount = 0;
        auto startTime = std::chrono::high_resolution_clock::now();
        
        std::cout << "[BST] Starting streaming load..." << std::endl;
        
        std::string fullLine;
        while (fgets(buffer, sizeof(buffer), f) != nullptr) {
            // Check timeout
            auto currentTime = std::chrono::high_resolution_clock::now();
            double elapsedSec = std::chrono::duration<double>(currentTime - startTime).count();
            if (elapsedSec >= timeoutSeconds) {
                std::cout << "\n[BST] TIMEOUT after " << timeoutSeconds << " seconds" << std::endl;
                break;
            }
            
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
                        
                        insertIterative(id, timestamp, score);
                        lineCount++;
                        
                        if (lineCount % 100 == 0) {
                            double rate = lineCount / elapsedSec;
                            std::cout << "\r[BST] Posts: " << lineCount << " | Time: " << std::fixed << std::setprecision(2) 
                                    << elapsedSec << "s | Rate: " << std::fixed << std::setprecision(0) << rate << " posts/s" 
                                    << " | Remaining: " << (timeoutSeconds - (int)elapsedSec) << "s" << std::flush;
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
        
        std::cout << "\n ----------- [BST] LOAD COMPLETE -----------" << std::endl;
        std::cout << "[BST] Posts Loaded: " << lineCount << " | Time: " << std::fixed << std::setprecision(3) 
                << totalTime << "s | Tree Height: " << getHeight() << std::endl << std::endl;
    }


    // Destructor
    ~BinarySearchTree() {
        clearIterative();
    }
};

#endif // BST_H