#include "list.h"
#include <stdio.h>
#include <stdlib.h>

void addHead(LinkedList *list, void *data) {
    Node *node = (Node*) malloc(sizeof(Node));
    node->data = data;
  
    if ( list->head == NULL ) 
    {
        list->tail = node;
        node->next = NULL;
    } 
    else 
    {
        node->next = list->head;
    }
    
    list->head = node;
}


void initList(LinkedList *list) {
  list->head = NULL;
  list->head = NULL;
  list->current = NULL;
}


void displayAllLinkedList(LinkedList *list, DISPLAY display) {
  Node *current = list->head;
  while ( current != NULL ) {
    display(current->data);
    current = current->next;
  }
}


int deleteAllNodesInList(LinkedList *list, REMOVE remove) {
    Node *node = list->head;
    Node *tmp;
    while ( node != NULL )
    {
        tmp = node;
        node = node->next;
        remove(tmp->data);
        free(tmp->data);
        free(tmp);
    }
  return (EXIT_SUCCESS);
}
