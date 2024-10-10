/* 
 * Jack Porter, Purdue ECE368
 * Assignment 5, main file
 * node.h and list.c supplementary
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "tree.h"

#define RIGHT 1
#define LEFT 0

//screw it, using a global variable because I want this to work
Node* root = NULL;

void freeT(){
    freetree(root);
}
//recursively delete all nodes from tree with root n
void freetree(Node* n){
	if (n) {
		freetree(n->right);
		freetree(n->left);
		free(n);
	}
}

//PRINTING:
//Helper functions:
//function to print branches of the binary tree
void showBranches(Branch* p){
	if (p) {
		showBranches(p->prev); //print the previous branch (in this line)
		printf("%s", p->str);
	}
}

void print(){
    printTree(root, NULL, 0);
}

//function to print binary tree using branches
void printTree(Node* n, Branch* prev, int isRight) {
    if (!n) { //dont print this node
        return;
    }

    char prev_str[10];
    strcpy(prev_str, "         "); //start with a space precceding (horizontally)
    Branch* branch = malloc(sizeof(Branch)); //new branch for this node
    branch->str = malloc(sizeof(char) * 10);
    branch->prev = prev; //building linked list of horzontal branches
    strcpy(branch->str, prev_str);
    printTree(n->right, branch, 1); //print the tree for the right most branch

    if (!prev) { //root of the tree
	strcpy(branch->str, "———");
    }
    else if (isRight){
	strcpy(branch->str, ".———"); //right node
        strcpy(prev_str, "        |");
    }
    else {
	strcpy(branch->str, "`———"); //left node
        strcpy(prev->str, prev_str);
    }
    showBranches(branch); //print the linked list of preceeding branches on this line
    printf(" %d, %d", n->x, n->y);
	
	//debug code to show parent value
    //if (n->parent) printf(" p=%d" n->parent->x);
    
	if (n->color == RED) printf(" (R)\n");
    else if (n->color == BLACK) printf(" (B)\n");
    
    if (prev) {	//add space or vertical line depending on left/right
        strcpy(prev->str, prev_str);
    }
    strcpy(branch->str, "        |");
    printTree(n->left, branch, 0);
    free(branch->str); //get rid of the branch when finished
    free(branch);
}

//function to rotate a tree through P
void rotate(Node* P, int right){
	Node* G = P->parent;
	Node* N;
	if(right == 1){
		N = P->left;
		P->parent = N;
		P->left = N->right;
		if (N->right) N->right->parent = P;
		N->right = P;
	}
	else{
		N = P->right;
		P->parent = N;
		P->right = N->left;
		if (N->left) N->left->parent = P;
		N->left = P;
	}
	
	N->parent = G;
	if(!G) root = N;
	else if (G->right == P) G->right = N;
	else G->left = N;
}

//function to return the uncle of a node
Node* uncle(Node* n){
	if (n->parent && n->parent->parent){ //n has a grandparent
		if (n->parent == n->parent->parent->left) return n->parent->parent->right;
		else if (n->parent = n->parent->parent->right) return n->parent->parent->left;
	}
	else return NULL;
}

//function to return the sibling
Node* sibling(Node* n){ //return the sibling
	if (n->parent && n == n->parent->right) return n->parent->left;
	else if (n->parent && n == n->parent->left) return n->parent->right;
	else return NULL;
}

//ballance the tree after inserting a node
void balanceIns(Node* n){
	if (!n) return;
	//printTree(root, NULL, 0);
	//printf("balancing\n");
	if (n->parent == NULL) { //case 1, inserting at the root
		//printf("case 1\n"); //set the color to black
		n->color = BLACK;
		return;
	}
	
	else if (n->parent->color == BLACK){ //case 2, parent is black
		//printf("case 2\n"); //nothing happens
		return; 
	}
	
	else if (uncle(n) && n->parent->color == RED && uncle(n)->color == RED){ //case 3, parent and uncle are both red
		//printf("case 3\n");
		n->parent->color = BLACK; //parent becomes black
		n->parent->parent->color = RED; //grandparent becomes red
		uncle(n)->color = BLACK; //uncle becomes black
		balanceIns(n->parent->parent); //ballance from the grandparent
	}
	//case 4, (node left, parent right)||(node right, parent left), uncle is black, parent is red
	else if (n->parent->color == RED && n->parent->parent //parent is red (1) and has a grandparent
			&& (!uncle(n) || uncle(n)->color == BLACK) //Uncle is black (2)
			&& (n == n->parent->left && n->parent == n->parent->parent->right //node is the right inside child (3)
				|| n == n->parent->right && n->parent == n->parent->parent->left)){ //node is the left inside child (3)
		//printf("case 4\n");
		if (n == n->parent->left && n->parent == n->parent->parent->right){ //node is the inside grandchild to the right, case 4
			//rotate to the right through parent
			Node* P = n->parent;
			rotate(P, RIGHT);
			balanceIns(P);
		}
		else if (n == n->parent->right && n->parent == n->parent->parent->left){ //node is the inside grandchild to the left, case 4
			//rotate to the left through parent
			Node* P = n->parent;
			rotate(P, LEFT);
			balanceIns(P);
		}
	}
	//case 5 n is red, parent is red, uncle is black, (node left, parent left) || (node right, parent right)
	else if (n->color == RED && n->parent->color == RED && n->parent->parent //n is red, parent is red, has a grandparent
			&& (!uncle(n) || uncle(n)->color == BLACK) //uncle is black
			&& (n == n->parent->left && n->parent == n->parent->parent->left || //left left or right right
				n == n->parent->right && n->parent == n->parent->parent->right)) { //case 5
		//printf("case 5, n=%d\n", n->x);
		//if (n->parent) printf(" p=%d\n", n->parent->x);
		if (n == n->parent->left && n->parent == n->parent->parent->left){ //node is the outside grandchild to the left, case 5 
			//printf("l-l\n");
			
			Node* G = n->parent->parent;
			Node* P = n->parent;

			G->color = RED; //change colors of parent and grandparent
			P->color = BLACK;
			
			//print();

			rotate(G, RIGHT); //rotate right through grandparent
		}
		else if (n == n->parent->right && n->parent == n->parent->parent->right){ //node is the outside grandchild to the right, case 5
			//printf("r-r\n");

			Node* G = n->parent->parent;
			Node* P = n->parent;
			
			G->color = RED; //change colors of parent and grandparent
			P->color = BLACK;
			
			//print();
			
			rotate(G, LEFT); //rotate left through grandparent
		}
	}
}

//recursively add node to binary tree
void addAfter(Node* val, Node** n, Node* parent){ 
	if ((*n)) {
		if (val->x >= (*n)->x) {
			addAfter(val, &((*n)->right), *n); //add it to the right because it is greater than or equal to n
		}
		else if (val->x < (*n)->x){ //add it to the left because it less than n
			addAfter(val, &((*n)->left), *n);
		}
	}
	else {
		*n = val;
		(*n)->parent = parent;
		//printf("Inserting %d and balancing\n", val->x);
		balanceIns(*n);
		//printf("completed tree:\n");
		//print();
	}
}

//add a node to the tree
void push(int x, int y){
	Node* newNode = malloc(sizeof(Node));
    *(newNode) = (Node){.x = x, .y = y,
                        .left = NULL, .right = NULL, .parent = NULL,
                        .color = RED};
	addAfter(newNode, &root, NULL);
}

//VERIFICATION (for debugging):
//Helper function:
//returns the number of black nodes in the sub tree or a very large negative number if the rb tree is not valid
int verR(Node* n){
	if (!n) return 0;
	int countL = verR(n->left);
	int countR = verR(n->right);
	
	//n is root it must be black
	if (n->parent == NULL && n->color == RED){
		printf("root is red\n");
		return INT32_MIN;
	}
	//number of black nodes must be the same
	if (countR != countL){
		printf("unequal counts\n");
		return INT32_MIN;
	}
	//if n is red it must have two black children
	if (n->color == RED && !((!n->left || n->left->color == BLACK) && (!n->right || n->right->color == BLACK))) {
		printf("red with one non-black child\n");
		return INT32_MIN;
	}
	//left must be smaller or equal and right must be greater or equal
	if ((n->right && n->right->x < n->x) || (n->left && n->left->x > n->x)){
		printf("Violates bst\n");
		return INT32_MIN;
	}
	if (n->color == BLACK) return 1 + countL;
	else return 0 + countL;
}

//returns 1 if the tree is a valid red black tree
int verify(){
	int count = verR(root);
	if (count < 0) {
		return 0;
	}
	else {
		return 1;
	}
}

//returns 1 if 2 points are within radius r of each other and 0 if not
int inradp(int x1, int y1, int x2, int y2, int r){
    //printf("1: (%d, %d) 2: (%d, %d) r: %d\n", x1, y1, x2, y2, r);
    int xdist = abs_fast(x1 - x2);
    int ydist = abs_fast(y1 - y2);
    if (xdist*xdist + ydist*ydist <= r*r) {
            //printf("stupid\n");
            return 1;
    }
    return 0;
}

int npoints(int x, int y, int r){
    return getnpointsR(root, x-r, x+r, x, y, r);
}

int getnpointsR(Node* n, int xmin, int xmax, int x, int y, int r) {
    if(n == NULL) return 0;
    //printf("looking at pt %d, %d, xmin: %d, xmax: %d\n", n->x, n->y, xmin, xmax);

    return inradp(n->x, n->y, x, y, r)
            + (n->x >= xmin ? getnpointsR(n->left, xmin, xmax, x, y, r) : 0)
            + (n->x <= xmax ? getnpointsR(n->right, xmin, xmax, x, y, r) : 0);

}

//super fast absolute value function
int32_t abs_fast(int32_t x) {
    int32_t mask = x >> 31; 
    return (x ^ mask) - mask;
}
