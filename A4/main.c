/* 
 * Jack Porter, Purdue ECE368
 * Assignment 4, main file
 * node.h and list.c supplementary
 * Window manager program with linked list implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

int main(int argc, char ** argv)
{
	char* cmd = malloc(sizeof(char) * 10);
    int win;
    node** list = malloc(sizeof(node*));
    *list = NULL;
    
    while (1) {
        scanf("%s %d", cmd, &win);
        if (strcmp(cmd, "open") == 0) {
            // assuming that the window does not already exist
            // add a new node at the head
            node* n = malloc(sizeof(node));
            n->val = win;
            addNode(list, n);
        }
        if (strcmp(cmd, "switch") == 0) {
            // assuming window exists
            // remove node
            node* n = removeNode(list, win);
            // add to head of list
            if (n) addNode(list, n);
        }
        if (strcmp(cmd, "close") == 0) {
            // assuming that the window exists
            // remove node
            node* n = removeNode(list, win);
            // delete node
            free(n);
            // if list empty then exit
            if(*list==NULL) {
                free(cmd);
                free(list);
                return 0;
            }
        }
        //print head of list
        if (*list) printf("%d\n", (*list)->val);
    }
	
}
