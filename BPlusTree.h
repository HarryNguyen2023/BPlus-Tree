// Forward declaration of the BPusTree class
template <class T>
class BPlusTree;

// Class to define the node of a B+ Tree
template <class T>
class BPlusNode
{
    private:
    bool leaf;
    T* keys;
    int Max, n;
    BPlusNode<T>** ptr;

    public:
    BPlusNode(int _Max, bool _leaf);
    // Declare BPlusTree as the friend class
    friend class BPlusTree<T>;
};

template <class T>
class BPlusTree
{
    private:
    BPlusNode<T>* root;
    int Max, size;

    public:
    BPlusTree(int _Max);
    bool searchTree(T data);
    void traverse(BPlusNode<T>* node);
    void traverse();
};