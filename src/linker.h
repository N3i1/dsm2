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
typedef int(*COMPARE)(void*, void*);
typedef void(*RELEASE)(void*);
typedef void(*UPDATE)(void*);

/**
 * Traverse linked list and call update function
 *
 * @param list      struct LinkedList
 * @param display   Function pointer (UPDATE)
.
 */
void updateLinkedList(LinkedList *list, UPDATE update, unsigned int* latchFree);


/**
 * Finds and returns the first matching element 
 *
 * @param list      struct LinkedList
 * @param compare   Function pointer (COMPARE)
 * @param data      Pointer to ..
 * 
 *
 * @return      Node struct of matching data
 */
Node *getFirstMatchingNode(LinkedList *list, COMPARE compare, void *data);

/**
 * Crosschecks *list(Session->addy) against the *data(AmmInfoSess->ammlowAddr) &&
 * (AmmInfoSess->ammhighAddr) of a given /dev/shm/..file. If Session->addr is 
 * within the range of the /dev/shm file then we will add it into *listm(linkedMaps)
 * Only one unique reference to a /dev/shm/..file is is added into *listm(linkedMaps)
 *
 * @param   list      LinkedList of database Sessions.
 * @param   listm     LinkedMaps of pmons mmaps files.
 * @param   listmm    LinkedList for our unique mmaps file we need.
 * 
 */
int CrosscheckNodes(LinkedList *sessionlist, LinkedList *pmonMapslist);

/**
 * Display ALL elements in given list
 * of /dev/shm files which are needed by Sessions[] 
 *
 * @param   list      LinkedList of Sessions[].
 * @param   display   Function pointer (DISPLAY).
 * 
 */
void displayAllLinkedList(LinkedList *list, DISPLAY display);

/**
 * Adds data into list
 *
 * @param   list      LinkedList struct.
 * @param   data      pointer to data type.
 */
void addHead(LinkedList *list, void *data);

/**
 * inits LinkedList struct
 *
 * @param   list      LinkedList struct.
 */
void initList(LinkedList *list);

/**
 * 
 * @param list
 * @return 
 */
int addMapstoAddySpace(LinkedList *list);

/**
 * 
 * @param list
 * @return 
 */

int removeMapsFromAddySpace(LinkedList *list);

/**
 * Free's memory for a given Linked Lists
 *
 * @param list
 * @param RELEASE ptr to function which releases Node
 */
void releaseLinkedList(LinkedList *list, RELEASE release);

/**
 * Free's memory for a given Node
 * @param list
 * @param node
 * @return SUCCESS or FAILURE
 */
int deleteNode(LinkedList *pmonMapslist);

int deleteNodeInList(LinkedList *list);