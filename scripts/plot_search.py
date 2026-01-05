import matplotlib.pyplot as plt
import sys
import os

if __name__ == "__main__":
    args = sys.argv[1:]
    
    # Parse arguments - different structure than insertion!
    operation_names = ["getMostPopular", "getMostRecent(10)"]
    bst_times = [float(x) for x in args[:2]]      # First 2 are BST times
    treap_times = [float(x) for x in args[2:]]    # Next 2 are Treap times
    
    # Create bar chart
    x_pos = range(len(operation_names))
    width = 0.35
    
    plt.figure(figsize=(10, 6))
    plt.bar([x - width/2 for x in x_pos], bst_times, width, label='BST', color='red', alpha=0.7)
    plt.bar([x + width/2 for x in x_pos], treap_times, width, label='Treap', color='blue', alpha=0.7)
    
    plt.xlabel('Search Operations')
    plt.ylabel('Time (microseconds)')
    plt.title('BST vs Treap - Search Performance')
    plt.xticks(x_pos, operation_names)
    plt.legend()
    plt.grid(True, alpha=0.3)

    os.makedirs('graphs', exist_ok=True)
    plt.savefig('graphs/search_performance.png', dpi=300, bbox_inches='tight')
    print("✅ Insertion graph saved as: ../graphs/search_performance.png")

    
    plt.show()