#include <stdio.h>
#include <stdlib.h>
#include "my_malloc.h"
#include "bTree/bTree.h"
#include "list/list.h"

#define MAX_SIZE 256

static BTree* bTree;
static void* initBlock;
static List* listOfAllocatedBlocks;
static int stored;
static void* ptr;
static size_t remainingSize;
static bool begunSearch;

void* mem_init(){
    remainingSize = MAX_SIZE;
    initBlock = malloc(MAX_SIZE);
    bTree = BTree_create(MAX_SIZE, initBlock);
    listOfAllocatedBlocks = List_create();
    return initBlock;
}

static void* rec2MyMal(BTreeNode* node, size_t reqSize) {
    if (node->state == FULL || node->size == MAX_SIZE && begunSearch == true) {
        return NULL;
    }
    if (stored == 0) {
    } else if (reqSize <= (node->size/2)) {
        begunSearch = true;
        printf("We are here at %ld trying to fit in %ld\n", node->size, reqSize);
        if (node->isParent == false) {
            node->isParent = true;
            BTree_splitNode(node);
        }
        if (node->right->state != FULL) {
            printf("Going right!\n");
            ptr =rec2MyMal(node->right, reqSize);
        }
        if(node->left->state != FULL) {
            printf("Going left!\n");
            ptr = rec2MyMal(node->left, reqSize);
        }
    } else {
        if (node->state == FREE) {
            printf("We've found a node of appropriate size and stored data!\n");
            remainingSize = remainingSize - node->size;
            stored = 0;
            node->state = FULL;
            List_append(listOfAllocatedBlocks,node);
            printf("Pointer to the first memory location of the block: %p\n", node->memAddr);
            return (node->memAddr);
        }
    }
    if (stored == 0) {
        if (node->isParent == true) {
            if (node->left->state == FULL && node->right->state == FULL) {
                node->state = FULL;
            } else {
                node->state = PARTIAL;
            }
        }
        return ptr;
    }
    return ptr;
}
    
void* my_malloc(size_t reqSize) {
    printf("Our remaining is: %ld\n", remainingSize);
    if (reqSize < 1|| reqSize > remainingSize) {
        printf("Cannot handle request!\n");
        return NULL;
    }
    begunSearch = false;
    BTreeNode* root = bTree->root;
    ptr = NULL;
    stored = 1;
    if (rec2MyMal(root, reqSize) == NULL) {
        printf("Cannot handle request!\n");
        return NULL;
    }
    printf("\n*** Current Binary Tree state ****");
    print2D(bTree->root);
    printf("\n");
    return ptr;
}

// For searching
static bool memAddrEquals(void* pCurrent, void* pArg) {
    BTreeNode* pItem = pCurrent;
    return (pItem->memAddr == pArg);
}

void my_free(void* ptr) {
    printf("Searching for the block to be freed with memAddr %p...", ptr);
    List_first(listOfAllocatedBlocks);
    BTreeNode* pBlock = (BTreeNode*) List_search(listOfAllocatedBlocks, memAddrEquals, ptr);
    if (pBlock) {
        printf("Block was found!\n");
    } else {
        printf("ERROR: Block was NOT found!\n");
    }
    remainingSize = remainingSize + pBlock->size;
    pBlock->state = FREE;
    
    BTreeNode* sibling;
    BTreeNode* parentNode = pBlock->parent;
    BTreeNode* newerNode = pBlock;
    if (parentNode->left && parentNode->left != pBlock) { // if there is left child and I'm not left, then left child is my sibling
        sibling = parentNode->left;
    } else {
        sibling = parentNode->right;
    }
    while (sibling != NULL && sibling->state == FREE) {
        newerNode = BTree_mergeNodes(newerNode, sibling);
        newerNode->isParent = false;
        if (newerNode == bTree->root) { // this solves seg fault
            break;
        }
        if (newerNode->parent->left && newerNode->parent->left != newerNode) {
            sibling = newerNode->parent->left;
        } else {
            sibling = newerNode->parent->right;
        }
    }
    while (newerNode->parent != NULL && newerNode->parent->state == FULL && (sibling->state != FREE)) {
        newerNode->parent->state = PARTIAL;
        newerNode = newerNode->parent;
    }
    List_remove(listOfAllocatedBlocks);
    printf("Block at %p was freed!\n", ptr);
    printf("\n*** Current Binary Tree state ****");
    print2D(bTree->root);
    printf("\n");
}

void* my_malloc_cleanup() {
    free(initBlock);
    BTree_cleanup(bTree->root);
    free(bTree);
}
