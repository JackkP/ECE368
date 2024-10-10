/*
 * Jack Porter, Purdue ECE368
 * Assignment 5, point struct
 */

#include <stdint.h>

#ifndef TREE_H
#define TREE_H


#define RED 1
#define BLACK 0

//node struct to buid tree
typedef struct Node {
	struct Node* right;
	struct Node* left;
	struct Node* parent;
	int x;
    int y; 
	int color;

} Node;

//struct used to print tree
typedef struct Branch {
	struct Branch* prev;
	char* str;
} Branch;

int getnpoints_t(Node* head, const int x, const int y, const int r);

//push a node to tree
void push(int x, int y);
//verify that the tree is a redblack tree (debugging)
int verify();
//rotate a tree through node h to the right=true or left=false
void rotate(Node* P, int right);

//Return the uncle or NULL if no uncle
Node* uncle(Node* n);
//Return the sibling of the node or NULL if no sibling
Node* sibling(Node* n);

//Ballance the tree with root N
void balanceIns(Node* n);
//helper function for push
void addAfter(Node* val, Node** n, Node* parent);
//recursive delete
void freeT();
void freetree(Node* n);

//helper function for verification
int verR(Node* n);

void print();
void showBranches(Branch* p);
void printTree(Node* n, Branch* prev, int isRight);

int inradp(int x1, int y1, int x2, int y2, int r);
int npoints(int x, int y, int r);
int getnpointsR(Node* n, int xmax, int xmin, int x, int y, int r);

//super fast absolute value function
int32_t abs_fast(int32_t x);

#endif