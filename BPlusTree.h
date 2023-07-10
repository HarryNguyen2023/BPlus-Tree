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
    T getPred(BPlusNode<T>* node);
    T getSucc(BPlusNode<T>* node);
    void borrowFromLeft(BPlusNode<T>* parent, int index);
    void borrowFromRight(BPlusNode<T>* parent, int index);
    void merge(BPlusNode<T>* parent, int index, BPlusNode<T>* pred, BPlusNode<T>* succ);
    void rightRotate(BPlusNode<T>* parent, int index);
    void leftRotate(BPlusNode<T>* parent, int index);
    void delFromLeaf(int index);
    // Declare BPlusTree as the friend class
    friend class BPlusTree<T>;
};

template <class T>
class BPlusTree
{
    private:
    BPlusNode<T>* root;
    int Max;
    long long size;

    void traverse(BPlusNode<T>* node);
    void insertInternal(T data, BPlusNode<T>* current, BPlusNode<T>* child);
    BPlusNode<T>* findParent(BPlusNode<T>* current, BPlusNode<T>* child);
    bool delNode(BPlusNode<T>* node, T data);

    public:
    BPlusTree(int _Max);
    bool searchTree(T data);
    void traverse();
    void insertNode(T data);
    long long getSize();
    void delNode(T data);
};