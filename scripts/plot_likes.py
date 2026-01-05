import matplotlib.pyplot as plt
import sys
import os

if __name__ == "__main__":
    args = sys.argv[1:]
    
    # Parse arguments
    bst_single_like = float(args[0])
    treap_single_like = float(args[1])
    bst_multiple_likes = float(args[2])
    treap_multiple_likes = float(args[3])
    total_rotations = int(args[4])
    bubble_rotations = int(args[5])
    
    # Create subplots
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))
    
    # Left: Like times comparison
    operations = ['Single Like', 'Multiple Likes']
    bst_times = [bst_single_like, bst_multiple_likes]
    treap_times = [treap_single_like, treap_multiple_likes]
    
    x_pos = range(len(operations))
    width = 0.35
    
    ax1.bar([x - width/2 for x in x_pos], bst_times, width, label='BST', color='red', alpha=0.7)
    ax1.bar([x + width/2 for x in x_pos], treap_times, width, label='Treap', color='blue', alpha=0.7)
    ax1.set_xlabel('Like Operations')
    ax1.set_ylabel('Time (microseconds)')
    ax1.set_title('BST vs Treap - Like Performance')
    ax1.set_xticks(x_pos)
    ax1.set_xticklabels(operations)
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Right: Rotations (Treap only)
    rotation_types = ['Total Rotations', 'Bubble Rotations']
    rotation_counts = [total_rotations, bubble_rotations]
    
    bars = ax2.bar(rotation_types, rotation_counts, color=['orange', 'green'], alpha=0.7)
    ax2.set_ylabel('Number of Rotations')
    ax2.set_title('Treap Rotations During Likes')
    ax2.grid(True, alpha=0.3)
    
    # Add value labels on bars
    for bar in bars:
        height = bar.get_height()
        ax2.text(bar.get_x() + bar.get_width()/2., height,
                f'{int(height)}', ha='center', va='bottom')
    
    plt.tight_layout()
    
    os.makedirs('graphs', exist_ok=True)
    plt.savefig('graphs/likes_performance.png', dpi=300, bbox_inches='tight')
    print("✅ Insertion graph saved as: ../graphs/likes_performance.png")

    plt.show()