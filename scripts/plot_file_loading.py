import matplotlib.pyplot as plt
import sys
import os

if __name__ == "__main__":
    args = sys.argv[1:]
    
    # Parse arguments
    bst_csv_posts = int(args[0])
    treap_csv_posts = int(args[1])
    bst_tgz_posts = int(args[2])
    treap_tgz_posts = int(args[3])
    bst_csv_height = int(args[4])
    treap_csv_height = int(args[5])
    bst_tgz_height = int(args[6])
    treap_tgz_height = int(args[7])
    
    # Create subplots
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
    
    # Left: Posts loaded comparison
    categories = ['BST-CSV', 'Treap-CSV', 'BST-TGZ', 'Treap-TGZ']
    posts = [bst_csv_posts, treap_csv_posts, bst_tgz_posts, treap_tgz_posts]
    colors = ['red', 'blue', 'red', 'blue']
    
    bars1 = ax1.bar(categories, posts, color=colors, alpha=0.7)
    ax1.set_ylabel('Posts Loaded')
    ax1.set_title('Posts Loaded from Different Sources')
    ax1.grid(True, alpha=0.3)
    ax1.tick_params(axis='x', rotation=45)
    
    # Add value labels on bars
    for bar in bars1:
        height = bar.get_height()
        ax1.text(bar.get_x() + bar.get_width()/2., height,
                f'{height:,}', ha='center', va='bottom', fontsize=9)
    
    # Right: Tree height comparison
    heights = [bst_csv_height, treap_csv_height, bst_tgz_height, treap_tgz_height]
    
    bars2 = ax2.bar(categories, heights, color=colors, alpha=0.7)
    ax2.set_ylabel('Tree Height')
    ax2.set_title('Tree Height After Loading')
    ax2.grid(True, alpha=0.3)
    ax2.tick_params(axis='x', rotation=45)
    
    # Add value labels on bars
    for bar in bars2:
        height = bar.get_height()
        ax2.text(bar.get_x() + bar.get_width()/2., height,
                f'{height}', ha='center', va='bottom', fontsize=9)
    
    plt.tight_layout()

    os.makedirs('graphs', exist_ok=True)
    plt.savefig('graphs/file_loading_performance.png', dpi=300, bbox_inches='tight')
    print("✅ Insertion graph saved as: ../graphs/file_loading_performance.png")
    
    plt.show()