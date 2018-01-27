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

typedef struct _node{
    void *data;
    struct _node *next;
} Node; 


typedef struct _linkedList{
    Node *head;
    Node *tail;
    Node *current;
} LinkedList;

typedef struct _ammInfo AmmInfo;
/*
 *
 * Function pointers
 */
typedef void(*DISPLAY)(void*);
typedef void(*REMOVE)(void*);
typedef int(*COMPARE)(void*, void*);


void addHead(LinkedList *list, void *data);

void initList(LinkedList *list);

void displayAllLinkedList(LinkedList *list, DISPLAY display);

int deleteAllNodesInList(LinkedList *list, REMOVE remove);

int CrosscheckPmonAddy(LinkedList *ksuseList, LinkedList *pmonMapslist);

int removeNoneMatchedNodes(LinkedList *list) ;

void releaseNode(LinkedList *list, Node *node);

Node* getNode(LinkedList *list, void* data);