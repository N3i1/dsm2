/* This file is part of dsm2.
 *
 * Dsm2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Dsm2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dsm2.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

void addHead(LinkedList *list, void *data) {
   Node *node = (Node*) malloc(sizeof(Node));
   node->data = data;
  
   if ( list->head == NULL ) {
      list->tail = node;
      node->next = NULL;
   } 
   else {
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
   while ( node != NULL ) {
      tmp = node;
      node = node->next;
      remove(tmp->data);
      free(tmp->data);
      free(tmp);
   }
   return (EXIT_SUCCESS);
}

int crosscheckPmonAddy(LinkedList *ksuseList, LinkedList *pmonMapslist) {
   Node *ksuseNode = ksuseList->head;
   AmmInfo* mInfo = NULL;
   int found = 0;
   while (ksuseNode != NULL) {
      Node *mmapsNode = pmonMapslist->head;

      while (mmapsNode != NULL) {
         if ( compareAddy(ksuseNode->data, mmapsNode->data) == 0) {
         //Add a flag into Mmaps *maps to say KEEP a record
         setKeep(mmapsNode->data);
         found++;
         break;
         }
         mmapsNode = mmapsNode->next;
      }
      ksuseNode = ksuseNode->next;
   }
  return 0;
}

int removeNoneMatchedNodes(LinkedList *list) {
   Node *node = list->head;
   while (node != NULL) {
      Node *tmpNode = getNode(list, node->data);
      releaseNode(list, tmpNode);
      node = node->next;
      free(tmpNode);
  }
  return 0;
}

void releaseNode(LinkedList *list, Node *node) {
   if (node == list->head) {
      if (list->head->next == NULL) {
         list->head = list->tail = NULL;
      }
      else {
         list->head = list->head->next;
      }
   }
   else {
      Node *tmp = list->head;
      while (tmp != NULL && tmp->next != node) {
         tmp = tmp->next;
      }
      if (tmp != NULL) {
         tmp->next = node->next;
      }
  }
  releaseNodeMmaps(node->data);
  free(node->data);
}

Node *getNode(LinkedList *list, void* data) {
   Node* node = list->head;
   while( node != NULL ) {
      if( isKeepSet(node->data ) == 1 ) {
         return node;
      }
      node = node->next;
   }
  return NULL;
}

Node *getMatchingNode(LinkedList *list, COMPARE compare, void *data) {
   Node *node = list->head;
   while ( node != NULL ) {
       if ( compare(node->data, data) == 0 ) {
         return node;
      }
      node = node->next;
   }
  return NULL;
}

void updateLinkedList(LinkedList *list, UPDATE update, unsigned int* latchFree) {
   Node *current = list->head;
   while (current != NULL) {
      update(current->data);
      current = current->next;
   }
}

int addMapstoAddySpace(LinkedList *list) {
   Node *current = list->head;
   while (current != NULL) {
      int chk = 0;
      chk = mapFileToAddr(current->data);
      if (chk == 0)
         current = current->next;
      else {
         printf("Error mapping file");
         return (EXIT_FAILURE);
      }
  }
  return (EXIT_SUCCESS);
}