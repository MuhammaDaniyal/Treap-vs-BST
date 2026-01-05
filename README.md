# Treap vs Binary Search Tree: Performance Analysis & Comparison

<div align="center">

A comprehensive empirical analysis comparing the performance characteristics of Treaps (Treap trees) and Binary Search Trees in handling real-world post data with insertion, deletion, search, and update operations.

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
![Language](https://img.shields.io/badge/Language-C++-00599C?logo=cplusplus)
![Python](https://img.shields.io/badge/Visualization-Python-3776AB?logo=python)

</div>

---

## 📋 Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Features](#features)
- [Performance Results](#performance-results)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Compilation](#compilation)
- [Usage](#usage)
  - [Running the Program](#running-the-program)
  - [Generating Performance Graphs](#generating-performance-graphs)
- [Implementation Details](#implementation-details)
  - [Treap (Treap Tree)](#treap-treap-tree)
  - [Binary Search Tree](#binary-search-tree)
  - [Comparison Analysis](#comparison-analysis)
- [Results & Analysis](#results--analysis)
- [Contributing](#contributing)
- [License](#license)
- [Author](#author)

---

## 🎯 Overview

This project implements and compares two binary tree data structures for managing **real-world Reddit post data**. It performs extensive benchmarking across multiple operations:

- **Insertion**: Adding new posts to the tree
- **Deletion**: Removing posts from the tree
- **Search**: Finding posts by timestamp
- **Updates**: Liking posts to increase their score
- **Tree Metrics**: Height, balance factor, and structural analysis

The analysis demonstrates that **Treaps provide superior insertion and search performance** with consistent O(log n) average-case behavior, while **BSTs excel in deletion operations** but suffer from tree imbalance in worst-case scenarios.

### 📊 Dataset Information

This project utilizes real-world Reddit post data with multiple format options:

| Dataset Version | Format | Size | Use Case |
|-----------------|--------|------|----------|
| **Processed** | CSV | 3 GB | Development & Testing |
| **Compressed** | TGZ | 15 GB | Standard Benchmarking |
| **Raw** | Uncompressed | 200 GB | Comprehensive Analysis |

**Extracted Fields from Reddit Data:**
- Post ID (unique identifier)
- Timestamp (creation time)
- Score (popularity metric)

For initial testing and development, the **3GB CSV version** is recommended. Dataset paths are configurable in [Menu.h](Menu.h) - see [Configuration & Dataset Paths](#configuration--dataset-paths) section.

Data set is from Kaggle

The Pushshift Reddit Dataset - submissions
Link: [text](https://www.kaggle.com/datasets/i221113hadiyatanveer/the-pushshift-reddit-dataset-submissions)

---

## 📁 Project Structure

```
Treap-vs-BST/
├── main.cpp                    # Entry point and test cases
├── Menu.h                      # Interactive menu system for operations
├── Treap.h                     # Treap (Randomized BST) implementation
├── BST.h                       # Binary Search Tree implementation
├── ComparisonAnalysis.h        # Benchmarking and performance analysis functions
├── comparison_analysis.txt     # Detailed timing and metric results
├── README.md                   # Project documentation (this file)
├── LICENSE                     # MIT License
├── graphs/                     # Directory for generated performance graphs
└── scripts/                    # Python visualization scripts
    ├── plot_insertion.py       # Visualization: insertion time analysis
    ├── plot_deletion.py        # Visualization: deletion time analysis
    ├── plot_search.py          # Visualization: search operation analysis
    ├── plot_queries.py         # Visualization: query time analysis
    ├── plot_likes.py           # Visualization: like operation analysis
    ├── plot_loading.py         # Visualization: file loading performance
    └── plot_file_loading.py    # Visualization: detailed file I/O analysis
```

---

## ✨ Features

### Core Operations
✅ **Post Management**
- Add posts with unique ID, timestamp, and initial score
- Delete posts while maintaining tree integrity
- Update post scores through "like" operations

✅ **Search & Retrieval**
- Find posts by timestamp
- Retrieve most popular post in O(1) time
- Range queries on timestamps

✅ **Performance Metrics**
- Precise timing measurements using `std::chrono`
- Tree height calculations
- Balance factor analysis
- Memory usage tracking

✅ **Interactive Menu System**
- User-friendly CLI interface
- Manual operation testing
- Batch processing capabilities

✅ **Comprehensive Visualization**
- Python-based graph generation
- Performance comparison charts
- Operation time distribution plots
- Scalability analysis

---

## 📊 Performance Results

Based on extensive benchmarking with ~10,000 operations:

| Metric | Treap | Binary Search Tree |
|--------|-------|-------------------|
| **Insertion Time (avg)** | 1.814 ms | 168.290 ms |
| **Deletion Time (avg)** | 182.766 ms | 1.228 ms |
| **Search Time (avg)** | 7.415 μs | 416.532 μs |
| **Tree Height** | 25 | 4025 |
| **Balance Factor** | 17.5% | 0.3% |

### Key Insights

🔍 **Treap Advantages:**
- **56x faster insertion** due to randomized priority-based balancing
- **Consistent O(log n) search** with better average-case performance
- **Excellent height balance** (25 vs 4025 levels)
- Better cache locality in balanced structures

⚡ **BST Advantages:**
- **149x faster deletion** using standard rotation techniques
- Simpler implementation without randomization overhead
- Better deletion patterns for sequential data

---

## 🚀 Getting Started

### Prerequisites

- **C++17** or higher
- **Python 3.x** (for graph generation)
- GCC/Clang compiler
- Linux/macOS/Windows with standard build tools
- zstd library (`libzstd-dev`)

#### Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get install build-essential libzstd-dev python3 python3-pip
pip3 install matplotlib numpy
```

### Installation

Clone the repository:
```bash
git clone https://github.com/yourusername/Treap-vs-BST.git
cd Treap-vs-BST
```

### Compilation

#### Using g++
```bash
g++ -std=c++17 -O2 main.cpp -o treap_bst -lzstd

./treap_bst
```

### Interactive Menu Options

The program presents an interactive menu with the following operations:

```
╔════════════════════════════════╗
║   Treap vs BST Comparison      ║
╚════════════════════════════════╝

1. Add Post
2. Delete Post
3. Search Post
4. Like Post
5. Get Most Popular Post
6. Display Tree Structure
7. Run Comparison Analysis
8. Exit
```

---

## 🔧 Implementation Details

### Treap (Treap Tree)

**File:** Treap.h

A **Treap** (Tree + Heap) combines properties of binary search trees and heaps:

- **BST Property**: Maintains timestamp ordering for efficient search
- **Heap Property**: Uses random priorities for automatic balancing
- **Rotations**: Left and right rotations maintain the heap property

**Key Characteristics:**
- Average O(log n) time complexity for all operations
- O(1) space per node (excluding data)
- Self-balancing without explicit balance factors
- Probabilistic guarantees prevent pathological cases

**Operations:**
```cpp
void addPost(string id, long long timestamp, int score)      // O(log n)
void deletePost(string id)                                    // O(log n)
PostInfo searchPost(string id)                                // O(log n)
void likePost(string id)                                      // O(log n)
string getMostPopular()                                       // O(1)
```

### Binary Search Tree

**File:** BST.h

A standard **Binary Search Tree** without balancing:

- **Simple Structure**: Keys ordered left-to-right
- **No Automatic Balance**: Height can become O(n) in worst case
- **Fast Deletion**: Efficient removal using standard techniques

**Key Characteristics:**
- Average O(log n), worst-case O(n) time complexity
- Minimal overhead per node
- Straightforward implementation
- Vulnerable to unbalanced input sequences

**Operations:**
```cpp
void addPost(string id, long long timestamp, int score)      // O(log n) avg
void deletePost(string id)                                    // O(log n) avg
PostInfo searchPost(string id)                                // O(log n) avg
void likePost(string id)                                      // O(log n) avg
string getMostPopular()                                       // O(1)
```

### Comparison Analysis

**File:** ComparisonAnalysis.h

Comprehensive benchmarking module providing:

- **Timing Measurements**: Microsecond precision with `std::chrono::high_resolution_clock`
- **Statistical Analysis**: Average, min, max, and variance calculations
- **Memory Profiling**: Peak memory usage tracking
- **Tree Metrics**: Height, balance factor, and structural properties
- **Batch Operations**: Automated testing with varying dataset sizes

**Analysis Functions:**
- `runComparisonAnalysis()` - Complete benchmark suite
- `measureInsertionTime()` - Insertion performance
- `measureDeletionTime()` - Deletion performance
- `measureSearchTime()` - Search operation timing
- `calculateTreeMetrics()` - Height and balance analysis

**Output:** Generates `comparison_analysis.txt` with detailed timing results.

### Interactive Menu

**File:** Menu.h

User-friendly command-line interface allowing:
- Single operation testing
- Batch file processing from Reddit dataset
- Real-time performance display
- Tree visualization

#### Configuration & Dataset Paths

Dataset file paths are configured at the top of Menu.h in the `MenuSystem` constructor:

```cpp
MenuSystem() 
    : csv_path("/media/daniyal/New Volume/DataSet/reddit_data.csv"),
      tgz_path("/media/daniyal/New Volume/DataSet/dataset.tgz"),
      timeLimit(1) {}
```

**To change dataset paths:**

1. Open [Menu.h](Menu.h) (around line 18-20)
2. Modify the file paths in the constructor:
   ```cpp
   csv_path("YOUR_PATH/reddit_data.csv")
   tgz_path("YOUR_PATH/dataset.tgz")
   ```
3. Save and recompile:
   ```bash
   g++ -std=c++17 -O2 main.cpp -o treap_bst -lzstd
   ./treap_bst
   ```

**Dataset Path Variables:**
- `csv_path`: Path to CSV dataset (3 GB) - fastest loading
- `tgz_path`: Path to compressed dataset (15 GB) - used for large-scale benchmarks
- `timeLimit`: Time limit in seconds for performance tests (adjustable)

---

## 📈 Results & Analysis

### Performance Characteristics

**When to use Treap:**
- ✅ Applications requiring balanced insertion-heavy workloads
- ✅ Systems with unpredictable data patterns
- ✅ When consistent O(log n) guarantees are critical
- ✅ Search-intensive applications

**When to use BST:**
- ✅ Deletion-heavy operations
- ✅ Applications with sorted/sequential input
- ✅ Memory-constrained environments
- ✅ Simpler codebase maintenance

### Detailed Comparison

See comparison_analysis.txt for complete benchmark results including:
- Per-operation timing breakdowns
- Memory usage statistics
- Tree structure metrics
- Scalability analysis

### Generating Custom Reports

Modify ComparisonAnalysis.h to:
- Change dataset sizes
- Adjust operation ratios
- Add custom metrics
- Generate specialized reports

---

## 🤝 Contributing

Contributions are welcome! Areas for enhancement:

- [ ] AVL Tree implementation for comparison
- [ ] Red-Black Tree variant
- [ ] Persistent data structure support
- [ ] Parallel operation benchmarking
- [ ] Additional visualization types
- [ ] Performance optimization techniques

To contribute:
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/enhancement`)
3. Commit changes (`git commit -am 'Add enhancement'`)
4. Push to branch (`git push origin feature/enhancement`)
5. Open a Pull Request

---

## 📄 License

This project is licensed under the **MIT License** - see LICENSE file for details.

```
MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
```

---

## 👤 Author

**Daniyal**
- Student ID: 23i-0579_F
- Project: Data Structures & Algorithms Analysis
- University Project

---

## 📚 References

### Data Structures
- [Treap - Wikipedia](https://en.wikipedia.org/wiki/Treap)
- [Binary Search Tree - Wikipedia](https://en.wikipedia.org/wiki/Binary_search_tree)
- [Randomized Data Structures](https://en.wikipedia.org/wiki/Randomized_algorithm)

### Performance Analysis
- Introduction to Algorithms (CLRS)
- Algorithm Analysis and Big O Notation
- Empirical Performance Evaluation Methodologies

---

## 🙋 FAQ

**Q: Why is insertion so slow in BST?**  
A: Without explicit balancing, BSTs can develop deep unbalanced structures, increasing search depth during insertion.

**Q: How do I change the dataset path?**  
A: Edit the `csv_path` and `tgz_path` variables in Menu.h constructor (line 18-20), then recompile with: `g++ -std=c++17 -O2 main.cpp -o treap_bst -lzstd`

**Q: Which dataset version should I use?**  
A: For testing, use the 3GB CSV version (fastest). For comprehensive benchmarking, use the 15GB compressed version. The 200GB raw version is for intensive analysis.

**Q: Can I modify dataset sizes for benchmarking?**  
A: Yes! Edit the constants in ComparisonAnalysis.h to change test parameters.

**Q: How accurate are the timing measurements?**  
A: Timing uses `std::chrono::high_resolution_clock` with microsecond precision. Results are averaged over multiple runs.

**Q: Can this handle large datasets?**  
A: The BST has a safety limit of 150M nodes to prevent memory overflow. Both structures scale to millions of nodes.

**Q: What if I don't have the Reddit dataset?**  
A: You can use the Interactive Operations menu to manually add/test posts, or modify the dataset loading code in Menu.h to use your own data source.

---

<div align="center">

**⭐ If you found this helpful, please star the repository! ⭐**

Made with ❤️ for Data Structures Education

</div>
