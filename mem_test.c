#include <stdio.h>
#include <stdlib.h>
#include "my_malloc.h"
#include "bTree/bTree.h"

// Note: 
// There is a function called print2D() in bTree.c file that was 
// copied from https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/ and used for debugging only
// It is NOT a part of our solution but it prints the binary tree in a neat way (using tabs).
// We were not sure if we are allowed to use code from external resources, so we've commented it out.
// If you want to print the state of the binary tree during algorithm execution, feel free to uncomment it.
// Printing the tree will make testing easier and might help to understand the algorithm better.

int main(int argc, char** args) {
    void* pInitBlock = mem_init();

    printf("\n----- TEST1: Allocate 32 bytes -----\n");
    int* test1 = (int*)my_malloc(32);
    if(test1 != NULL) {
        test1[0] = 9;
        test1[8] = 88;
        printf("\n----- TEST1: Free 32 bytes -----\n");
        my_free(test1);
    }

    printf("\n----- TEST2: Allocate too many bytes -----\n");
    int* test2 = (int*)my_malloc(1024);
    if(test2 != NULL) {
        test2[0] = 7;
    }

    printf("\n----- TEST3: Allocate 32, 32, 128 bytes -----\n");
    int* test31 = (int*)my_malloc(32);
    int* test32 = (int*)my_malloc(32);
    int* test33 = (int*)my_malloc(128);
    if (test31 != NULL && 
    test32 != NULL &&
    test33 != NULL
    ) {
        test31[0] = 1;
        test32[0] = 2;
        test33[0] = 3;
        printf("\n----- TEST3: Free 32, 32, 128 bytes -----\n");
        my_free(test32);
        my_free(test33);
        my_free(test31);
    }

    printf("\n----- TEST4: Allocate 32, 64, 32, 16, 32 bytes -----\n");
    int* test41 = (int*)my_malloc(32);
    int* test42 = (int*)my_malloc(64);
    int* test43 = (int*)my_malloc(32);
    int* test44 = (int*)my_malloc(16);
    int* test45 = (int*)my_malloc(32);
    if (test41 != NULL && 
    test42 != NULL &&
    test43 != NULL &&
    test44 != NULL &&
    test45 != NULL
    ) {
        test41[0] = 1;
        test42[0] = 2;
        test43[0] = 3;
        test44[0] = 4;
        test45[0] = 5;
        printf("\n----- TEST4: Free 64, 16 (create \"holes\" in memory) bytes -----\n");
        my_free(test42);
        my_free(test44);
    }
    printf("\n----- TEST4: Allocate 64 bytes to a \"hole\" of appropriate size -----\n");
    int* test46 = (int*)my_malloc(64);

    my_malloc_cleanup();
    return 0;
}