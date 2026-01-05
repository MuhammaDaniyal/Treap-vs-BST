import matplotlib.pyplot as plt
import sys
import os

if __name__ == "__main__":
    args = sys.argv[1:]
    
    # Parse arguments
    file_type = args[0]  # "CSV" or "TGZ"
    bst_time = float(args[1])
    treap_time = float(args[2])
    bst_height = int(args[3])
    treap_height = int(args[4])
    
    # Create subplots
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))
    
    # Left: Loading time comparison
    structures = ['BST', 'Treap']
    times = [bst_time, treap_time]
    
    bars1 = ax1.bar(structures, times, color=['red', 'blue'], alpha=0.7)
    ax1.set_ylabel('Loading Time (seconds)')
    ax1.set_title(f'{file_type} Loading Time Comparison')
    ax1.grid(True, alpha=0.3)
    
    # Add value labels on bars
    for bar in bars1:
        height = bar.get_height()
        ax1.text(bar.get_x() + bar.get_width()/2., height,
                f'{height:.1f}s', ha='center', va='bottom')
    
    # Right: Tree height comparison
    heights = [bst_height, treap_height]
    
    bars2 = ax2.bar(structures, heights, color=['red', 'blue'], alpha=0.7)
    ax2.set_ylabel('Tree Height')
    ax2.set_title(f'Tree Height After {file_type} Loading')
    ax2.grid(True, alpha=0.3)
    
    # Add value labels on bars
    for bar in bars2:
        height = bar.get_height()
        ax2.text(bar.get_x() + bar.get_width()/2., height,
                f'{height}', ha='center', va='bottom')
    
    plt.tight_layout()

    os.makedirs('graphs', exist_ok=True)
    plt.savefig('graphs/loading_performance.png', dpi=300, bbox_inches='tight')
    print("✅ Insertion graph saved as: ../graphs/loading_performance.png")

    plt.show()