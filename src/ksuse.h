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

typedef struct _ammInfo {
        int* ammLowAddy;
        int* ammHighAddy;
        char* ammName;
        int inUse;
} AmmInfo;


typedef struct maps Mmaps;


typedef struct _node Node; 


typedef struct _linkedList LinkedList;

/*
 *
 * _ksuse mirrors x$ksuse
 *
 */
typedef struct _ksuse {
        int sid;
	void *addy;
	unsigned short seq;	
	unsigned short pseq;  
	char *unm;
	unsigned int pid;	
	char *opc;  
	short opcnum;	
	int ksledClassid; 
	char *ksledClassName; 
	char* oct;
        char *octName;
	char *lna;    
	char *pnm;	
	char *opcP1;    
	long long p1;   
	char *opcP2; 
	long long p2;  
	char *opcP3;   
	long long p3;    
	char *latch;
	long long sql;  
	unsigned int sqh;
	long long psq;
	unsigned int pha;
	int fil;  
	long long blk;	   
	unsigned short slt;   
	unsigned short obj;
	int idl;
        int flg;
	int lui;    
        char* state;
        char* con;
} Ksuse;

 
void displayAmmMmaps(AmmInfo *mInfo);


int checkforMmaps(Mmaps *maps, AmmInfo *mInfo);


AmmInfo* AddMaps(Mmaps *maps);

/**
 * Used to check pmons files gainst session addr 
 * 
 * @param s Ksuse data
 * @param mInfo Mmaps data
 * @return 
 */
int compareAddy(Ksuse *s, Mmaps *mInfo);
  

int compareSessionAddy(Ksuse *s1, Ksuse *s2);

/**
 * Prints values for struct Ksuse
 *
 *  * @param ksuse	Struct Ksuse
 *
 */
void displayKsuseInfo(Ksuse *ksuse);

/**
 * 
 * @return Ptr to Session
 */
int initKsuse(Ksuse* ksuse);

/**
 * 
 * @param start
 * @param end
 */
void diff(struct timespec start, struct timespec end);

/**
 *
 * Finds a given SID within a linked list
 *
 * @param ksuse_ll
 * @param ksuse - SID you wish to find
 */
int findKsuseBySID(Ksuse* ksuse_ll, Ksuse* ksuse);


Ksuse* getSessDetails(Node* sessInfo, unsigned int* latchFree);

/**
 * Updates the metadata
 * 
 * @param ksuse - node we wish to update
 * @param latchFree - flag which is set to help identify if P2 vale is displayed as
 *                     an int of string
 */
void updateKsuseMetadata(Ksuse* ksuse, unsigned int* latchFree);

/**
 * 
 * @param ksuse
 */
void printKsuseSummary(Ksuse* ksuse);


void printKsuseVerboseLatch(Ksuse* ksuse);

void printKsuseVerbose(Ksuse* ksuse);



void deallocSessStruct(Ksuse* ksuse);

/**
 * 
 * @param ksuse
 */
void deallocSessNode( Ksuse* ksuse);

