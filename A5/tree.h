/*
 * Jack Porter, Purdue ECE368
 * Assignment 5, point struct
 */

#ifndef TREE_H
#define TREE_H

typedef struct point {
    int x;
    int y;
    struct point* next;
} point;

typedef struct node{
    int coords[4]; //xn, xp, yn, yp, corners of box
    int count;
    struct point* point;
    struct node* quads[4]; //q1, q2, q3, q4
} node;

void print(point* head);
void freelist(point* head);
int getnpoints_n(point* head, const int x, const int y, const int r);

node* buildtree (point* head, int maxX, int maxY, int minX, int minY);
void addnode(node* root, point* p);
void freetree (node* head);
int getnpoints_t(node* head, const int x, const int y, const int r);

#endif