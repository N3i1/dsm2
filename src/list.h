

typedef struct _node{
    void *data;
    struct _node *next;
} Node; 


typedef struct _linkedList{
    Node *head;
    Node *tail;
    Node *current;
} LinkedList;


/*
 *
 * Function pointers
 */
typedef void(*DISPLAY)(void*);
typedef void(*REMOVE)(void*);


void addHead(LinkedList *list, void *data);


void initList(LinkedList *list);


void displayAllLinkedList(LinkedList *list, DISPLAY display);


int deleteAllNodesInList(LinkedList *list, REMOVE remove);


