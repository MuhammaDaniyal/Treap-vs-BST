#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <cstdlib>

#include "BST.h"
#include "Treap.h"

using namespace std;

#ifndef ANALYSIS_H
#define ANALYSIS_H

struct Post {
    string postId;
    long long timestamp;
    int score;
};

struct LoadingResults {
    long long bst_csv_posts;
    long long treap_csv_posts;
    long long bst_tgz_posts;
    long long treap_tgz_posts;
    int bst_csv_height;
    int treap_csv_height;
    int bst_tgz_height;
    int treap_tgz_height;
    double total_time;
};

struct OperationMetrics {
    double insertionTime_BST, insertionTime_Treap;
    double deletionTime_BST, deletionTime_Treap;
    double searchTime_BST, searchTime_Treap;
    double likeTime_BST, likeTime_Treap;
    double balancingFactor_BST, balancingFactor_Treap;
    double queryTime_BST, queryTime_Treap;
    int treeHeight_BST, treeHeight_Treap;
    int rotations_Treap;
    long long nodeCount_BST, nodeCount_Treap;
};

class ComparisonAnalysis {
private:
    BinarySearchTree bst_csv, bst_tgz, bst_test;
    Treap treap_csv, treap_tgz, treap_test;
    LoadingResults results;
    OperationMetrics opMetrics;
    vector<Post> testDataSet;
    
public:
    ComparisonAnalysis() {
        results = {0, 0, 0, 0, 0, 0, 0, 0, 0.0};
        opMetrics = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    }
    
    void initializeTestData(int dataSize) {
        testDataSet.clear();
        for (int i = 0; i < dataSize; i++) {
            Post p;
            p.postId = "post_" + to_string(i);
            p.timestamp = 1609459200 + (i * 3600);
            p.score = (rand() % 1000) + 1;
            testDataSet.push_back(p);
        }
    }
    
    /////////////////////////////////////////////////////////
    ///////////////// LOADING ANALYSIS //////////////////////
    /////////////////////////////////////////////////////////

    void loadingFileAnalysis(int timeLimitSeconds, const string& csv_path, const string& tgz_path) {

        cout << "┌────────────────────────────────────────────────────────────┐" << endl;
        cout << "│              COMPARISON ANALYSIS - STARTING                │" << endl;
        cout << "├────────────────────────────────────────────────────────────┤" << endl;
        cout << "│ CSV Path: " << setw(47) << left << csv_path << " │" << endl;
        cout << "│ TGZ Path: " << setw(47) << left << tgz_path << " │" << endl;
        cout << "│ Time Limit: " << setw(44) << left << (to_string(timeLimitSeconds) + " seconds") << " │" << endl;
        cout << "└────────────────────────────────────────────────────────────┘" << endl;
        
        testFileLoadingPerformance(timeLimitSeconds, csv_path, tgz_path);
        displayResults();
    }

    /// Run loading performance tests

    void testFileLoadingPerformance(int timeLimitSeconds, const string& csv_path, const string& tgz_path) {
        results = runLoadingComparison(timeLimitSeconds, csv_path, tgz_path);
    }

    /// Run loading comparison and return results
        
    LoadingResults runLoadingComparison(int timeLimitSeconds, const string& csv_path, const string& tgz_path) {
        LoadingResults res = {0, 0, 0, 0, 0, 0, 0, 0, 0.0};
        
        auto overallStart = chrono::high_resolution_clock::now();
        
        // Load BST from CSV
        cout << "[1/4] BST from CSV..." << endl;
        bst_csv.loadFromCSVWithTimeout(csv_path, timeLimitSeconds);
        res.bst_csv_posts = bst_csv.getNodeCount();
        res.bst_csv_height = bst_csv.getHeight();
        cout << "      Posts: " << res.bst_csv_posts << " | Height: " << res.bst_csv_height << endl;
        
        // Load Treap from CSV
        cout << "[2/4] Treap from CSV..." << endl;
        treap_csv.loadFromCSVWithTimeout(csv_path, timeLimitSeconds);
        res.treap_csv_posts = treap_csv.getNodeCount();
        res.treap_csv_height = treap_csv.getHeight();
        cout << "      Posts: " << res.treap_csv_posts << " | Height: " << res.treap_csv_height << endl;
        
        // Load BST from TGZ
        cout << "[3/4] BST from TGZ..." << endl;
        bst_tgz.loadFromTGZWithTimeout(tgz_path, timeLimitSeconds);
        res.bst_tgz_posts = bst_tgz.getNodeCount();
        res.bst_tgz_height = bst_tgz.getHeight();
        cout << "      Posts: " << res.bst_tgz_posts << " | Height: " << res.bst_tgz_height << endl;
        
        // Load Treap from TGZ
        cout << "[4/4] Treap from TGZ..." << endl;
        treap_tgz.loadFromTGZWithTimeout(tgz_path, timeLimitSeconds);
        res.treap_tgz_posts = treap_tgz.getNodeCount();
        res.treap_tgz_height = treap_tgz.getHeight();
        cout << "      Posts: " << res.treap_tgz_posts << " | Height: " << res.treap_tgz_height << endl;
        
        auto overallEnd = chrono::high_resolution_clock::now();
        res.total_time = chrono::duration<double>(overallEnd - overallStart).count();
        
        return res;
    }
  
    /// Display loading results in a formatted table

    void displayResults() {
        cout << "\n┌─────────────────────────────────────────────────────────────────────┐" << endl;
        cout << "│                      FILE LOADING RESULTS SUMMARY                   │" << endl;
        cout << "├─────────────────────────────────────────────────────────────────────┤" << endl;
        cout << "│ Structure  │ Source │ Posts Loaded │ Tree Height │ Loading Status   │" << endl;
        cout << "├────────────┼────────┼──────────────┼─────────────┼──────────────────┤" << endl;
        cout << "│    BST     │  CSV   │ " << setw(12) << results.bst_csv_posts << " │ " << setw(11) << results.bst_csv_height << " │ " 
            << (results.bst_csv_posts > 0 ? "    ✅ Done     " : "    ❌ Failed   ") << " │" << endl;
        cout << "├────────────┼────────┼──────────────┼─────────────┼──────────────────┤" << endl;
        cout << "│   Treap    │  CSV   │ " << setw(12) << results.treap_csv_posts << " │ " << setw(11) << results.treap_csv_height << " │ " 
            << (results.treap_csv_posts > 0 ? "    ✅ Done     " : "    ❌ Failed   ") << " │" << endl;
        cout << "├────────────┼────────┼──────────────┼─────────────┼──────────────────┤" << endl;
        cout << "│    BST     │  TGZ   │ " << setw(12) << results.bst_tgz_posts << " │ " << setw(11) << results.bst_tgz_height << " │ " 
            << (results.bst_tgz_posts > 0 ? "    ✅ Done     " : "    ❌ Failed   ") << " │" << endl;
        cout << "├────────────┼────────┼──────────────┼─────────────┼──────────────────┤" << endl;
        cout << "│   Treap    │  TGZ   │ " << setw(12) << results.treap_tgz_posts << " │ " << setw(11) << results.treap_tgz_height << " │ " 
            << (results.treap_tgz_posts > 0 ? "    ✅ Done     " : "    ❌ Failed   ") << " │" << endl;
        cout << "├────────────┴────────┴──────────────┴─────────────┴──────────────────┤" << endl;
        cout << "│ Total Time: " << setw(50) << fixed << setprecision(3) << (to_string(results.total_time) + " seconds") << " │" << endl;
        cout << "└─────────────────────────────────────────────────────────────────────┘" << endl;
        
        // Add graph
        string pythonCmd = "python3 scripts/plot_file_loading.py";
        pythonCmd += " " + to_string(results.bst_csv_posts);
        pythonCmd += " " + to_string(results.treap_csv_posts);
        pythonCmd += " " + to_string(results.bst_tgz_posts);
        pythonCmd += " " + to_string(results.treap_tgz_posts);
        pythonCmd += " " + to_string(results.bst_csv_height);
        pythonCmd += " " + to_string(results.treap_csv_height);
        pythonCmd += " " + to_string(results.bst_tgz_height);
        pythonCmd += " " + to_string(results.treap_tgz_height);

        int graphStatus = system(pythonCmd.c_str());
        cout << "📊 File loading comparison graph closed." << endl;
    } 


    /////////////////////////////////////////////////////////
    //////////// FULL DATA SET LOAD ANALYSIS ////////////////
    /////////////////////////////////////////////////////////

    /// Test CSV loading performance for full data set

    void testCSVLoading(const string& csv_path) {
        cout << "Testing CSV Loading..." << endl;
        BinarySearchTree bst;
        Treap treap;
        
        double time_treap = treap.loadFromCSV(csv_path);
        double time_bst = bst.loadFromCSV(csv_path);  // Now this works!

        cout << "┌───────────────────┬────────────┬────────────┬────────────┐" << endl;
        cout << "│      Metric       │    BST     │   Treap    │   Winner   │" << endl;
        cout << "├───────────────────┼────────────┼────────────┼────────────┤" << endl;
        cout << "│ Loading Time (s)  │ " << setw(10) << fixed << setprecision(3) << time_bst 
            << " │ " << setw(10) << time_treap << " │ ";
        if (time_bst < time_treap) {
            cout << "   BST    │" << endl;
        } else {
            cout << "  Treap    │" << endl;
        }
        cout << "├───────────────────┼────────────┼────────────┼────────────┤" << endl;
        cout << "│ Tree Height       │ " << setw(10) << bst.getHeight() 
            << " │ " << setw(10) << treap.getHeight() << " │ ";
        if (bst.getHeight() < treap.getHeight()) {
            cout << "   BST    │" << endl;
        } else {
            cout << "  Treap    │" << endl;
        }
        cout << "└───────────────────┴────────────┴────────────┴────────────┘" << endl;
    
    
        string pythonCmd = "python3 scripts/plot_loading.py";
        pythonCmd += " CSV"; 
        pythonCmd += " " + to_string(time_bst);
        pythonCmd += " " + to_string(time_treap);
        pythonCmd += " " + to_string(bst.getHeight());
        pythonCmd += " " + to_string(treap.getHeight());

        int graphStatus = system(pythonCmd.c_str());
        cout << "📊 Loading performance graph closed." << endl;
    }
   
    /// Test TGZ loading performance for full data set

    void testTGZLoading(const string& tgz_path) {
        cout << "Testing TGZ Loading..." << endl;
        BinarySearchTree bst;
        Treap treap;
        
        double time_treap = treap.loadFromTGZ(tgz_path);
        double time_bst = bst.loadFromTGZ(tgz_path);

        cout << "┌───────────────────┬────────────┬────────────┬────────────┐" << endl;
        cout << "│      Metric       │    BST     │   Treap    │   Winner   │" << endl;
        cout << "├───────────────────┼────────────┼────────────┼────────────┤" << endl;
        cout << "│ Loading Time (s)  │ " << setw(10) << fixed << setprecision(3) << time_bst 
            << " │ " << setw(10) << time_treap << " │ ";
        if (time_bst < time_treap) {
            cout << "   BST    │" << endl;
        } else {
            cout << "  Treap    │" << endl;
        }
        cout << "├───────────────────┼────────────┼────────────┼────────────┤" << endl;
        cout << "│ Tree Height       │ " << setw(10) << bst.getHeight() 
            << " │ " << setw(10) << treap.getHeight() << " │ ";
        if (bst.getHeight() < treap.getHeight()) {
            cout << "   BST    │" << endl;
        } else {
            cout << "  Treap    │" << endl;
        }
        cout << "└───────────────────┴────────────┴────────────┴────────────┘" << endl;

        // Add graph
        string pythonCmd = "python3 scripts/plot_loading.py";
        pythonCmd += " TGZ"; 
        pythonCmd += " " + to_string(time_bst);
        pythonCmd += " " + to_string(time_treap);
        pythonCmd += " " + to_string(bst.getHeight());
        pythonCmd += " " + to_string(treap.getHeight());

        int graphStatus = system(pythonCmd.c_str());
        cout << "📊 TGZ Loading performance graph closed." << endl;
    }
    

    ///////////////////////////////////////////////////////
    ////////////// INSERTION ANALYSIS /////////////////////
    ///////////////////////////////////////////////////////

    void testInsertionPerformance() {
        cout << "\n========================================" << endl;
        cout << "INSERTION PERFORMANCE TEST" << endl;
        cout << "========================================" << endl;
        
        // Test with different dataset sizes
        vector<int> testSizes = {100, 1000, 5000, 10000};
        vector<double> bstInsertTimes, treapInsertTimes;
        vector<int> bstHeights, treapHeights;
        vector<double> bstBalancingFactors, treapBalancingFactors;  // NEW

        int lineCount = 0;


        for (int size : testSizes) {
            cout << "\n--- Testing with " << size << " posts ---" << endl;
            
            // Generate test data
            
            initializeTestData(size);
            
            // Test BST insertion
            BinarySearchTree bst;
            auto start = chrono::high_resolution_clock::now(),
                elapsedTime = start;

            for (const auto& post : testDataSet) {
                bst.addPost(post.postId, post.timestamp, post.score);
                        
                if(++lineCount % 1000 == 0) {
                    elapsedTime = chrono::high_resolution_clock::now();
                    double elapsedSeconds = chrono::duration<double>(elapsedTime - start).count();
                    double rate = lineCount / elapsedSeconds;
                    
                    std::cout << "\r[BST] Posts: " << lineCount << " | Time: " 
                        << elapsedSeconds << "s | Rate: " << std::fixed << std::setprecision(0) << rate << " posts/s\n" 
                        << std::string(20, ' ') << std::flush;
                }
            }
            lineCount = 0; // Reset for next test

            auto end = chrono::high_resolution_clock::now();
            double bst_time = chrono::duration<double, milli>(end - start).count();
            int bst_height = bst.getHeight();
            
            // Test Treap insertion
            Treap treap;
            start = chrono::high_resolution_clock::now();

            for (const auto& post : testDataSet) {
                treap.addPost(post.postId, post.timestamp, post.score);
                if(++lineCount % 1000 == 0) {
                    elapsedTime = chrono::high_resolution_clock::now();
                    double elapsedSeconds = chrono::duration<double>(elapsedTime - start).count();
                    double rate = lineCount / elapsedSeconds;
                    
                    std::cout << "\r[Treap] Posts: " << lineCount << " | Time: " 
                        << elapsedSeconds << "s | Rate: " << std::fixed << std::setprecision(0) << rate << " posts/s\n" 
                        << std::string(20, ' ') << std::flush;
                }
            }
            
            end = chrono::high_resolution_clock::now();
            double treap_time = chrono::duration<double, milli>(end - start).count();
            int treap_height = treap.getHeight();
            int rotations = treap.getRotationCount();
            std::cout << std::endl;
            std::cout << "┌───────────────────┬────────────┬────────────┬────────────┐" << std::endl;
            std::cout << "│      Category     │    BST     │   Treap    │   Winner   │" << std::endl;
            std::cout << "├───────────────────┼────────────┼────────────┼────────────┤" << std::endl;
            std::cout << "│ Insertion Time(ms)│ " << std::setw(10) << fixed << setprecision(3) << bst_time 
                      << " │ " << std::setw(10) << fixed << setprecision(3) << treap_time << " │ ";
            if (bst_time < treap_time) {
                std::cout << "   BST    │" << std::endl;
            } else {
                std::cout << "  Treap    │" << std::endl;
            }
            std::cout << "├───────────────────┼────────────┼────────────┼────────────┤" << std::endl;
            std::cout << "│   Tree Height     │ " << std::setw(10) << bst_height 
                      << " │ " << std::setw(10) << treap_height << " │ ";
            if (bst_height < treap_height) {
                std::cout << "   BST    │" << std::endl;
            } else {
                std::cout << "  Treap    │" << std::endl;
            }
            std::cout << "├───────────────────┼────────────┼────────────┼────────────┤" << std::endl;
            std::cout << "│   Rotations       │     N/A    │ " << std::setw(10) << treap.getRotationCount() << " │   Treap    │" << std::endl;
            std::cout << "└───────────────────┴────────────┴────────────┴────────────┘" << std::endl;

            bstInsertTimes.push_back(bst_time);
            treapInsertTimes.push_back(treap_time);

            bstHeights.push_back(bst.getHeight());
            treapHeights.push_back(treap.getHeight());
        
            double bstBalance = (double)bst.calculateMinHeight() / bst.getHeight();
            double treapBalance = (double)treap.calculateMinHeight() / treap.getHeight();
            bstBalancingFactors.push_back(bstBalance);
            treapBalancingFactors.push_back(treapBalance);
        }

        opMetrics.insertionTime_BST = calculateAverage(bstInsertTimes);
        opMetrics.insertionTime_Treap = calculateAverage(treapInsertTimes);

        opMetrics.treeHeight_BST = calculateAverage(bstHeights);
        opMetrics.treeHeight_Treap = calculateAverage(treapHeights);

        opMetrics.balancingFactor_BST = calculateAverage(bstBalancingFactors);
        opMetrics.balancingFactor_Treap = calculateAverage(treapBalancingFactors);

        /// Graphs

        // Build Python command with dynamic data   
        string pythonCmd = "python3 scripts/plot_insertion.py";
        for (int size : testSizes) pythonCmd += " " + to_string(size);
        for (double time : bstInsertTimes) pythonCmd += " " + to_string(time);
        for (double time : treapInsertTimes) pythonCmd += " " + to_string(time);
        
        // Run Python in background
        int temp = system((pythonCmd).c_str());
        
        cout << "📊 Graph generating in background..." << endl;
    }

    ///////////////////////////////////////////////////////
    ///////////////// SEARCH ANALYSIS /////////////////////
    ///////////////////////////////////////////////////////

    void testSearchPerformance() {
        cout << "\n========================================" << endl;
        cout << "SEARCH PERFORMANCE TEST" << endl;
        cout << "========================================" << endl;
        
        // Use a medium-sized dataset
        initializeTestData(5000);
        
        // Build both trees first
        BinarySearchTree bst;
        Treap treap;
        
        cout << "Building trees for search tests..." << endl;
        int lineCount = 0;
        
        // Build BST
        auto start = chrono::high_resolution_clock::now();
        for (const auto& post : testDataSet) {
            bst.addPost(post.postId, post.timestamp, post.score);
            if(++lineCount % 1000 == 0) {
                auto elapsedTime = chrono::high_resolution_clock::now();
                double elapsedSeconds = chrono::duration<double>(elapsedTime - start).count();
                double rate = lineCount / elapsedSeconds;
                cout << "\r[BST Build] Posts: " << lineCount << " | Time: " 
                    << elapsedSeconds << "s | Rate: " << fixed << setprecision(0) << rate << " posts/s" 
                    << string(20, ' ') << flush;
            }
        }
        cout << endl;
        
        // Build Treap
        lineCount = 0;
        start = chrono::high_resolution_clock::now();
        for (const auto& post : testDataSet) {
            treap.addPost(post.postId, post.timestamp, post.score);
            if(++lineCount % 1000 == 0) {
                auto elapsedTime = chrono::high_resolution_clock::now();
                double elapsedSeconds = chrono::duration<double>(elapsedTime - start).count();
                double rate = lineCount / elapsedSeconds;
                cout << "\r[Treap Build] Posts: " << lineCount << " | Time: " 
                    << elapsedSeconds << "s | Rate: " << fixed << setprecision(0) << rate << " posts/s" 
                    << string(20, ' ') << flush;
            }
        }
        cout << endl;
        
        cout << "Trees built - BST Height: " << bst.getHeight() << " | Treap Height: " << treap.getHeight() << endl;
        
        // Test 1: getMostPopular() performance
        cout << "\n--- getMostPopular() Test (1000 iterations) ---" << endl;
        
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++) {
            bst.getMostPopular();
        }
        auto end = chrono::high_resolution_clock::now();
        double bst_most_popular = chrono::duration<double, micro>(end - start).count() / 1000.0;
        
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++) {
            treap.getMostPopular();
        }
        end = chrono::high_resolution_clock::now();
        double treap_most_popular = chrono::duration<double, micro>(end - start).count() / 1000.0;
        
        // Test 2: getMostRecent(k) performance
        cout << "\n--- getMostRecent(10) Test (100 iterations) ---" << endl;
        
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 100; i++) {
            bst.getMostRecent(10);
        }
        end = chrono::high_resolution_clock::now();
        double bst_most_recent = chrono::duration<double, micro>(end - start).count() / 100.0;
        
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 100; i++) {
            treap.getMostRecent(10);
        }
        end = chrono::high_resolution_clock::now();
        double treap_most_recent = chrono::duration<double, micro>(end - start).count() / 100.0;
        
        // Display results in table format
        cout << "\n┌──────────────────────────┬────────────┬────────────┬────────────┐" << endl;
        cout << "│        Category          │    BST     │   Treap    │   Winner   │" << endl;
        cout << "├──────────────────────────┼────────────┼────────────┼────────────┤" << endl;
        
        // getMostPopular row
        cout << "│  getMostPopular (μs)     │ " << setw(10) << fixed << setprecision(3) << bst_most_popular 
            << " │ " << setw(10) << fixed << setprecision(3) << treap_most_popular << " │ ";
        if (bst_most_popular < treap_most_popular) {
            cout << "   BST    │" << endl;
        } else {
            cout << "  Treap    │" << endl;
        }
        
        // getMostRecent row
        cout << "├──────────────────────────┼────────────┼────────────┼────────────┤" << endl;
        cout << "│ getMostRecent(10) (μs)   │ " << setw(10) << fixed << setprecision(3) << bst_most_recent 
            << " │ " << setw(10) << fixed << setprecision(3) << treap_most_recent << " │ ";
        if (bst_most_recent < treap_most_recent) {
            cout << "   BST    │" << endl;
        } else {
            cout << "  Treap    │" << endl;
        }
        
        // Speed multiplier row
        cout << "├──────────────────────────┼────────────┼────────────┼────────────┤" << endl;
        double popular_speedup = bst_most_popular / treap_most_popular;
        double recent_speedup = bst_most_recent / treap_most_recent;
        
        cout << "│ Popular Speed Multiplier │" << setw(10) << fixed << setprecision(1) << popular_speedup 
            << "x │ " << setw(9) << fixed << setprecision(1) << recent_speedup << "x │ ";
        
        if (popular_speedup > 1.0 || recent_speedup > 1.0) {
            cout << "  Treap    │" << endl;
        } else {
            cout << "   BST    │" << endl;
        }
        
        cout << "└──────────────────────────┴────────────┴────────────┴────────────┘" << endl;

        double avg_bst_search = (bst_most_popular + bst_most_recent) / 2.0;
        double avg_treap_search = (treap_most_popular + treap_most_recent) / 2.0;

        // Store in your metrics struct
        opMetrics.searchTime_BST = avg_bst_search;
        opMetrics.searchTime_Treap = avg_treap_search;

        // Graph for search performance
        string pythonCmd = "python3 scripts/plot_search.py";
        pythonCmd += " " + to_string(bst_most_popular);
        pythonCmd += " " + to_string(bst_most_recent);
        pythonCmd += " " + to_string(treap_most_popular);
        pythonCmd += " " + to_string(treap_most_recent);

        int temp = system(pythonCmd.c_str());
        cout << "📊 Search performance graph closed. Continuing..." << endl;

    }

    ////////////////////////////////////////////////////////
    ////////////// LIKE OPERATION ANALYSIS //////////////////
    ////////////////////////////////////////////////////////

    void testLikeOperationPerformance() {
        cout << "\n========================================" << endl;
        cout << "LIKE OPERATION PERFORMANCE TEST" << endl;
        cout << "========================================" << endl;
        
        // Use a medium-sized dataset
        initializeTestData(5000);
        
        // Build both trees first
        BinarySearchTree bst;
        Treap treap;
        
        cout << "Building trees for like operation tests..." << endl;
        int lineCount = 0;
        
        // Build BST
        auto start = chrono::high_resolution_clock::now();
        for (const auto& post : testDataSet) {
            bst.addPost(post.postId, post.timestamp, post.score);
            if(++lineCount % 1000 == 0) {
                auto elapsedTime = chrono::high_resolution_clock::now();
                double elapsedSeconds = chrono::duration<double>(elapsedTime - start).count();
                double rate = lineCount / elapsedSeconds;
                cout << "\r[BST Build] Posts: " << lineCount << " | Time: " 
                    << elapsedSeconds << "s | Rate: " << fixed << setprecision(0) << rate << " posts/s" 
                    << string(20, ' ');
                cout.flush();
            }
        }
        cout << endl;
        
        // Build Treap
        lineCount = 0;
        start = chrono::high_resolution_clock::now();
        for (const auto& post : testDataSet) {
            treap.addPost(post.postId, post.timestamp, post.score);
            if(++lineCount % 1000 == 0) {
                auto elapsedTime = chrono::high_resolution_clock::now();
                double elapsedSeconds = chrono::duration<double>(elapsedTime - start).count();
                double rate = lineCount / elapsedSeconds;
                cout << "\r[Treap Build] Posts: " << lineCount << " | Time: " 
                    << elapsedSeconds << "s | Rate: " << fixed << setprecision(0) << rate << " posts/s" 
                    << string(20, ' ');
                cout.flush();
            }
        }
        cout << endl;
        
        cout << "Trees built - BST Height: " << bst.getHeight() << " | Treap Height: " << treap.getHeight() << endl;
        
        // Reset rotation counter for Treap
        treap.resetRotationCount();
        
        // Test 1: Single like operation performance
        cout << "\n--- Single Like Operation Test (1000 iterations) ---" << endl;
        
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++) {
            int randomIndex = rand() % testDataSet.size();
            bst.likePost(testDataSet[randomIndex].postId);
        }
        auto end = chrono::high_resolution_clock::now();
        double bst_like_time = chrono::duration<double, micro>(end - start).count() / 1000.0;
        
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++) {
            int randomIndex = rand() % testDataSet.size();
            treap.likePost(testDataSet[randomIndex].postId);
        }
        end = chrono::high_resolution_clock::now();
        double treap_like_time = chrono::duration<double, micro>(end - start).count() / 1000.0;
        int total_rotations = treap.getRotationCount();
        
        // Test 2: Multiple likes on same post (bubbling test)
        cout << "\n--- Multiple Likes Bubbling Test ---" << endl;
        
        // Pick one post to like multiple times
        string testPostId = testDataSet[100].postId;
        int initial_rotations = treap.getRotationCount();
        
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 100; i++) {
            treap.likePost(testPostId);
        }
        end = chrono::high_resolution_clock::now();
        double treap_bubble_time = chrono::duration<double, micro>(end - start).count() / 100.0;
        int bubble_rotations = treap.getRotationCount() - initial_rotations;
        
        // For BST, just update score (no structural changes)
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 100; i++) {
            bst.likePost(testPostId);
        }
        end = chrono::high_resolution_clock::now();
        double bst_update_time = chrono::duration<double, micro>(end - start).count() / 100.0;
        
        // Display results in table format
        cout << "\n┌─────────────────────────────┬────────────┬────────────┬────────────┐" << endl;
        cout << "│          Category           │    BST     │   Treap    │   Winner   │" << endl;
        cout << "├─────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
        
        // Single like row
        cout << "│  Single Like Time (μs)      │ " << setw(10) << fixed << setprecision(3) << bst_like_time 
            << " │ " << setw(10) << fixed << setprecision(3) << treap_like_time << " │ ";
        if (bst_like_time < treap_like_time) {
            cout << "   BST     │" << endl;
        } else {
            cout << "  Treap   │" << endl;
        }
        
        // Multiple likes row
        cout << "├─────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
        cout << "│  Multiple Likes Time (μs)   │ " << setw(10) << fixed << setprecision(3) << bst_update_time 
            << " │ " << setw(10) << fixed << setprecision(3) << treap_bubble_time << " │ ";
        if (bst_update_time < treap_bubble_time) {
            cout << "   BST     │" << endl;
        } else {
            cout << "  Treap   │" << endl;
        }
        
        // Rotations row
        cout << "├─────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
        cout << "│  Total Rotations            │     N/A    │ " << setw(10) << total_rotations 
            << " │   Treap    │" << endl;
        
        // Bubble rotations row
        cout << "├─────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
        cout << "│  Bubble Rotations           │     N/A    │ " << setw(10) << bubble_rotations 
            << " │   Treap    │" << endl;
        
        // Speed multiplier row
        cout << "├─────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
        double like_speedup = bst_like_time / treap_like_time;
        double bubble_speedup = bst_update_time / treap_bubble_time;
        cout << "│  Like Speed Multiplier      │ " << setw(9) << fixed << setprecision(1) << like_speedup 
            << "x │ " << setw(9) << fixed << setprecision(1) << bubble_speedup << "x │ ";
        if (like_speedup > 1.0 || bubble_speedup > 1.0) {
            cout << "  Treap   │" << endl;
        } else {
            cout << "   BST     │" << endl;
        }
        
        cout << "└─────────────────────────────┴────────────┴────────────┴────────────┘" << endl;
        
        // Summary
        cout << "\n=== LIKE OPERATION SUMMARY ===" << endl;
        if (bst_like_time < treap_like_time) {
            cout << "✅ BST wins single like operations - " << fixed << setprecision(1) 
                << (treap_like_time / bst_like_time) << "x faster (no rotations needed)" << endl;
        } else {
            cout << "✅ Treap wins single like operations - maintains heap property with rotations" << endl;
        }
        
        cout << "✅ Treap performed " << total_rotations << " total rotations to maintain heap property" << endl;
        cout << "✅ During bubbling test, Treap performed " << bubble_rotations << " rotations" << endl;
        
        // Check if bubbling actually worked
        string most_popular_after = treap.getMostPopular();
        if (most_popular_after.find(testPostId) != string::npos) {
            cout << "✅ Bubbling verified - test post became most popular!" << endl;
        }
    
        string pythonCmd = "python3 scripts/plot_likes.py";
        pythonCmd += " " + to_string(bst_like_time);
        pythonCmd += " " + to_string(treap_like_time);
        pythonCmd += " " + to_string(bst_update_time);
        pythonCmd += " " + to_string(treap_bubble_time);
        pythonCmd += " " + to_string(total_rotations);
        pythonCmd += " " + to_string(bubble_rotations);

        int graphStatus = system(pythonCmd.c_str());
        cout << "📊 Like operations graph closed. Continuing..." << endl;
    
    }

    ////////////////////////////////////////////////////////
    ////////////// DELETION ANALYSIS /////////////////////
    ////////////////////////////////////////////////////////
    
    void testDeletionPerformance() {
        cout << "\n========================================" << endl;
        cout << "DELETION PERFORMANCE TEST" << endl;
        cout << "========================================" << endl;
        
        // Test with different dataset sizes
        vector<int> testSizes = {1000, 5000};
        
        vector<int> bstInitialHeights, treapInitialHeights;
        vector<int> bstFinalHeights, treapFinalHeights;
        vector<int> deletionRotations;

        vector<double> bstDeletionTimes, treapDeletionTimes;
        int lineCount = 0;

        for (int size : testSizes) {
            cout << "\n--- Testing with " << size << " posts ---" << endl;
            
            // Generate test data
            initializeTestData(size);
            
            // Test BST deletion
            BinarySearchTree bst;
            for (const auto& post : testDataSet) {
                bst.addPost(post.postId, post.timestamp, post.score);
            }
            int initial_bst_height = bst.getHeight();
            long long initial_bst_count = bst.getNodeCount();
            
            // Inside the loop, after building trees but BEFORE deletion, store initial heights:
            bstInitialHeights.push_back(initial_bst_height);


            auto start = chrono::high_resolution_clock::now();
            lineCount = 0;
            
            // Delete first 30% of posts
            int deleteCount = size * 0.3;
            for (int i = 0; i < deleteCount; i++) {
                bst.deletePost(testDataSet[i].postId);
                
                if(++lineCount % 100 == 0) {
                    auto elapsedTime = chrono::high_resolution_clock::now();
                    double elapsedSeconds = chrono::duration<double>(elapsedTime - start).count();
                    double rate = lineCount / elapsedSeconds;
                    cout << "\r[BST Delete] Deleted: " << lineCount << " | Time: " 
                        << elapsedSeconds << "s | Rate: " << fixed << setprecision(0) << rate << " deletes/s" 
                        << string(20, ' ');
                    cout.flush();
                }
            }
            auto end = chrono::high_resolution_clock::now();
            double bst_time = chrono::duration<double, milli>(end - start).count();
            
            int final_bst_height = bst.getHeight();
            bstFinalHeights.push_back(final_bst_height);
            long long final_bst_count = bst.getNodeCount();
            cout << endl;
            
            // Test Treap deletion
            Treap treap;
            for (const auto& post : testDataSet) {
                treap.addPost(post.postId, post.timestamp, post.score);
            }
            int initial_treap_height = treap.getHeight();
            treapInitialHeights.push_back(initial_treap_height);
            
            long long initial_treap_count = treap.getNodeCount();
            treap.resetRotationCount();
            
            start = chrono::high_resolution_clock::now();
            lineCount = 0;
            
            // Delete first 30% of posts
            for (int i = 0; i < deleteCount; i++) {
                treap.deletePost(testDataSet[i].postId);
                
                if(++lineCount % 100 == 0) {
                    auto elapsedTime = chrono::high_resolution_clock::now();
                    double elapsedSeconds = chrono::duration<double>(elapsedTime - start).count();
                    double rate = lineCount / elapsedSeconds;
                    cout << "\r[Treap Delete] Deleted: " << lineCount << " | Time: " 
                        << elapsedSeconds << "s | Rate: " << fixed << setprecision(0) << rate << " deletes/s" 
                        << string(20, ' ');
                    cout.flush();
                }
            }
            end = chrono::high_resolution_clock::now();
            double treap_time = chrono::duration<double, milli>(end - start).count();
            
            int final_treap_height = treap.getHeight();
            long long final_treap_count = treap.getNodeCount();
            int deletion_rotations = treap.getRotationCount();
            treapFinalHeights.push_back(final_treap_height);
            deletionRotations.push_back(deletion_rotations);

            cout << endl;
            
            // Display results in table format
            cout << "\n┌─────────────────────────────┬────────────┬────────────┬────────────┐" << endl;
            cout << "│          Category           │    BST     │   Treap    │   Winner   │" << endl;
            cout << "├─────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
            
            // Deletion time row
            cout << "│ Deletion Time (ms)          │ " << setw(10) << fixed << setprecision(3) << bst_time 
                << " │ " << setw(10) << fixed << setprecision(3) << treap_time << " │ ";
            if (bst_time < treap_time) {
                cout << "   BST     │" << endl;
            } else {
                cout << "  Treap    │" << endl;
            }
            
            // Initial height row
            cout << "├─────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
            cout << "│ Initial Height              │ " << setw(10) << initial_bst_height 
                << " │ " << setw(10) << initial_treap_height << " │ ";
            if (initial_bst_height < initial_treap_height) {
                cout << "   BST     │" << endl;
            } else {
                cout << "  Treap    │" << endl;
            }
            
            // Final height row
            cout << "├─────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
            cout << "│ Final Height                │ " << setw(10) << final_bst_height 
                << " │ " << setw(10) << final_treap_height << " │ ";
            if (final_bst_height < final_treap_height) {
                cout << "   BST     │" << endl;
            } else {
                cout << "  Treap    │" << endl;
            }
            
            // Height change row
            cout << "├─────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
            int bst_height_change = initial_bst_height - final_bst_height;
            int treap_height_change = initial_treap_height - final_treap_height;
            cout << "│ Height Change               │ " << setw(10) << bst_height_change 
                << " │ " << setw(10) << treap_height_change << " │ ";
            if (bst_height_change > treap_height_change) {
                cout << "   BST     │" << endl;
            } else {
                cout << "  Treap    │" << endl;
            }
            
            // Rotations row
            cout << "├─────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
            cout << "│ Deletion Rotations          │     N/A    │ " << setw(10) << deletion_rotations 
                << " │   Treap    │" << endl;
            
            // Nodes deleted row
            cout << "├─────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
            long long bst_deleted = initial_bst_count - final_bst_count;
            long long treap_deleted = initial_treap_count - final_treap_count;
            cout << "│ Nodes Deleted               │ " << setw(10) << bst_deleted 
                << " │ " << setw(10) << treap_deleted << " │ ";
            if (bst_deleted > treap_deleted) {
                cout << "   BST     │" << endl;
            } else if (treap_deleted > bst_deleted) {
                cout << "  Treap    │" << endl;
            } else {
                cout << "   Tie     │" << endl;
            }
            
            cout << "└─────────────────────────────┴────────────┴────────────┴────────────┘" << endl;
            
            // Summary
            cout << "\n=== DELETION SUMMARY ===" << endl;
            cout << "Deleted " << deleteCount << " posts out of " << size << " total" << endl;
            
            if (bst_time < treap_time) {
                cout << "✅ BST deletion is " << fixed << setprecision(1) << (treap_time / bst_time) 
                    << "x faster than Treap" << endl;
            } else {
                cout << "✅ Treap deletion is " << fixed << setprecision(1) << (bst_time / treap_time) 
                    << "x faster than BST" << endl;
            }
            
            cout << "✅ Treap performed " << deletion_rotations << " rotations during deletion" << endl;
            cout << "✅ Height reduction - BST: " << bst_height_change << ", Treap: " << treap_height_change << endl;
        
            bstDeletionTimes.push_back(bst_time);
            treapDeletionTimes.push_back(treap_time);
        }

        opMetrics.deletionTime_BST = calculateAverage(bstDeletionTimes);
        opMetrics.deletionTime_Treap = calculateAverage(treapDeletionTimes);
    
    
        // ADD THIS GRAPH CODE AT THE END OF THE FUNCTION (after the loop):
        string pythonCmd = "python3 scripts/plot_deletion.py";
        for (int size : testSizes) pythonCmd += " " + to_string(size);
        for (double time : bstDeletionTimes) pythonCmd += " " + to_string(time);
        for (double time : treapDeletionTimes) pythonCmd += " " + to_string(time);
        for (int height : bstInitialHeights) pythonCmd += " " + to_string(height);
        for (int height : treapInitialHeights) pythonCmd += " " + to_string(height);
        for (int rotation : deletionRotations) pythonCmd += " " + to_string(rotation);

        // Add error checking
        int graphStatus = system(pythonCmd.c_str());
        if (graphStatus != 0) {
            cout << "❌ Graph generation failed with status: " << graphStatus << endl;
        } else {
            cout << "📊 Deletion performance graph closed. Continuing..." << endl;
        }    
    }

    ////////////////////////////////////////////////////////
    ////////////// QUERY OPERATION ANALYSIS //////////////////
    ////////////////////////////////////////////////////////

    void testQueryPerformance() {
        cout << "\n========================================" << endl;
        cout << "QUERY PERFORMANCE TEST" << endl;
        cout << "========================================" << endl;
        
        // Use a larger dataset for meaningful query tests
        initializeTestData(10000);
        
        // Build both trees
        BinarySearchTree bst;
        Treap treap;
        
        cout << "Building trees for query tests..." << endl;
        int lineCount = 0;
        
        // Build BST
        auto start = chrono::high_resolution_clock::now();
        for (const auto& post : testDataSet) {
            bst.addPost(post.postId, post.timestamp, post.score);
            if(++lineCount % 2000 == 0) {
                auto elapsedTime = chrono::high_resolution_clock::now();
                double elapsedSeconds = chrono::duration<double>(elapsedTime - start).count();
                double rate = lineCount / elapsedSeconds;
                cout << "\r[BST Build] Posts: " << lineCount << " | Time: " 
                    << elapsedSeconds << "s | Rate: " << fixed << setprecision(0) << rate << " posts/s" 
                    << string(20, ' ');
                cout.flush();
            }
        }
        cout << endl;
        
        // Build Treap
        lineCount = 0;
        start = chrono::high_resolution_clock::now();
        for (const auto& post : testDataSet) {
            treap.addPost(post.postId, post.timestamp, post.score);
            if(++lineCount % 2000 == 0) {
                auto elapsedTime = chrono::high_resolution_clock::now();
                double elapsedSeconds = chrono::duration<double>(elapsedTime - start).count();
                double rate = lineCount / elapsedSeconds;
                cout << "\r[Treap Build] Posts: " << lineCount << " | Time: " 
                    << elapsedSeconds << "s | Rate: " << fixed << setprecision(0) << rate << " posts/s" 
                    << string(20, ' ');
                cout.flush();
            }
        }
        cout << endl;
        
        cout << "Trees built - BST Height: " << bst.getHeight() << " | Treap Height: " << treap.getHeight() << endl;
        
        // Test 1: getMostPopular() - Single call vs Multiple calls
        cout << "\n--- getMostPopular() Performance ---" << endl;
        
        start = chrono::high_resolution_clock::now();
        string bst_most_popular = bst.getMostPopular();
        auto end = chrono::high_resolution_clock::now();
        double bst_single_popular = chrono::duration<double, micro>(end - start).count();
        
        start = chrono::high_resolution_clock::now();
        string treap_most_popular = treap.getMostPopular();
        end = chrono::high_resolution_clock::now();
        double treap_single_popular = chrono::duration<double, micro>(end - start).count();
        
        // Test 2: getMostRecent(k) with different k values
        cout << "\n--- getMostRecent(k) Performance ---" << endl;
        
        vector<int> k_values = {5, 10, 20, 50};
        vector<double> bst_recent_times, treap_recent_times;
        
        for (int k : k_values) {
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; i++) {
                bst.getMostRecent(k);
            }
            end = chrono::high_resolution_clock::now();
            bst_recent_times.push_back(chrono::duration<double, micro>(end - start).count() / 100.0);
            
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; i++) {
                treap.getMostRecent(k);
            }
            end = chrono::high_resolution_clock::now();
            treap_recent_times.push_back(chrono::duration<double, micro>(end - start).count() / 100.0);
        }
        
        // Test 3: Mixed query workload
        cout << "\n--- Mixed Query Workload ---" << endl;
        
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 500; i++) {
            if (i % 3 == 0) bst.getMostPopular();
            else if (i % 3 == 1) bst.getMostRecent(5);
            else bst.getMostRecent(15);
        }
        end = chrono::high_resolution_clock::now();
        double bst_mixed_time = chrono::duration<double, micro>(end - start).count() / 500.0;
        
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 500; i++) {
            if (i % 3 == 0) treap.getMostPopular();
            else if (i % 3 == 1) treap.getMostRecent(5);
            else treap.getMostRecent(15);
        }
        end = chrono::high_resolution_clock::now();
        double treap_mixed_time = chrono::duration<double, micro>(end - start).count() / 500.0;
        
        // Display results in table format
        cout << "\n┌───────────────────────────────┬────────────┬────────────┬────────────┐" << endl;
        cout << "│           Category            │    BST     │   Treap    │   Winner   │" << endl;
        cout << "├───────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
        
        // getMostPopular single row
        cout << "│ getMostPopular Single (μs)    │ " << setw(10) << fixed << setprecision(3) << bst_single_popular 
            << " │ " << setw(10) << fixed << setprecision(3) << treap_single_popular << " │ ";
        if (bst_single_popular < treap_single_popular) {
            cout << "   BST    │" << endl;
        } else {
            cout << "  Treap   │" << endl;
        }
        
        // Mixed workload row
        cout << "├───────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
        cout << "│ Mixed Workload (μs)           │ " << setw(10) << fixed << setprecision(3) << bst_mixed_time 
            << " │ " << setw(10) << fixed << setprecision(3) << treap_mixed_time << " │ ";
        if (bst_mixed_time < treap_mixed_time) {
            cout << "   BST    │" << endl;
        } else {
            cout << "  Treap   │" << endl;
        }
        
        // getMostRecent for different k values
        cout << "├───────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
        for (size_t i = 0; i < k_values.size(); i++) {
            cout << "│ getMostRecent(" << setw(2) << k_values[i] << ") (μs)        │ " 
                << setw(10) << fixed << setprecision(3) << bst_recent_times[i] 
                << " │ " << setw(10) << fixed << setprecision(3) << treap_recent_times[i] << " │ ";
            if (bst_recent_times[i] < treap_recent_times[i]) {
                cout << "   BST    │" << endl;
            } else {
                cout << "  Treap   │" << endl;
            }
            if (i < k_values.size() - 1) {
                cout << "├───────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
            }
        }
        
        // Performance summary row
        cout << "├───────────────────────────────┼────────────┼────────────┼────────────┤" << endl;
        double popular_speedup = bst_single_popular / treap_single_popular;
        double mixed_speedup = bst_mixed_time / treap_mixed_time;
        cout << "│ Overall Speed Multiplier      │" << setw(10) << fixed << setprecision(1) << popular_speedup 
            << "x │ " << setw(9) << fixed << setprecision(1) << mixed_speedup << "x │ ";
        if (popular_speedup > 1.0 || mixed_speedup > 1.0) {
            cout << "  Treap   │" << endl;
        } else {
            cout << "   BST    │" << endl;
        }
        
        cout << "└───────────────────────────────┴────────────┴────────────┴────────────┘" << endl;
        
        // Summary
        cout << "\n=== QUERY PERFORMANCE SUMMARY ===" << endl;
        cout << "Most Popular Post - BST: " << bst_most_popular.substr(0, 50) << "..." << endl;
        cout << "Most Popular Post - Treap: " << treap_most_popular.substr(0, 50) << "..." << endl;
        
        if (treap_single_popular < bst_single_popular) {
            cout << "✅ Treap dominates getMostPopular() - " << fixed << setprecision(1) 
                << (bst_single_popular / treap_single_popular) << "x faster!" << endl;
        }
        
        if (treap_mixed_time < bst_mixed_time) {
            cout << "✅ Treap wins mixed workload - better for real-world scenarios" << endl;
        }
        
        // Analyze getMostRecent performance trends
        cout << "\n=== getMostRecent(k) ANALYSIS ===" << endl;
        for (size_t i = 0; i < k_values.size(); i++) {
            double speedup = bst_recent_times[i] / treap_recent_times[i];
            cout << "k=" << k_values[i] << ": BST=" << fixed << setprecision(3) << bst_recent_times[i] 
                << "μs, Treap=" << treap_recent_times[i] << "μs, Speedup=" << fixed << setprecision(1) << speedup << "x" << endl;
        }

        // Graph for query performance
        string pythonCmd = "python3 scripts/plot_queries.py";
        pythonCmd += " " + to_string(bst_single_popular);
        pythonCmd += " " + to_string(treap_single_popular);
        pythonCmd += " " + to_string(bst_mixed_time);
        pythonCmd += " " + to_string(treap_mixed_time);

        // Add k values
        for (int k : k_values) pythonCmd += " " + to_string(k);

        // Add recent times
        for (double time : bst_recent_times) pythonCmd += " " + to_string(time);
        for (double time : treap_recent_times) pythonCmd += " " + to_string(time);

        int graphStatus = system(pythonCmd.c_str());
        cout << "📊 Query performance graph closed. Continuing..." << endl;
    
    }

    //////////////////////////////////////////////////////////
    ////////////// FINAL COMPREHENSIVE ANALYSIS /////////////
    //////////////////////////////////////////////////////////

    void runFinalComprehensiveAnalysis() {
        cout << "\n" << string(70, '=') << endl;
        cout << "🏆 FINAL COMPREHENSIVE ANALYSIS - BST vs TREAP" << endl;
        cout << string(70, '=') << endl;
        
        // Reset cumulative metrics
        opMetrics = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        
        // Run all tests
        testInsertionPerformance();
        testSearchPerformance(); 
        testLikeOperationPerformance();
        testDeletionPerformance();
        testQueryPerformance();
        
        printComparisonTable();
        saveResultsToFile();
    }

    /// Print comparison table for final analysis

    void printComparisonTable() {
        cout << "┌──────────────────────────────────────┬──────────────────────┬──────────────────────┐" << endl;
        cout << "│              Metrics                 │        Treaps        │  Binary Search Tree  │" << endl;
        cout << "├──────────────────────────────────────┼──────────────────────┼──────────────────────┤" << endl;
        
        // Insertion Time
        cout << "│ Insertion Time (avg)                 │ " << setw(17) << fixed << setprecision(3) << opMetrics.insertionTime_Treap 
            << " ms │ " << setw(17) << opMetrics.insertionTime_BST << " ms │" << endl;
        
        // Deletion Time  
        cout << "├──────────────────────────────────────┼──────────────────────┼──────────────────────┤" << endl;
        cout << "│ Deletion Time (avg)                  │ " << setw(17) << opMetrics.deletionTime_Treap 
            << " ms │ " << setw(17) << opMetrics.deletionTime_BST << " ms │" << endl;
        
        // Search Time
        cout << "├──────────────────────────────────────┼──────────────────────┼──────────────────────┤" << endl;
        cout << "│ Search Time (avg)                    │ " << setw(17) << opMetrics.searchTime_Treap 
            << " μs │ " << setw(17) << opMetrics.searchTime_BST << " μs │" << endl;
        
        // Height of the Tree
        cout << "├──────────────────────────────────────┼──────────────────────┼──────────────────────┤" << endl;
        cout << "│ Height of the Tree                   │ " << setw(17) << opMetrics.treeHeight_Treap 
            << "    │ " << setw(17) << opMetrics.treeHeight_BST << "    │" << endl;
        
        // Tree Balancing Factor
// Tree Balancing Factor - show as percentage
        cout << "├──────────────────────────────────────┼──────────────────────┼──────────────────────┤" << endl;
        cout << "│ Tree Balancing Factor                │ " << setw(17) << fixed << setprecision(1) << (opMetrics.balancingFactor_Treap * 100) 
            << " %  │ " << setw(17) << (opMetrics.balancingFactor_BST * 100) << " %  │" << endl;        
        cout << "└──────────────────────────────────────┴──────────────────────┴──────────────────────┘" << endl;
    }

    /// Save results to a text file

    void saveResultsToFile() {
        // Save only operation metrics (no loading results)
        cout << "\nSaving operation results to comparison_analysis.txt..." << endl;
        ofstream file("comparison_analysis.txt");
        
        if (file.is_open()) {
            file << "┌──────────────────────────────────────┬──────────────────────┬──────────────────────┐" << endl;
            file << "│              Metrics                 │        Treaps        │  Binary Search Tree  │" << endl;
            file << "├──────────────────────────────────────┼──────────────────────┼──────────────────────┤" << endl;
            
            // Insertion Time
            file << "│ Insertion Time (avg)                 │ " << setw(17) << fixed << setprecision(3) << opMetrics.insertionTime_Treap 
                << " ms │ " << setw(17) << opMetrics.insertionTime_BST << " ms │" << endl;
            
            // Deletion Time  
            file << "├──────────────────────────────────────┼──────────────────────┼──────────────────────┤" << endl;
            file << "│ Deletion Time (avg)                  │ " << setw(17) << opMetrics.deletionTime_Treap 
                << " ms │ " << setw(17) << opMetrics.deletionTime_BST << " ms │" << endl;
            
            // Search Time
            file << "├──────────────────────────────────────┼──────────────────────┼──────────────────────┤" << endl;
            file << "│ Search Time (avg)                    │ " << setw(17) << opMetrics.searchTime_Treap 
                << " μs │ " << setw(17) << opMetrics.searchTime_BST << " μs │" << endl;
  
            // Query Time
            file << "├──────────────────────────────────────┼──────────────────────┼──────────────────────┤" << endl;
            file << "│ Query Time (avg)                     │ " << setw(17) << opMetrics.queryTime_Treap 
                << " μs │ " << setw(17) << opMetrics.queryTime_BST << " μs │" << endl;
  
            // Like post Time
            file << "├──────────────────────────────────────┼──────────────────────┼──────────────────────┤" << endl;
            file << "│ Like Post Time (avg)                 │ " << setw(17) << opMetrics.likeTime_Treap 
                << " μs │ " << setw(17) << opMetrics.likeTime_BST << " μs │" << endl;
  
            // Height of the Tree
            file << "├──────────────────────────────────────┼──────────────────────┼──────────────────────┤" << endl;
            file << "│ Height of the Tree                   │ " << setw(17) << opMetrics.treeHeight_Treap 
                << "    │ " << setw(17) << opMetrics.treeHeight_BST << "    │" << endl;
            
            // Tree Balancing Factor
            file << "├──────────────────────────────────────┼──────────────────────┼──────────────────────┤" << endl;
            file << "│ Tree Balancing Factor                │ " << setw(17) << fixed << setprecision(1) << (opMetrics.balancingFactor_Treap * 100) 
                << " %  │ " << setw(17) << (opMetrics.balancingFactor_BST * 100) << " %  │" << endl;        
            file << "└──────────────────────────────────────┴──────────────────────┴──────────────────────┘" << endl;
            
            file.close();
            cout << "✅ Operation results saved successfully!" << endl;
        }
        else {
            cout << "❌ Error: Unable to open file for writing." << endl;
        }
    }

    ////////////////////////////////////////////////
    ////////////////// Utilities ///////////////////
    ////////////////////////////////////////////////

    /// Helper function to calculate average of a vector of doubles

    double calculateAverage(const vector<double>& values) {
        double sum = 0;
        for(double val : values) sum += val;
        return sum / values.size();
    }

    double calculateAverage(const vector<int>& values) {
        int sum = 0;
        for(int val : values) sum += val;
        return sum / values.size();
    }

};

#endif