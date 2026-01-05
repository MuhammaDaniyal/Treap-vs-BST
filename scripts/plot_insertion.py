import matplotlib.pyplot as plt
import sys
import os

# Ensure we can import from parent directory if needed
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

if __name__ == "__main__":
    args = sys.argv[1:]  # Get command line arguments
    
    # Parse arguments
    sizes = [int(x) for x in args[:4]]  # First 4 are sizes
    bst_times = [float(x) for x in args[4:8]]  # Next 4 are BST times
    treap_times = [float(x) for x in args[8:]]  # Last 4 are Treap times
    
    # Create plot
    plt.figure(figsize=(10, 6))
    plt.plot(sizes, bst_times, 'ro-', label='BST', linewidth=2)
    plt.plot(sizes, treap_times, 'bo-', label='Treap', linewidth=2)
    
    plt.xlabel('Number of Posts')
    plt.ylabel('Insertion Time (ms)')
    plt.title('BST vs Treap - Insertion Performance')
    plt.legend()
    plt.grid(True)

    os.makedirs('graphs', exist_ok=True)
    plt.savefig('graphs/insertion_performance.png', dpi=300, bbox_inches='tight')
    print("✅ Insertion graph saved as: ../graphs/insertion_performance.png")

    plt.show()