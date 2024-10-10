/* 
 * Jack Porter, Purdue ECE368
 * Assignment 5 (see A5.pdf for details)
 * Using a red-black tree to balance a binary tree sorted by x value, then only look at points within the x values that the circle covers
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include "tree.h"


int main(int argc, char ** argv)
{
	//read all of the points into an array using a file
    if (argc == 1) return 1;
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) return 1;
    //read every line from the file into two integers into a linkedlist
    int x, y;

    //head of RB tree
    while(2 == fscanf(file, "%d %d\n", &x, &y)){
        //printf("adding node %d\n", x);
        push(x, y);
        //printf("\n");
        //print();
        //printf("verify: %d\n", verify());
        //printf("\n");

    }
    fclose(file);

    struct timeval stop, start;

    //gettimeofday(&start, NULL);
    //gettimeofday(&stop, NULL);
    //printf("buildtree took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
    //print();
    //printf("%d\n", verify());

    //printf("survived buildtree\n");
    int r;
    while(scanf("%d %d %d", &x, &y, &r) == 3) {
        //gettimeofday(&start, NULL);
        printf("%d\n", npoints(x, y, r));
        //gettimeofday(&stop, NULL);
        //printf("took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
    }
    freeT();
    return 0;
}
