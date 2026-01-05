#include "Menu.h"

void test_tescases_bst()
{
    BinarySearchTree bst;

    bst.addPost("ejualnb", 1554076800, 55);
    bst.addPost("ejualnc", 1554076800, 12);
    bst.addPost("ejualnd", 1554076800, 27);
    bst.addPost("ejualne", 1554076800, 14);
    bst.addPost("ejualnl", 1554076809, 13);

    cout << "BST Most Popular: " << bst.getMostPopular() << endl;
 
    bst.printVerticalStructure();    cout << endl;
    bst.likePost("ejualnl");
    bst.likePost("ejualnl");

    cout << "BST Most Popular: " << bst.getMostPopular() << endl;

    bst.printVerticalStructure();  cout << endl;
    
    bst.deletePost("ejualnc");
    
    cout << "BST Most Popular: " << bst.getMostPopular() << endl;
    
    bst.printVerticalStructure();  cout << endl;

}

void test_tescases_treap()
{
    Treap treap;

    treap.addPost("ejualnb", 1554076800, 55);
    treap.addPost("ejualnc", 1554076800, 12);
    treap.addPost("ejualnd", 1554076800, 27);
    treap.addPost("ejualne", 1554076800, 14);
    treap.addPost("ejualnl", 1554076809, 13);

    cout << "Treap Most Popular: " << treap.getMostPopular() << endl;
 
    treap.printTreapStructure();    cout << endl;

    treap.likePost("ejualnl");
    treap.likePost("ejualnl");

    cout << "Treap Most Popular: " << treap.getMostPopular() << endl;

    treap.printTreapStructure();  cout << endl;
    
    treap.deletePost("ejualnc");
    
    cout << "Treap Most Popular: " << treap.getMostPopular() << endl;
    
    treap.printTreapStructure();  cout << endl;
}

int main() {
    
    MenuSystem menu;
    // menu.run();

    ComparisonAnalysis analysis;

    Treap treap;
    // treap.loadFromCSVWithTimeout("/media/daniyal/New Volume/DataSet/reddit_data.csv", 60);
    // treap.loadFromCSV("/media/daniyal/New Volume/DataSet/reddit_data.csv");

    // treap.getMostRecent()
    // cout << treap.getMostPopular() << endl;
    // treap.deletePost("ejualo4");
    // cout << treap.getMostPopular() << endl;
    // treap.addPost("Daniyal", 10000, 200);



    // treap.loadFromCSVWithTimeout("/media/daniyal/New Volume/DataSet/reddit_data.csv", );
    

    // treap.loadFromTGZ("/media/daniyal/New Volume/DataSet/dataset.tgz");

    // BinarySearchTree bst;
    // bst.loadFromCSVWithTimeout("/media/daniyal/New Volume/DataSet/reddit_data.csv", 60);

    // analysis.testCSVLoading("/media/daniyal/New Volume/DataSet/reddit_data.csv");
    // analysis.testTGZLoading("/media/daniyal/New Volume/DataSet/dataset.tgz");
    // analysis.runFinalComprehensiveAnalysis();
    // analysis.loadingFileAnalysis(1, "/media/daniyal/New Volume/DataSet/reddit_data.csv", "/media/daniyal/New Volume/DataSet/dataset.tgz");

    // analysis.testCSVLoading("/media/daniyal/New Volume/DataSet/reddit_data.csv");
    // analysis.testTGZLoading("/media/daniyal/New Volume/DataSet/dataset.tgz");
    test_tescases_treap();
    test_tescases_bst();

    return 0;
}