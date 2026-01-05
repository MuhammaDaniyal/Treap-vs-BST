import matplotlib.pyplot as plt
import sys
import os

print("🔧 Python script started!")
print(f"Arguments received: {len(sys.argv)}")
print(f"Args: {sys.argv[1:]}")

if __name__ == "__main__":
    args = sys.argv[1:]
    
    try:
        # Parse arguments
        sizes = [int(x) for x in args[:2]]  # First 2 are sizes (1000, 5000)
        bst_times = [float(x) for x in args[2:4]]  # Next 2 are BST times
        treap_times = [float(x) for x in args[4:6]]  # Next 2 are Treap times
        bst_heights = [int(x) for x in args[6:8]]  # Next 2 are BST heights
        treap_heights = [int(x) for x in args[8:10]]  # Next 2 are Treap heights
        rotations = [int(x) for x in args[10:]]  # Last 2 are rotations
        
        print(f"Sizes: {sizes}")
        print(f"BST Times: {bst_times}")
        print(f"Treap Times: {treap_times}")
        print(f"BST Heights: {bst_heights}")
        print(f"Treap Heights: {treap_heights}")
        print(f"Rotations: {rotations}")
        
        # Create subplots
        fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(15, 5))
        
        # Left: Deletion time comparison
        ax1.plot(sizes, bst_times, 'ro-', label='BST', linewidth=2, markersize=8)
        ax1.plot(sizes, treap_times, 'bo-', label='Treap', linewidth=2, markersize=8)
        ax1.set_xlabel('Dataset Size (posts)')
        ax1.set_ylabel('Deletion Time (ms)')
        ax1.set_title('Deletion Time vs Dataset Size')
        ax1.legend()
        ax1.grid(True, alpha=0.3)
        
        # Middle: Tree height comparison
        ax2.plot(sizes, bst_heights, 'ro-', label='BST Height', linewidth=2, markersize=8)
        ax2.plot(sizes, treap_heights, 'bo-', label='Treap Height', linewidth=2, markersize=8)
        ax2.set_xlabel('Dataset Size (posts)')
        ax2.set_ylabel('Tree Height')
        ax2.set_title('Tree Height vs Dataset Size')
        ax2.legend()
        ax2.grid(True, alpha=0.3)
        
        # Right: Treap rotations
        ax3.bar(['1000 posts', '5000 posts'], rotations, color=['orange', 'purple'], alpha=0.7)
        ax3.set_ylabel('Number of Rotations')
        ax3.set_title('Treap Rotations During Deletion')
        ax3.grid(True, alpha=0.3)
        
        # Add value labels on bars
        for i, v in enumerate(rotations):
            ax3.text(i, v + max(rotations)*0.01, str(v), ha='center', va='bottom')
        
        os.makedirs('graphs', exist_ok=True)
        plt.savefig('graphs/deletion_performance.png', dpi=300, bbox_inches='tight')
        print("✅ Insertion graph saved as: ../graphs/deletion_performance.png")

        plt.tight_layout()
        print("✅ Graph created successfully! Showing...")
        plt.show()
        print("✅ Graph closed!")
        
    except Exception as e:
        print(f"❌ Error: {e}")
        import traceback
        print(f"❌ Traceback: {traceback.format_exc()}")