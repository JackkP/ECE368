/*
 * Jack Porter, Purdue ECE368
 * Assignment 4, node struct
 */

#ifndef NODE_H
#define NODE_H

typedef struct node
{
  int val;
  struct node * next;
} node;

void addNode(node** head, node* n);
node* removeNode(node** head, int n);
void printlist(node* head);

#endif