
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include "tree.h"

//for stack using left pointer

Node* push(Node* top, Node* new) {
	new->next = top;
	return new;
}

Node* pop(Node** top) {
    if ((*top)==NULL) return NULL;
	Node* ret = *top;
	*top = (*top)->next;
	return ret;
}

void freetree(Node* root){
    if (root != NULL){
        freetree(root->left);
        freetree(root->right);
        free(root);
    }
}

//print preorder traversal without the sizes of the nodes
void printpre(FILE* file, Node* root){
    if (root == NULL) return;
    if (root->vh != 0) {
        fprintf(file, "%c\n", root->vh);
    }
    else {
        fprintf(file, "%d(%d,%d)\n", root->n, root->x, root->y);
    }
    printpre(file, root->left);
    printpre(file, root->right);
}

//print post-order traversal with the sizes of the nodes
void printpost(FILE* file, Node* root){
    if (root == NULL) return;
    printpost(file, root->left);
    printpost(file, root->right);
    if (root->vh != 0) {
        fprintf(file, "%c(%d,%d)\n", root->vh, root->x, root->y);
    }
    else {
        fprintf(file, "%d(%d,%d)\n", root->n, root->x, root->y);
    }
}

void printpack(FILE* file, Node* root, int corn_x, int corn_y){
    if (root == NULL) return;
    if (root->right == NULL) { //if this is a leaf node
        fprintf(file, "%d((%d,%d)(%d,%d))\n", root->n, root->x, root->y, corn_x, corn_y);
        return;
    }
    
    if (root->vh == 'V') {
        printpack(file, root->left, corn_x, corn_y);
        printpack(file, root->right, corn_x + root->left->x, corn_y);
    }
    else {
        printpack(file, root->left, corn_x, corn_y + root->right->y);
        printpack(file, root->right, corn_x, corn_y);
    }
}

//PRINTING (for debugging):
//Helper functions:
//function to print branches of the binary tree
void showBranches(Branch* p){
	if (p) {
		showBranches(p->prev); //print the previous branch (in this line)
		printf("%s", p->str);
	}
}

void print(Node* root){
    printTree(root, NULL, 0);
}

//function to print binary tree using branches
void printTree(Node* n, Branch* prev, int isRight) {
    if (!n) { //dont print this node
        return;
    }

    char prev_str[10];
    strcpy(prev_str, "      "); //start with a space precceding (horizontally)
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
        strcpy(prev_str, "     |");
    }
    else {
	strcpy(branch->str, "`———"); //left node
        strcpy(prev->str, prev_str);
    }
    showBranches(branch); //print the linked list of preceeding branches on this line
    if (n->vh != 0) printf(" %c(%d,%d)\n", n->vh, n->x, n->y);
    else printf(" %d(%d,%d)\n", n->n, n->x, n->y);	
    
    if (prev) {	//add space or vertical line depending on left/right
        strcpy(prev->str, prev_str);
    }
    strcpy(branch->str, "     |");
    printTree(n->left, branch, 0);
    free(branch->str); //get rid of the branch when finished
    free(branch);
}

