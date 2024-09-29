/* 
 * Jack Porter, Purdue ECE368
 * Assignment 5 (see A5.pdf for details)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "point.h"

int getnpoints_n(point* head, int x, int y, int r);
void printlist(point* head);
void freelist(point* head);
int32_t abs(int32_t x);

int main(int argc, char ** argv)
{
	//read all of the points into an array using a file
    if (argc == 1) return 1;
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) return 1;
    //read every line from the file into two integers into a linkedlist
    int x, y;
    point* head = NULL;
    while(2 == fscanf(file, "%d %d\n", &x, &y)){
        point* p = malloc(sizeof(point));
        p->x = x;
        p->y = y;
        p->next = head;
        head = p;
    }
    fclose(file);

    //build a quad tree from the linked list

    int r;
    while(scanf("%d %d %d", &x, &y, &r) == 3)
        printf("%d\n", getnpoints_n(head, x, y, r));
    freelist(head);
    return 0;
}

// print number of points using brute force
// using this to check grading scores/speed for
// reference since it takes no time to write
// (all testcases timed out)
int getnpoints_n(point* head, const int x, const int y, const int r){
    const int r2 = r*r;
    const int insr = (int)(r*0.70710678118);
    //printf("insr: %d\n", insr);
    int count = 0;
    while (head){
        int xdist = abs(x-head->x);
        int ydist = abs(y-head->y);
        //printf("dists: %d, %d\n", xdist, ydist);
        if (xdist <= insr && ydist <= insr || // accept anything inside of inscribed rectangle
            (xdist <= r && ydist <= r && // reject anything that is outside of circumscribed rectangle
            (x-head->x)*(x-head->x) + (y-head->y)*(y-head->y) <= r2)) {
            //printf("added\n");
            count++;
        }
        head = head->next;
    }
    return count;
}

//debug function to print a list of points
void printlist(point* head){
    while (head){
        printf("%d, %d\n", head->x, head->y);
        head = head->next;
    }
}

//free the points in the list
void freelist(point* head){
    point* tmp;
    while (head){
        tmp = head->next;
        free(head);
        head = tmp;
    }
}

//super fast absolute value function
int32_t abs(int32_t x) {
    int32_t mask = x >> 31; 
    return (x ^ mask) - mask;
}
