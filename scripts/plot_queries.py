import matplotlib.pyplot as plt
import sys
import os

if __name__ == "__main__":
    args = sys.argv[1:]
    
    # Parse arguments
    bst_single_popular = float(args[0])
    treap_single_popular = float(args[1])
    bst_mixed = float(args[2])
    treap_mixed = float(args[3])
    k_values = [int(x) for x in args[4:8]]  # k values: 5, 10, 20, 50
    bst_recent_times = [float(x) for x in args[8:12]]  # BST times for each k
    treap_recent_times = [float(x) for x in args[12:]]  # Treap times for each k
    
    # Create subplots
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
    
    # Left: Single operations comparison
    operations_single = ['getMostPopular', 'Mixed Workload']
    bst_single_times = [bst_single_popular, bst_mixed]
    treap_single_times = [treap_single_popular, treap_mixed]
    
    x_pos = range(len(operations_single))
    width = 0.35
    
    bars1 = ax1.bar([x - width/2 for x in x_pos], bst_single_times, width, 
                   label='BST', color='red', alpha=0.7)
    bars2 = ax1.bar([x + width/2 for x in x_pos], treap_single_times, width, 
                   label='Treap', color='blue', alpha=0.7)
    
    ax1.set_xlabel('Query Operations')
    ax1.set_ylabel('Time (microseconds)')
    ax1.set_title('Single Query Performance')
    ax1.set_xticks(x_pos)
    ax1.set_xticklabels(operations_single)
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Add value labels on bars
    for bars in [bars1, bars2]:
        for bar in bars:
            height = bar.get_height()
            ax1.text(bar.get_x() + bar.get_width()/2., height,
                    f'{height:.1f}μs', ha='center', va='bottom', fontsize=8)
    
    # Right: getMostRecent(k) performance
    ax2.plot(k_values, bst_recent_times, 'ro-', label='BST', linewidth=2, markersize=6)
    ax2.plot(k_values, treap_recent_times, 'bo-', label='Treap', linewidth=2, markersize=6)
    ax2.set_xlabel('Number of Recent Posts (k)')
    ax2.set_ylabel('Time per Query (microseconds)')
    ax2.set_title('getMostRecent(k) Performance')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    # Add value labels on points
    for i, (k, bst_time, treap_time) in enumerate(zip(k_values, bst_recent_times, treap_recent_times)):
        ax2.annotate(f'{bst_time:.1f}', (k, bst_time), textcoords="offset points", 
                    xytext=(0,10), ha='center', fontsize=8, color='red')
        ax2.annotate(f'{treap_time:.1f}', (k, treap_time), textcoords="offset points", 
                    xytext=(0,10), ha='center', fontsize=8, color='blue')
    
    plt.tight_layout()

    os.makedirs('graphs', exist_ok=True)
    plt.savefig('graphs/queries_performance.png', dpi=300, bbox_inches='tight')
    print("✅ Insertion graph saved as: ../graphs/queries_performance.png")

    plt.show()