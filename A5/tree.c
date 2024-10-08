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

    int count = 2;
    //for every node in the linked list add it to the tree, tree will either stack point or create new box;
    while (head){
        //printf("adding node %d\n", count);
        point* tmp = head->next;
        addnode(root, head);
        head = tmp;
        count++;
    }

    //return the root node
    return root;
}

void addnode(node* root, point* p){
    //printf("adding_pt %d, %d\n", p->x, p->y);
    //printf("node corners are (%d, %d), (%d, %d), count: %d\n", root->coords[0], root->coords[2], root->coords[1], root->coords[3], root->count);
    p->next = NULL;
    //since the point is in this node increase the count by 1;
    root->count++;

    //figure out which quadrant of this node the point is in
    //dividing such that subdivisions are always the same size +/- 1
    int mx = (root->coords[0] == root->coords[1] ? root->coords[0] : (root->coords[0] + root->coords[1] - 1 )/2);
    int my = (root->coords[2] == root->coords[3] ? root->coords[2] : (root->coords[2] + root->coords[3] - 1 )/2);

    //printf("mx %d, my %d\n", mx, my);

    //quadrant of node that p is in
    //if xp=xn only quads 1 and 2 are valid
    //if yp=yn only quads 2 and 3 are valid

    int quad = p->x <= mx ? (root->coords[2] == root->coords[3] ? 2 : (p->y <= my ? 2 : 1)) :
                            (root->coords[2] == root->coords[3] ? 3 : (p->y <= my ? 3 : 0));
    //printf("point is in quadrant %d\n", quad);
    // climb the tree until there is a node with a point in it  
    if (root->point){
        //printf("node has a point at %d, %d\n", root->point->x, root->point->y);
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
            if (root->coords[1] > root->coords[0] && root->coords[3] > root->coords[2]){
                //quad 0
                root->quads[0] = malloc(sizeof(node));
                *(root->quads[0]) = (node){.coords = {mx+1, root->coords[1], my+1, root->coords[3]},
                        .quads = {NULL, NULL, NULL, NULL}, .point=NULL,
                        .count = 0};
                //printf("quad0 corners: (%d, %d) (%d, %d)\n", root->quads[0]->coords[0], root->quads[0]->coords[2], root->quads[0]->coords[1], root->quads[0]->coords[3]);
            }
            if (root->coords[3] > root->coords[2]){
                //quad 1
                root->quads[1] = malloc(sizeof(node));
                *(root->quads[1]) = (node){.coords = {root->coords[0], mx, my+1, root->coords[3]},
                        .quads = {NULL, NULL, NULL, NULL}, .point=NULL,
                        .count = 0};
                //printf("quad1 corners: (%d, %d) (%d, %d)\n", root->quads[1]->coords[0], root->quads[1]->coords[2], root->quads[1]->coords[1], root->quads[1]->coords[3]);

            }
            if (root->coords[1] > root->coords[0]){
                //quad 3
                root->quads[3] = malloc(sizeof(node));
                *(root->quads[3]) = (node){.coords = {mx+1, root->coords[1], root->coords[2], my},
                        .quads = {NULL, NULL, NULL, NULL}, .point=NULL,
                        .count = 0};
                //printf("quad3 corners: (%d, %d) (%d, %d)\n", root->quads[3]->coords[0], root->quads[3]->coords[2], root->quads[3]->coords[1], root->quads[3]->coords[3]);
            }
            //quad 2
            root->quads[2] = malloc(sizeof(node));
            *(root->quads[2]) = (node){.coords = {root->coords[0], mx, root->coords[2], my},
                    .quads = {NULL, NULL, NULL, NULL}, .point=NULL,
                    .count = 0};
            //printf("quad2 corners: (%d, %d) (%d, %d)\n", root->quads[2]->coords[0], root->quads[2]->coords[2], root->quads[2]->coords[1], root->quads[2]->coords[3]);

            
            //need to consider the case where both nodes are in the same quad:
            //printf("subdivided, p quad is %d\n", quad);

            // root->point into its respective quad
            point* r = root->point;
            root->point = NULL;
            int quadr = r->x <= mx ? (root->coords[2] == root->coords[3] ? 2 : (r->y <= my ? 2 : 1)) :
                            (root->coords[2] == root->coords[3] ? 3 : (r->y <= my ? 3 : 0));
            //printf("node point is in quad %d\n", quadr);
            root->quads[quadr]->point = r;
            root->quads[quadr]->count = root->count-1;

            // move into the child for p
            //printf("adding point to quad: %d\n", quad);
            addnode(root->quads[quad], p);

            //printf("returning, adding next point \n\n");
            return;
        }
    }
    else{
        //printf("node has no point\n");
        //if the node has no points at all
        //assign this as the point
        if (root->count == 1){
            //printf("node is not subdivided, setting node point\n");
            //root->count ++;
            root->point = p;
            return;
        }
        else {
            //printf("node is subdivided\n ===>>>\n");
            // recur in the quadrant that the point is in
            addnode(root->quads[quad], p);
            //printf(">\n");
        }
    }
}

//free every point and every node in a tree
void freetree (node* head){
    if (head == NULL) return;
    //free all of the child nodes
    int i;
    for (i=0; i<4; i++)
        freetree(head->quads[i]);
    free(head->point);
    free(head);
}

int getnpoints_t(node* head, const int x, const int y, const int r) {
    //if (head) printf("node (%d, %d) to ( %d, %d), count = %d\n", head->coords[0], head->coords[2], head->coords[1], head->coords[3], head->count);
    
    // if there is no node return 0
    // if the node has no points return 0
    if (head == NULL || head->count == 0) {
        //printf("no points or head == NULL\n");
        return 0;
    }

    int in = inradn(head, x, y, r);
    // if the node is completely in the circle, return count
    if (in == 1) {
        //printf("completely inside circle\n");
        return head->count;
    }
    // if the node is partiall in the circle, call for its sub quadrants
    else if (in == 2) {
        //printf("partially inside circle\n");
        return getnpoints_t(head->quads[0], x, y, r) +
                getnpoints_t(head->quads[1], x, y, r) +
                getnpoints_t(head->quads[2], x, y, r) +
                getnpoints_t(head->quads[3], x, y, r);
    }
    else {
        //printf("not inside circle\n");
        return 0;
    }
}

//returns 1 if 2 points are within radius r of each other and 0 if not
int inradp(int x1, int y1, int x2, int y2, int r){
    //printf("1: (%d, %d) 2: (%d, %d) r: %d\n", x1, y1, x2, y2, r);
    const int r2 = r*r;
    const int insr = (int)(r*0.70710678118);
    int xdist = abs_fast(x1-x2);
    int ydist = abs_fast(y1-y2);
    //printf("xdist: %d, ydist: %d, r: %d, insr: %d\n", xdist, ydist, r, insr);
    
    if (((xdist <= insr) && (ydist <= insr)) || // accept anything inside of inscribed rectangle
                (xdist <= r && ydist <= r && // reject anything that is outside of circumscribed rectangle
                xdist*xdist + ydist*ydist <= r2)) {
            //printf("stupid\n");
            return 1;
    }
    return 0;
}

//return 1 if rectangle is completely inside a radius
//return 0 if rectangle is completely outside radius
//return 2 if rectangle is partialy inside a radius
int inradn(node* n, int x, int y, int r){
    //if the node has a point just check if the point is in it
    if (n->point) {
        //printf("n has a point\n");
        return inradp(n->point->x, n->point->y, x, y, r);
    }

    int count = inradp(n->coords[0], n->coords[2], x, y, r);
    count += inradp(n->coords[0], n->coords[3], x, y, r);
    count += inradp(n->coords[1], n->coords[2], x, y, r);
    count += inradp(n->coords[1], n->coords[3], x, y, r);
    // if all 4 corners are inside then the whole rectangle is inside
    //printf("count: %d\n", count);
    if (count == 4) return 1;

    // if there is some intersection but not all 4 corners are inside then it is partiall inside
    else if (n->coords[2] <= y && n->coords[3] >= y && (n->coords[0] >= (x-r) || n->coords[0] <= (x+r)) ||
        n->coords[0] <= x && n->coords[1] >= x && (n->coords[2] >= (y-r) || n->coords[3] <= (y+r)) || count > 0) 
        return 2;
    
    // if there is no intersection and none of the corners are inside then it is completely outside
    else if (count == 0) return 0;
}

//super fast absolute value function
int32_t abs_fast(int32_t x) {
    int32_t mask = x >> 31; 
    return (x ^ mask) - mask;
}