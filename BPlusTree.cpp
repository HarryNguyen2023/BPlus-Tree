#include "BPlusTree.h"
#include <iostream>

// Function to initiate the node in the B+ Tree
template <typename T>
BPlusNode<T>::BPlusNode(int _Max, bool _leaf)
{
    Max = _Max;
    leaf = _leaf;
    n = 0;
    // Allocate the memeory for 2 dynamic arrays in each node
    keys = new T[Max];
    ptr = new BPlusNode<T>*[Max + 1]];
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

