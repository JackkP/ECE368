/* 
 * Jack Porter, Purdue ECE368
 * Assignment 5, main file
 * node.h and list.c supplementary
 */

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

//debug function to print a list of points
void print(point* head){
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

// print number of points in a linked list using brute force
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


//build a quadtree from a linked list
node* buildtree (point* head, int maxX, int maxY, int minX, int minY) {
    //printf("entered buildtree\n");
    if (head == NULL) return NULL;

    //create a root node that is the size of the grid and has head as the only point in it
    //printf("root_pt %d, %d\n", head->x, head->y);
    node* root = malloc(sizeof(node));
    *root = (node){.coords = {minX, maxX, minY, maxY},
                    .quads = {NULL, NULL, NULL, NULL},
                    .count = 1, .point = head};
    head = head->next;
    root->point->next = NULL;

    //for every node in the linked list add it to the tree, tree will either stack point or create new box;
    while (head){
        point* tmp = head->next;
        addnode(root, head);
        head = tmp;
    }

    //return the root node
    return root;
}

void addnode(node* root, point* p){
    //printf("adding_pt %d, %d\n", p->x, p->y);
    p->next = NULL;
    //since the point is in this node increase the count by 1;
    root->count++;

    //figure out which quadrant of this node the point is in
    int mx = (root->coords[0] + root->coords[1])/2;
    int my = (root->coords[2] + root->coords[3])/2;

    //quadrant of node that p is in
    //if xp=xn only quads 0 and 3 are valid
    //if yp=yn only quads 2 and 3 are valid

    int quad = p->x > mx ? (p->y > my ? 0 : 3) : (p->y > my ? 1 : 2);
    //printf("point is in quadrant %d\n", quad);
    // climb the tree until there is a node with a point in it  
    if (root->point){
        //printf("node has a point\n");
        //check if the point is the same as the one assigned to this node
        if (root->point->x == p->x && root->point->y == p->y) {
            //printf("duplicate point\n");
            //if its the same then add the point to the count of that node (already done)
            //get rid of p to stack duplicate points
            free(p);
            //printf("returning, adding next point \n\n");
            return;
        }
        //if not, subdivide, then move root->point and p into their quadrants
        else{
            //printf("subdividing\n");
            //subdivide (creating child nodes)
            if (root->coords[3] > root->coords[2]){
                //quad 0
                root->quads[0] = malloc(sizeof(node));
                *(root->quads[0]) = (node){.coords = {mx+1, root->coords[1], my+1, root->coords[3]},
                        .quads = {NULL, NULL, NULL, NULL}, .point=NULL,
                        .count = 0};
            }
            if (root->coords[1] > root->coords[0] && root->coords[3] > root->coords[2]){
                //quad 1
                root->quads[1] = malloc(sizeof(node));
                *(root->quads[1]) = (node){.coords = {root->coords[0], mx, my+1, root->coords[3]},
                        .quads = {NULL, NULL, NULL, NULL}, .point=NULL,
                        .count = 0};
            }
            if (root->coords[1] > root->coords[0]){
                //quad 2
                root->quads[2] = malloc(sizeof(node));
                *(root->quads[2]) = (node){.coords = {root->coords[0], mx, root->coords[2], my},
                        .quads = {NULL, NULL, NULL, NULL}, .point=NULL,
                        .count = 0};
            }
            //quad 3
            root->quads[3] = malloc(sizeof(node));
            *(root->quads[3]) = (node){.coords = {mx+1, root->coords[0], root->coords[2], my},
                    .quads = {NULL, NULL, NULL, NULL}, .point=NULL,
                    .count = 0};

            //printf("subdivided, quad is %d\n", quad);
            // move into the child for p
            //printf("%p\n", root->quads[quad]);
            (root->quads[quad])->point = p;
            //printf("adding count\n");
            (root->quads[quad])->count = 1;
            //printf("assigned p\n");

            // move into the child for root->point
            point* r = root->point;
            root->point = NULL;
            int quadr = r->x > mx ? (r->y > my ? 0 : 3) : (r->y > my ? 1 : 2);
            //printf("node point is in quad %d\n", quadr);
            root->quads[quadr]->point = r;
            root->quads[quadr]->count = 1;
            //printf("returning, adding next point \n\n");
            return;
        }
    }
    else{
        //printf("node has no point\n");
        //if the node has no points at all
        //assign this as the point and increase count by 1
        if (root->count == 1){
            //printf("node is not subdivided, setting node point\n");
            root->count ++;
            root->point = p;
            return;
        }
        else {
            //printf("node is subdivided\n");
            // recur in the quadrant that the point is in
            addnode(root->quads[quad], p);
            //printf(">\n");
        }
    }
}

void freetree (node* head){
    if (head == NULL) return;
    //free all of the child nodes
    for (int i=0; i<4; i++)
        freetree(head->quads[i]);
    free(head->point);
    free(head);
}