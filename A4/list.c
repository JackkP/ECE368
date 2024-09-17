/* 
 * Jack Porter, Purdue ECE368
 * Assignment 4, main file
 * node.h and list.c supplementary
 */

#include <stdio.h>
#include "node.h"

/*
 * Function to insert a node at the head of a linked list
 * does not account for the case where a node already exists
 */
void addNode(node** head, node* n){
    node* tmp = *head;
    *head = n;
    n->next = tmp;
}
/*
 * Function to remove a node from a linked list
 */
node* removeNode(node** head, int n) {
    // case 1: list empty
    if(*head == NULL) return NULL;
    // case 2: head contains n
    else if((*head) -> val == n) {
        node* tmp = *head;
        *head = (*head) -> next;
        tmp->next = NULL;
        return tmp;
    }
    // case 3: head does not contain n
    node* curr = *head;
    while (curr->next) {
        if (curr->next->val == n) {
            node* tmp = curr->next;
            curr->next = curr->next->next;
            return tmp;
        }
        curr = curr->next;
    }
    return NULL;
}

/*
 * Debug function to print the entire linked list
 */

void printlist(node* head){
    while (head){
        printf("%d\n", head->val);
        head = head->next;
    }

}