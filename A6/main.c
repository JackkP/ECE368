/* 
 * Jack Porter, Purdue ECE368
 * Assignment 5 (see A5.pdf for details)
 * Using a red-black tree to balance a binary tree sorted by x value, then only look at points within the x values that the circle covers
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "tree.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))


int main(int argc, char ** argv)
{
	//read all of the points into an array using a file
    if (argc == 1) return 1;

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) return 1;

    //read every line from the file into either: 3 integers or 1 char
    int n, x, y; //n for node number, x for x dimensions, y for height
    char l;

    Node* stacktop = NULL;
    
    while(1) {
        if(3 == fscanf(file, "%d(%d,%d)\n", &n, &x, &y)){
            //printf("adding leaf %d(%d,%d)\n", n, x, y);
            
            //create a new node and push to stack
            Node* newn = malloc(sizeof(Node));
            *newn = (Node){.vh = 0, .n = n, .x = x, .y = y, .left = NULL, .right = NULL, .next = NULL};
            stacktop = push(stacktop, newn);
        }
        else if(1 == fscanf(file, "%c\n", &l)){
            //printf("adding node %c\n", l);

            //create a new node
            Node* newn = malloc(sizeof(Node));
            //pop last two items from stack
            //add to left and right of this node
            Node* r = pop(&stacktop); //need to pop the right node first because order of function calls if in one line is funky
            *newn = (Node){.vh = l, .right = r, .left = pop(&stacktop), .next = NULL};
            //calculate the dimensions of this node
            if (l == 'V') {
                newn->x = newn->left->x + newn->right->x;
                newn->y = MAX(newn->left->y, newn->right->y);
            }
            else {
                newn->y = newn->left->y + newn->right->y;
                newn->x = MAX(newn->left->x, newn->right->x);
            }
            //push this node to stack
            stacktop = push(stacktop, newn);
        }
        else break;
    }
    fclose(file);

    //debug print tree
    //print(stacktop);

    //print output file 1
    file = fopen(argv[2], "w");
    if (file == NULL) return 1;
    printpre(file, stacktop);
    fclose(file);

    //print output file 2
    file = fopen(argv[3], "w");
    if (file == NULL) return 1;
    printpost(file, stacktop);
    fclose(file);

    //print output file 3
    file = fopen(argv[4], "w");
    if (file == NULL) return 1;
    printpack(file, stacktop, 0, 0);
    fclose(file);

    freetree(stacktop);

    return 0;
}
