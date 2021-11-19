#include <stdio.h>
#include <stdlib.h>
#include "my_malloc.h"
#include "bTree/bTree.h"

int main(int argc, char** args) {
    void* pInitBlock = mem_init();
    //void* test = my_malloc(32);
    //int* testInt = (int*)test;
    int* testInt = (int*)my_malloc(32);
    if(testInt != NULL) {
        testInt[0] = 9;
        testInt[8] = 88;
        my_free(testInt);
    }
    int* test2 = (int*)my_malloc(32);
    if(test2 != NULL) {
        test2[0] = 7;
    }
    int* testout = (int*)my_malloc(33333);
    if(testout != NULL) {
        testout[0] = 7;
    }
    int* testout2= (int*)my_malloc(2);
    printf("Didn't make it out\n");
    if(testout2 != NULL) {
        testout2[0] = 2;
    }
    
    //printf("first value in test one: %d\n first value in test two: %d\n",testInt[0],test2[0]);
    
    
    //void* test3 = my_malloc(0);
    //void* test4 = my_malloc(16);
    //void* test5 = my_malloc(16);
    
    my_malloc_cleanup();
    return 0;
}