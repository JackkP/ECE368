/* 
 * Jack Porter, Purdue ECE368
 * Assignment 5 (see A5.pdf for details)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "point.h"

int getnpoints_n(point* head, int x, int y, int r);
void printlist(point* head);

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
    while(3 == scanf("%d %d %d", &x, &y, &r))
        printf("%d\n", getnpoints_n(head, x, y, r));
    return 0;
}

// print number of points using brute force
// using this to check grading scores/speed for
// reference since it takes no time to write
// (all testcases timed out)
int getnpoints_n(point* head, int x, int y, int r){
    int count = 0;
    while (head){
        if ((x-head->x)*(y-head->y) + (y-head->y)*(y-head->y) <= r*r)
            count++;
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
