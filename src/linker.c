/*
 * This file is part of dsm2.
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

#include <stdio.h>
#include <stdlib.h>

#include "linker.h"
#include "ksuse.h"

void updateLinkedList(LinkedList *list, UPDATE update, unsigned int* latchFree) {
  Node *current = list->head;
  while (current != NULL) {
    update(current->data);
    current = current->next;
  }
}

void displayAllLinkedList(LinkedList *list, DISPLAY display) {
  Node *current = list->head;
  while (current != NULL) {
    display(current->data);
    current = current->next;
  }
}

Node *getMatchingNode(LinkedList *list, COMPARE compare, void *data) {
  Node *node = list->head;
  while (node != NULL) {
    if (compare(node->data, data) == 0) {
      return node;
    }
    node = node->next;
  }
  return NULL;
}

void displayAllMatchingNodes(LinkedList *list, COMPARE compare, DISPLAY display, void *data) {
  Node *node = list->head;
  while (node != NULL) {
    if (compare(node->data, data) == 0) {
      display(node->data);
      node = node->next;
    } else {
      node = node->next;
    }
  }
}

void addHead(LinkedList *list, void *data) {
  Node *node = (Node*) malloc(sizeof (Node));
  node->data = data;
  if (list->head == NULL) {
    list->tail = node;
    node->next = NULL;
  } else {
    node->next = list->head;
  }
  list->head = node;
}

void initList(LinkedList *list) {
  list->head = NULL;
  list->head = NULL;
  list->current = NULL;
}

int CrosscheckNodes(LinkedList *ksuseList, LinkedList *pmonMapslist) {
  Node *ksuseNode = ksuseList->head;
  //Node *mmapsNode = pmonMapslist->head;
  AmmInfo* mInfo = NULL;
  int found = 0;
  while (ksuseNode != NULL) {
    Node *mmapsNode = pmonMapslist->head;
    while (mmapsNode != NULL) {
      if (compareAddy(ksuseNode->data, mmapsNode->data) == 0) {
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
      //break;
    }
  }
  return (EXIT_SUCCESS);
}

int removeMapsFromAddySpace(LinkedList *list) {
  Node *current = list->head;
  while (current != NULL) {
    if (unmapFileFromAddr(current->data) == 0) {
      current = current->next;
    } else {
      return (EXIT_FAILURE);
      //break;
    }
  }
  return (EXIT_SUCCESS);
}

void releaseLinkedList(LinkedList *list, RELEASE release) {
  Node *current = list->head;
  while (current != NULL) {
    release(current->data);
    free(list->head);
    current = current->next;
  }
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
  free(node->data);
}

int deleteNode(LinkedList *list) {
  Node *node = list->head;
  while (node != NULL) {
    if ((isKeepSet(node->data)) != 0) {
      releaseNode(list, node);

    }
    node = node->next;
  }
  return 0;
}

int deleteNodeInList(LinkedList *list) {
  Node *node = list->head;
  while (node != NULL) {
    releaseNode(list, node);
    node = node->next;
  }
  return (EXIT_SUCCESS);
}
