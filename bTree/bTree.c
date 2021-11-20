#include <stdio.h>
#include <stdlib.h>
#include "bTree.h"

#define COUNT 10

// Helper fuction to create a node
static void* createNode(size_t size, void* memAddr, BTreeNode* left, BTreeNode* right, BTreeNode* parent) {
    size_t memSize = sizeof(size_t) + sizeof(void*) + 3 * sizeof(BTreeNode*) + sizeof(NodeState);
    BTreeNode* newNode = malloc(memSize + 5); // TO DO: find out why extra 4 bytes of mem are needed  //when adding is parent with +4 i had valgrind error, i changed to + 5 no error
    newNode->state = FREE;
    newNode->size = size;
    newNode->memAddr = memAddr;
    newNode->left = left;
    newNode->right = right;
    newNode->parent = parent;
    newNode->isParent = false;
    return newNode;
}

// Helper fuction to delete a node
static void deleteNode(BTreeNode* node) {
    node->parent = NULL;
    free(node);
}

static void printNode(BTreeNode* node) {
    printf("state: %d, size: %ld, memAddr: %p, left: %p, right: %p, parent: %p\n",
            node->state, node->size, node->memAddr, node->left, node->right, node->parent);
}

static void print2DUtil(BTreeNode* root, int space)
{
    if (root == NULL){
        return;
    }
    space += COUNT;
    print2DUtil(root->right, space);
    printf("\n");
    for (int i = COUNT; i < space; i++){
        printf(" ");
    }
    printNode(root); 
    print2DUtil(root->left, space);
}

// Prints a tree for debugging
// See how to interpet results here https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
void print2D(BTreeNode *root)
{
   print2DUtil(root, 0);
}

BTree* BTree_create(size_t size, void* memAddr) {
    size_t memSize = sizeof(BTreeNode*) + sizeof(size_t);
    BTree* bTree = malloc(memSize); 
    BTreeNode* root = createNode(size, memAddr, NULL, NULL, NULL);
    bTree->biggestBlockSize = size;
    bTree->root = root;
    return bTree;
}

void BTree_splitNode(BTreeNode* node) {
    size_t childSize = (node->size) / 2;
    BTreeNode* right = createNode(childSize, node->memAddr, NULL, NULL, node);
    BTreeNode* left = createNode(childSize, (node->memAddr) + childSize, NULL, NULL, node);
    node->right = right;
    node->left = left;
}

BTreeNode* BTree_mergeNodes(BTreeNode* left, BTreeNode* right) {
    BTreeNode* parent = right->parent;
    parent->state = FREE;
    parent->left = NULL;
    parent->right = NULL;
    printf("Buddies of size %d and %d are merged! Parent's state: %d, right: %p, left %p\n", (int)left->size, (int)right->size, parent->state, parent->right, parent->left);
    deleteNode(left);
    deleteNode(right);
    return parent;
}

void BTree_cleanup(BTreeNode* root) {
    if (root == NULL) {
        return;
    }
    BTree_cleanup(root->left);
    BTree_cleanup(root->right);
    free(root);
}