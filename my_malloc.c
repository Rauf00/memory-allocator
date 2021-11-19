#include <stdio.h>
#include <stdlib.h>
#include "my_malloc.h"
#include "bTree/bTree.h"
#include "list/list.h"
#define MAX_SIZE 64
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
void* rec2MyMal( BTreeNode* node, size_t reqSize) {
    if(node->state == FULL || node->size == MAX_SIZE && begunSearch == true) {
        return NULL;
    }
    
    if(stored == 0) {
    }else if( reqSize <= (node->size/2)) {
        begunSearch = true;
        printf("We are here at %d trying to fit in %d\n\n", node->size, reqSize);
        if(node->isParent == false) {
            node->isParent = true;
            BTree_splitNode(node);
            
        }
        
        if(node->right->state != FULL) {
            printf("Going right!\n\n");
            ptr =rec2MyMal(node->right, reqSize);
        }
        if(node->left->state != FULL) {
            printf("Going left!\n\n");
            ptr = rec2MyMal(node->left, reqSize);
        }
        
    } else {
        printf("We have stored!\n\n");
        remainingSize = remainingSize - node->size;
        stored = 0;
        node->state = FULL;
        List_append(listOfAllocatedBlocks,node);
        printf("%d\n", node->memAddr);
        return (node->memAddr);
    }
    if(stored == 0) {
        //printf("Is %d size node a parent?\n", node->size);
        if(node->isParent == true) {
            if(node->left->state == FULL && node->right->state == FULL) {
                //printf("please\n");
                node->state = FULL;
            } else {
                //printf("this at least?\n");
                node->state = PARTIAL;
            }
            
        }
        
        return ptr;
    }
    return ptr;

}
    
void* my_malloc(size_t reqSize) {
    // Test BTree ops
    printf("Our remaining is: %d\n", remainingSize);
    if(reqSize<1|| reqSize>remainingSize) {
        printf("Cannot handle request!\n");
        return NULL;
    }
    
    begunSearch = false;
    BTreeNode* root = bTree->root;
    ptr = NULL;
    stored = 1;
    if(rec2MyMal(root,reqSize) == NULL) {
        printf("Cannot handle request!\n");
        return NULL;
    }
    
    return ptr;
}

// For searching
bool memAddrEquals(BTreeNode* pItem, void* pArg) {
    if(pItem->memAddr == pArg) {
        printf("FOUND!!!\n\n");
    } else {
        printf("PROBLEM!!!\n\n");
    }
    return (pItem->memAddr == pArg);
}

// List stores nodes with node->val = pointer to our allocated memory block
void my_free(void* ptr) {
    List_first(listOfAllocatedBlocks);
    BTreeNode* pBlock = (BTreeNode*) List_search(listOfAllocatedBlocks, memAddrEquals, ptr); ;
    remainingSize = remainingSize + pBlock->size;
    pBlock->state = FREE;
    BTreeNode* sibling;
    BTreeNode* parentNode = pBlock->parent;
    BTreeNode* newerNode = pBlock;
    if(parentNode->left) {
        sibling = parentNode->left;
    } else {
        sibling = parentNode->right;
    }
    while(sibling != NULL && sibling->state == FREE) {
        newerNode = BTree_mergeNodes(newerNode, sibling);
        newerNode->isParent = false;
        if(newerNode->left) {
            sibling = newerNode->left;
        } else {
            sibling = newerNode->right;
        }
    }
    while(newerNode ->parent != NULL &&
    newerNode->parent->state == FULL && 
    (sibling->state != FREE)) {
        newerNode->parent->state = PARTIAL;
        newerNode = newerNode->parent;
    }
    List_remove(listOfAllocatedBlocks);
}
void* my_malloc_cleanup() {
    free(initBlock);
    BTree_cleanup(bTree->root);
    free(bTree);
}
