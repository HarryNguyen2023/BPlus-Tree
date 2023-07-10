#include "BPlusTree.h"
#include <iostream>
#include <vector>

// Function to initiate the node in the B+ Tree
template <typename T>
BPlusNode<T>::BPlusNode(int _Max, bool _leaf)
{
    Max = _Max;
    leaf = _leaf;
    n = 0;
    // Allocate the memeory for 2 dynamic arrays in each node
    keys = new T[Max];
    ptr = new BPlusNode<T>*[Max + 1];
}

// Function to inititate the B+ Tree
template <typename T>
BPlusTree<T>::BPlusTree(int _Max)
{
    root = NULL;
    Max = _Max;
    size = 0;
}

// Function to search for a key in the B+ Tree
template <typename T>
bool BPlusTree<T>::searchTree(T data)
{
    // Check if the tree is empty
    if(root == NULL)
    {
        std::cout<<"The tree is empty!"<<std::endl;
        return false;
    }
    else
    {
        // Temporary pointer to traverse the tree
        BPlusNode<T>* temp = root;
        // If the current node is not a leaf node, traverse to the leaf node 
        while (temp->leaf == false)
        {
            for(int i = 0; i < temp->n; ++i)
            {
                if(data < temp->keys[i])
                {
                    temp = temp->ptr[i];
                    break;
                }
                // If traverse to the last child of the node
                if(i == (temp->n) - 1)
                {
                    temp = temp->ptr[i + 1];
                    break;
                }
            }
        }
        // Seach for the key in the leaf node
        for(int i = 0; i < temp->n; ++i)
        {
            if(data == temp->keys[i])
                return true;
        }
    }
    return false;
}

// Function to traverse the B+ Tree
template <typename T>
void BPlusTree<T>::traverse(BPlusNode<T> *node)
{
    // Check if the current node is empty
    if(node == NULL)
        return;
    // Display all the keys of the current node
    for(int i = 0; i < node->n; ++i)
        std::cout<<node->keys[i]<<" ";
    std::cout<<std::endl;
    // Traverse to the child pointers of the current node
    if(node->leaf == false)
    {
        for(int i = 0; i <= node->n; ++i)
            traverse(node->ptr[i]);
    }
}

// General function to traverse the B+ Tree
template <typename T>
void BPlusTree<T>::traverse()
{
    // Check if the tree is empty
    if(root == NULL)
    {
        std::cout<<"The tree is empty!"<<std::endl;
        return;
    }
    traverse(root);
}

// Function to find the parent of the child node traverse from the current node
template <typename T>
BPlusNode<T>* BPlusTree<T>::findParent(BPlusNode<T>* current, BPlusNode<T>* child)
{
    BPlusNode<T>* parent;
    // If the current node is already at leaf
    if(current->leaf || current->ptr[0]->leaf)
        return NULL;
    // Sequentially traverse the pointers of the current node to find the parent of the child node
    for(int i = 0; i <= current->n; ++i)
    {
        if(current->ptr[i] == child)
        {
            parent= current;
            return parent;
        }
        else
        {
            parent = findParent(current->ptr[i], child);
            if(parent != NULL)
                return parent;
        }
    }
    return parent;
}

// Function to generally insert a new key into the tree at the leaf node
template <typename T>
void BPlusTree<T>::insertNode(T data)
{
    // Check if the tree is empty
    if(root == NULL)
    {
        root = new BPlusNode<T>(Max, true);
        root->keys[0] = data;
        root->n = 1;
    }
    else
    {
        // Traverse to the leaf node to insert the new key
        BPlusNode<T>* current = root;
        BPlusNode<T>* parent;

        while(current->leaf == false)
        {
            parent = current;
            for(int i = 0; i < current->n + 1; ++i)
            {
                if(data < current->keys[i])
                {
                    current = current->ptr[i];
                    break;
                }
                // When pointer point to the last child of the current node
                if(i == current->n - 1)
                {
                    current = current->ptr[i + 1];
                    break;
                }
            }
        }
        // Now we are at the leaf node to insert the new key
        // Case 1: the size of the node is smaller than maximum 
        if(current->n < Max)
        {
            int i = 0;
            while (i < current->n && data > current->keys[i])
                ++i;
            // Move all the keys at the right one step ahead
            for(int j = i; j < current->n; ++j)
                current->keys[j + 1] = current->keys[j];
            // Insert the new key at that position
            current->keys[i] = data;
            // Update the size of the current node
            current->n += 1;
            // Chanage the position of the linked list pointer to the new last child
            current->ptr[current->n] = current->ptr[current->n - 1];
            current->ptr[current->n - 1] = NULL; 
        }
        // Case 2: the leaf node is already full, split it and merge one node to the parent node
        else
        {
            // Create a new leaf node
            BPlusNode<T>* newleaf = new BPlusNode<T>(Max, true);
            // Create a temporary array to store the keys of the current node
            std::vector<T> tempNode(Max + 1);
            // Move all the keys of the current node into the temporary array
            for(int i = 0; i < Max; ++i)
                tempNode[i] = current->keys[i];
            // Find the position of the key to be inserted
            int i = 0;
            while(i < Max && data > tempNode[i])
                i++;
            // Move all the key at the right of the position one step ahead
            for(int j = i; j < Max; ++j)
                tempNode[j + 1] = tempNode[j];
            // Insert the key at that position 
            tempNode[i] = data;
            // Update the size of the current as well as the new leaf node
            current->n = (Max + 1) / 2;
            newleaf->n = (Max + 1) - current->n;
            // Update the poiter of both the current and the new leaf
            current->ptr[current->n] = newleaf;
            newleaf->ptr[newleaf->n] = current->ptr[Max];
            current->ptr[newleaf->n] = current->ptr[Max];
            current->ptr[Max] = NULL;
            // Keep (m - 1)/2 keys at the current node
            for(int i = 0; i < current->n; ++i)
                current->keys[i] = tempNode[i];
            // Move other keys to the new leaf node
            for(int i = 0; i < newleaf->n; ++i)
                newleaf->keys[i] = tempNode[i + current->n];

            // Case this is the root node, make the new root and increase the height of the tree
            if(current == root)
            {
                BPlusNode<T>* newroot = new BPlusNode<T>(Max, false);
                newroot->n = 1;
                // Assign the first key of new root to be the smallest key of new leaf node
                newroot->keys[0] = newleaf->keys[0];
                // Update the child of the new root node
                newroot->ptr[0] = current;
                newroot->ptr[1] = newleaf;
                root = newroot;
            }
            // Case call the node to insert key at the internal node
            else
            {
                insertInternal(newleaf->keys[0], parent, newleaf);
            }
        }
    }
}

// Function to insert a new key at the internal node in a B+ Tree
template <typename T>
void BPlusTree<T>::insertInternal(T data, BPlusNode<T>* current, BPlusNode<T>* child)
{
    // Case 1: The node has fewer than the maximum number of key
    if(current->n < Max)
    {
        // Find the position to insert the new key
        int i = 0;
        while(i < current->n && data > current->keys[i])
            i++;
        // Shift keys at the right of the position one step ahead
        for(int j = i; j < current->n; ++j)
            current->keys[j + 1] = current->keys[j];
        // Shift pointers at the right of the position one step ahead
        for(int j = i + 1; j <= current->n; ++j)
            current->ptr[j + 1] = current->ptr[j];
        // Insert new key and new child
        current->keys[i] = data;
        current->ptr[i + 1] = child;
        current->n += 1;
    }
    // Case 2: the internal node has maximum number of keys, split it and recursively call this function
    else
    {
        BPlusNode<T>* newInternal = new BPlusNode<T>(Max, false);
        // Create temporary arrays contain the keys and pointer of the current node before splitting
        std::vector<T> tempKey(Max + 1);
        BPlusNode<T>* tempPtr[Max + 2];

        // Move all the keys and pointer of the current node to the temporary arrays
        for(int i = 0; i < current->n; ++i)
            tempKey[i] = current->keys[i];
        for(int i = 0; i <= current->n; ++i)
            tempPtr[i] = current->ptr[i];

        // Find the position to insert the new key and new child into the temporary arrays
        int i = 0;
        while(i < Max && data > tempKey[i])
            i++;
        // Move all the keys and pointers at the right of the position one step ahead
        for(int j = i; j < Max; ++j)
            tempKey[j + 1] = tempKey[j];
        for(int j = i + 1; j <= Max; ++j)
            tempPtr[j + 1] = tempPtr[j];
        
        // Insert the new key and new child at the corresponding position
        tempKey[i] = data;
        tempPtr[i + 1] = child;

        // Update the size of the 2 nodes
        current->n = (Max + 1) / 2;
        newInternal->n = Max - (Max + 1) / 2;

        // Move the haft larger keys and pointer to the new internal node
        for(int j = 0; j < newInternal->n; ++j)
            newInternal->keys[j] = tempKey[j + (current->n + 1)];
        for(int j = 0; j <= newInternal->n; ++j)
            newInternal->ptr[j] = tempPtr[j + (current->n + 1)];
        
        // Case the current node is the root node, create a new root and change the height of the tree
        if(current == root)
        {
            BPlusNode<T>* newRoot = new BPlusNode<T>(Max, false);
            newRoot->n = 0;
            newRoot->keys[0] = current->keys[current->n];
            newRoot->ptr[0] = current;
            newRoot->ptr[1] = newInternal;
            root = newRoot;
        }
        // Or continue recuresively call this function
        {
            insertInternal(current->keys[current->n], findParent(root, current), newInternal);
        }
    }
}


int main()
{
    // Initiate the B+ tree
    BPlusTree<int> bplustree(3);
    // Add some new keys into the tree
    int random[] = {5, 7, 9, 3, 2, 15, 25, 33, 21, 48, 30, 109, 67, 78, 55, 13, 100};
    for(int i = 0; i < sizeof(random)/sizeof(int); ++i)
        bplustree.insertNode(random[i]);
    // Traverse the tree
    bplustree.traverse();
    // Search for a few node inside the tree
    std::cout<<"Node 25 is "<<(bplustree.searchTree(25) ? "" : "not ")<<"in the tree"<<std::endl;
    std::cout<<"Node 48 is "<<(bplustree.searchTree(48) ? "" : "not ")<<"in the tree"<<std::endl;
    std::cout<<"Node 66 is "<<(bplustree.searchTree(66) ? "" : "not ")<<"in the tree"<<std::endl;

    return 0;
}
