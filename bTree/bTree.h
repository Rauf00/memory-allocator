#ifndef _BTREE_H_
#define _BTREE_H_
#include <stdbool.h>

typedef enum NodeState_e NodeState;
enum NodeState_e {
    FREE,
    PARTIAL,
    FULL,
};

typedef struct BTree_node_s BTreeNode;
struct BTree_node_s {
    NodeState state; // 4 byte
    size_t size; // 8 bytes
    void* memAddr; // 8 bytes
    BTreeNode* left; // 8 bytes
    BTreeNode* right; // 8 bytes
    BTreeNode* parent; // 8 bytes
    bool isParent;
};

typedef struct BTree_s BTree;
struct BTree_s {
    BTreeNode* root;
    size_t biggestBlockSize;
};

// Initialize the tree by adding a node to the tree with specified size
BTree* BTree_create(size_t size, void* memAddr);

// Splits a node into two nodes of size node->size/2
void BTree_splitNode(BTreeNode* node);

// Removes two siblings and marks their parent as FREE 
BTreeNode* BTree_mergeNodes(BTreeNode* left, BTreeNode* right);

// Clean up BTree on program exit
void BTree_cleanup(BTreeNode* root);

// Prints a tree for debugging
// See how to interpet results here https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
void print2D(BTreeNode *root);

#endif