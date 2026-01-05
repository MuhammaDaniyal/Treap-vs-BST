// Menu.h
#ifndef MENU_H
#define MENU_H

#include "ComparisonAnalysis.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

class MenuSystem {
private:
    ComparisonAnalysis analysis;
    string csv_path;
    string tgz_path;
    int timeLimit;
    
public:
    MenuSystem() 
        : csv_path("/media/daniyal/New Volume/DataSet/reddit_data.csv"),
          tgz_path("/media/daniyal/New Volume/DataSet/dataset.tgz"),
          timeLimit(1) {}
    
    void run() {
        showMainMenu();
    }

private:

    ////////////////////////////////////////////
    ////////////// MAIN MENU ///////////////////
    ////////////////////////////////////////////

    void showMainMenu() {
        int choice;
        
        do {
            cout << "\n" << string(70, '=') << endl;
            cout << "🌳 BST vs TREAP PERFORMANCE ANALYZER - MAIN MENU" << endl;
            cout << string(70, '=') << endl;
            cout << "1. 📊 Performance Analysis Tests" << endl;
            cout << "2. 🎮 Interactive Operations (Add/Remove/Like Posts)" << endl;
            cout << "3. ⚙️  Configuration & Settings" << endl;
            cout << "0. 🚪 Exit" << endl;
            cout << string(70, '=') << endl;
            cout << "Enter your choice (0-4): ";
            
            cin >> choice;
            
            switch(choice) {
                case 1:
                    showAnalysisMenu();
                    break;
                case 2:
                    showInteractiveMenu();
                    break;
                case 3:
                    showConfigurationMenu();
                    break;
                case 0:
                    cout << "Goodbye! 👋" << endl;
                    break;
                default:
                    cout << "❌ Invalid choice!" << endl;
            }
            
        } while (choice != 0);
    }

    ////////////////////////////////////////////
    ///////////// ANALYSIS MENU ////////////////
    ////////////////////////////////////////////

    void showAnalysisMenu() {
        int choice;
        
        do {
            cout << "\n" << string(60, '=') << endl;
            cout << "📊 PERFORMANCE ANALYSIS TESTS" << endl;
            cout << string(60, '=') << endl;
            cout << "1. 📁 File Loading Analysis" << endl;
            cout << "2. ⚡ Insertion Performance" << endl;
            cout << "3. 🔍 Search Performance" << endl;
            cout << "4. ❤️  Like Operations" << endl;
            cout << "5. 🗑️  Deletion Performance" << endl;
            cout << "6. 📈 Query Performance" << endl;
            cout << "7. 🏆 Complete Analysis" << endl;
            cout << "0. ↩️  Back to Main Menu" << endl;
            cout << string(60, '=') << endl;
            cout << "Enter your choice (0-8): ";
            
            cin >> choice;
            
            switch(choice) {
                case 1:
                    analysis.loadingFileAnalysis(timeLimit, csv_path, tgz_path);
                    break;
                case 2:
                    analysis.testInsertionPerformance();
                    break;
                case 3:
                    analysis.testSearchPerformance();
                    break;
                case 4:
                    analysis.testLikeOperationPerformance();
                    break;
                case 5:
                    analysis.testDeletionPerformance();
                    break;
                case 6:
                    analysis.testQueryPerformance();
                    break;
                case 7:
                    analysis.runFinalComprehensiveAnalysis();
                    break;
                case 0:
                    cout << "Returning to main menu..." << endl;
                    break;
                default:
                    cout << "❌ Invalid choice!" << endl;
            }
            
            pause();
            
        } while (choice != 0);
    }

    ////////////////////////////////////////////////
    //////////// INTERACTIVE OPERATIONS ////////////
    ////////////////////////////////////////////////

    void showInteractiveMenu() {
        int choice;
        BinarySearchTree bst;
        Treap treap;
        
        do {
            cout << "\n" << string(60, '=') << endl;
            cout << "🎮 INTERACTIVE OPERATIONS" << endl;
            cout << string(60, '=') << endl;
            cout << "1. ➕ Add Post Manually" << endl;
            cout << "2. 🗑️  Remove Post" << endl;
            cout << "3. ❤️  Like Post" << endl;
            cout << "4. 🔍 Search Post" << endl;
            cout << "5. 📊 Show Most Popular" << endl;
            cout << "6. ⏰ Show Most Recent" << endl;
            cout << "7. 🌳 Print Tree Structures" << endl;
            cout << "0. ↩️  Back to Main Menu" << endl;
            cout << string(60, '=') << endl;
            cout << "Enter your choice (0-7): ";
            
            cin >> choice;
            
            switch(choice) {
                case 1:
                    addPostManually(bst, treap);
                    break;
                case 2:
                    removePostManually(bst, treap);
                    break;
                case 3:
                    likePostManually(bst, treap);
                    break;
                case 4:
                    searchPostManually(bst, treap);
                    break;
                case 5:
                    showMostPopular(bst, treap);
                    break;
                case 6: 
                    showMostRecent(bst, treap);
                    break;
                case 7: 
                    printTreeStructure(bst, treap);
                    break;
                case 0:
                    cout << "Returning to main menu..." << endl;
                    break;
                default:
                    cout << "❌ Invalid choice!" << endl;
            }
            
            pause();
            
        } while (choice != 0);
    }

    /// Add post
    
    void addPostManually(BinarySearchTree& bst, Treap& treap) {
        string id;
        long long timestamp;
        int score;
        
        cout << "\n➕ ADD POST MANUALLY" << endl;
        cout << "Enter Post ID: ";
        cin >> id;
        cout << "Enter Timestamp: ";
        cin >> timestamp;
        cout << "Enter Score: ";
        cin >> score;

        bst.addPost(id, timestamp, score);
        treap.addPost(id, timestamp, score);
        
        cout << "✅ Post added to both BST and Treap!" << endl;
    }

    /// Remove post

    void removePostManually(BinarySearchTree& bst, Treap& treap) {
        string id;
        cout << "\n🗑️ REMOVE POST" << endl;
        cout << "Enter Post ID to remove: ";
        cin >> id;
        
        bst.deletePost(id);
        treap.deletePost(id);
        
        cout << "✅ Post removed from both structures!" << endl;
    }

    /// Like post

    void likePostManually(BinarySearchTree& bst, Treap& treap) {
        string id;
        cout << "\n❤️ LIKE POST" << endl;
        cout << "Enter Post ID to like: ";
        cin >> id;
        
        bst.likePost(id);
        treap.likePost(id);
        
        cout << "✅ Post liked in both structures!" << endl;
    }

    /// Search post

    void searchPostManually(BinarySearchTree& bst, Treap& treap) {
        string id;
        cout << "\n🔍 SEARCH POST" << endl;
        cout << "Enter Post ID to search: ";
        cin >> id;
        
        treap.printPostById(id);
        bst.printPostById(id);
    }

    /// Show most popular post

    void showMostPopular(BinarySearchTree& bst, Treap& treap) {
        cout << "\n📊 MOST POPULAR POSTS" << endl;
        cout << "BST: " << bst.getMostPopular() << endl;
        cout << "Treap: " << treap.getMostPopular() << endl;
    }

    /// Show most recent posts

    void showMostRecent(BinarySearchTree& bst, Treap& treap) {
        int k;
        cout << "\n⏰ MOST RECENT POSTS" << endl;
        cout << "How many recent posts? ";
        cin >> k;
        
        vector<string> bst_most = bst.getMostRecent(k);
        vector<string> treap_most = treap.getMostRecent(k);
        
        for(const string& postId : bst_most) {
            cout << "BST: " << postId << endl;
        }
        for(const string& postId : treap_most) {
            cout << "Treap: " << postId << endl;
        }

    }

    /// Print tree structures

    void printTreeStructure(BinarySearchTree& bst, Treap& treap) {
        cout << "\n🌳 TREE STRUCTURES" << endl;
        cout << "BST Structure:" << endl;
        bst.printVerticalStructure();
        cout << endl;
        cout << "Treap Structure:" << endl;
        treap.printTreapStructure();
        cout << "--------------------------------------------------\n\n";
    }

    ////////////////////////////////////////////
    /////////// CONFIGURATION MENU /////////////
    ////////////////////////////////////////////

    void showConfigurationMenu() {
        int choice;
        
        do {
            cout << "\n" << string(60, '=') << endl;
            cout << "⚙️ CONFIGURATION & SETTINGS" << endl;
            cout << string(60, '=') << endl;
            cout << "1. 📁 Change Dataset Paths" << endl;
            cout << "2. ⏱️ Change Time Limit" << endl;
            cout << "3. 📊 Show Current Settings" << endl;
            cout << "0. ↩️ Back to Main Menu" << endl;
            cout << string(60, '=') << endl;
            cout << "Enter your choice (0-3): ";
            
            cin >> choice;
            
            switch(choice) {
                case 1:
                    changeDatasetPaths();
                    break;
                case 2:
                    cout << "Enter new time limit (seconds): ";
                    cin >> timeLimit;
                    cout << "✅ Time limit updated to " << timeLimit << " seconds" << endl;
                    break;
                case 3:
                    showCurrentSettings();
                    break;
                case 0:
                    cout << "Returning to main menu..." << endl;
                    break;
                default:
                    cout << "❌ Invalid choice!" << endl;
            }
            
            pause();
            
        } while (choice != 0);
    }

    /// Change dataset file paths

    void changeDatasetPaths() {
        cout << "\n📁 CHANGE DATASET PATHS" << endl;
        cout << "Current CSV: " << csv_path << endl;
        cout << "Current TGZ: " << tgz_path << endl;
        
        cout << "Enter new CSV path (or press Enter to keep current): ";
        cin.ignore();
        string new_csv;
        getline(cin, new_csv);
        if (!new_csv.empty()) csv_path = new_csv;
        
        cout << "Enter new TGZ path (or press Enter to keep current): ";
        string new_tgz;
        getline(cin, new_tgz);
        if (!new_tgz.empty()) tgz_path = new_tgz;
        
        cout << "✅ Paths updated successfully!" << endl;
    }

    /// Show current configuration settings

    void showCurrentSettings() {
        cout << "\n📊 CURRENT SETTINGS" << endl;
        cout << "CSV Path: " << csv_path << endl;
        cout << "TGZ Path: " << tgz_path << endl;
        cout << "Time Limit: " << timeLimit << " seconds" << endl;
    }


    /// ------ Pause for user input before continuing -------

    void pause() {
        if (cin.peek() == '\n') cin.ignore();
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
};

#endif