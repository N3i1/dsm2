typedef struct maps {
    char* fileName;
    unsigned long lowAddr;
    unsigned long highAddr;
    int keep;
    int length;
} Mmaps;

typedef struct _linkedList LinkedList;
typedef struct _ksuse Ksuse;
int readContentsOfProcessMapsFile(int target, LinkedList *list);

void releaseNodeMmaps(Mmaps *maps);
void releaseNodeKsuse(Ksuse *ksuse);

void displayMmaps(Mmaps *maps);