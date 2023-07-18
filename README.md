# BPlus-Tree
## Author Harry Nguyen (Created 07/15/2023)

B+ Tree is an advanced version of the B tree, which apply multilevel indexing for faster searching in the database. Like B tree, B+ contains many keys in each node, to be more sutiable for hard disk acessing. However, different from B tree, the B+ one only contains actual data as the leaf node and the leaf nodes are linked together like a linked list, the internal nodes are just index layers, which a huge update in disk accessing time compare to the B tree.

B+ Tree is mostly applied in database indexing in disk-based storage systems.

Alike B Tree, B+ Tree has a drawbacks that it is very complicated in insertion and deletion algorithm.

### References
[1]. B+ Tree, https://www.programiz.com/dsa/b-plus-tree.

[2]. SaagnikAdhikary, Introductin of B+ Tree, https://www.geeksforgeeks.org/introduction-of-b-tree/.
