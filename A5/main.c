/* 
 * Jack Porter, Purdue ECE368
 * Assignment 5 (see A5.pdf for details)
 * Using a quadtree to spatially divide points into rectangular spaces
 * Rectangles with a known number of points are then classified
 *  fully inside (counted)
 *  fully outside (ignored)
 *  partially inside (sub rectangles are then classified)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <sys/time.h>
#include "tree.h"


int main(int argc, char ** argv)
{
	//read all of the points into an array using a file
    if (argc == 1) return 1;
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) return 1;
    //read every line from the file into two integers into a linkedlist
    int x, y;

    int maxX = INT32_MIN;
    int maxY = INT32_MIN;
    int minX = INT32_MAX;
    int minY = INT32_MAX;
    
    //printf("xxnn: %d, %d, %d, %d\n", maxX, maxY, minX, minY);

    point* head = NULL;
    while(2 == fscanf(file, "%d %d\n", &x, &y)){
        if (x > maxX) maxX = x;
        if (y > maxY) maxY = y;
        if (x < minX) minX = x;
        if (x < minY) minY = y;
        point* p = malloc(sizeof(point));
        p->x = x;
        p->y = y;
        p->next = head;
        head = p;
    }
    fclose(file);

    //print(head);

    //build a quad tree from the linked list ?
    //printf("xxnn: %d, %d, %d, %d\n", maxX, maxY, minX, minY);

    //struct timeval stop, start;

    //gettimeofday(&start, NULL);
    node* root = buildtree(head, maxX, maxY, minX, minY);
    //gettimeofday(&stop, NULL);
    //printf("buildtree took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
    

    //printf("survived buildtree\n");
    int r;
    while(scanf("%d %d %d", &x, &y, &r) == 3) {
        //gettimeofday(&start, NULL);
        //printf("%d\n", getnpoints_n(head, x, y, r));
        printf("%d\n", getnpoints_t(root, x, y, r));
        //gettimeofday(&stop, NULL);
        //printf("took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
    }
    freetree(root);
    //freelist(head);
    return 0;
}
