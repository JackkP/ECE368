/*
 * Jack Porter, Purdue ECE368
 * Assignment 5, point struct
 */

#include <stdint.h>

#ifndef TREE_H
#define TREE_H

//node struct to buid tree
typedef struct Node {
	char vh;
	int n;
	int x;
	int y;
	struct Node* left;
	struct Node* right;
	struct Node* next;
} Node;

//struct used to print tree
typedef struct Branch {
	struct Branch* prev;
	char* str;
} Branch;

Node* push(Node* top, Node* new);
Node* pop(Node** top);

void freetree(Node* root);

void printpre(FILE* file, Node* root);
void printpost(FILE* file, Node* root);
void printpack(FILE* file, Node* root, int corn_x, int corn_y);


//for printing trees
void print(Node* root);
void showBranches(Branch* p);
void printTree(Node* n, Branch* prev, int isRight);

#endif