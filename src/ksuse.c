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
#include <string.h>
#include <time.h>

#include "ksuse.h"
#include "maps.h"
#include "linker.h"



#if ORA_VER_12101_AMM
    #include "12101.h"
#elif ORA_VER_12102_AMM
    #include "12102.h"
#elif ORA_VER_12201_AMM
    #include "12201.h"
#elif ORA_VER_12101_ASMM
    #include "12101.h"
#elif ORA_VER_12102_ASMM
    #include "12102.h"
#elif ORA_VER_12201_ASMM
    #include "12201.h"
#endif

#define FORMAT_VERBOSE_LATCH "Waiting for: '%s' P1:'%s'=%lld P2:'%s'=%s P3:%s'=%lld\n\t" \
    "command=%s ospid=%d sqlhv=%u psqlhv=%u Class:%s\n"


#define FORMAT_VERBOSE "Waiting for: '%s' P1:'%s'=%lld P2:'%s'=%lld P3:'%s'=%lld\n\t" \
    "command=%s ospid=%d sqlhv=%u psqlhv=%u Class:%s\n"


#define FORMAT_SUMMARY_TITLE "%4s %20.30s %35.45s %30s %11s %10s\n"


#define FORMAT_SUMMARY "%4d %-35.33s %-41.50s %10d %10s %10s\n"


void displayAmmMmaps(AmmInfo *mInfo){
    printf("Base:%p lowAddr:%p, highAddr:%p and filename:%s\n", mInfo, mInfo->ammLowAddy, mInfo->ammHighAddy, mInfo->ammName);
}


int findKsuseBySID(Ksuse* ksuse_ll, Ksuse* ksuse){
    if( ksuse_ll->sid  == ksuse->sid){
        return 0;
    }
    else
        return -1;

}


int checkforMmaps(Mmaps *maps, AmmInfo *mInfo){
    if (strcmp(mInfo->ammName, maps->name) == 0)
        return 0;
    else{ return 1;}
}


AmmInfo* AddMaps(Mmaps *maps){
    AmmInfo* mInfo = (AmmInfo*) malloc(sizeof(struct _ammInfo));
    mInfo->ammName = malloc(sizeof(maps->name));
    strcpy(mInfo->ammName, maps->name);
    mInfo->ammLowAddy = maps->lowAddr;
    mInfo->ammHighAddy = maps->highAddr;
    //printf("Inside AddMaps:%p..lowAddr:%p hAddr:%p and File:%s\n", &mInfo, mInfo->ammLowAddy, mInfo->ammHighAddy, mInfo->ammName);
    return mInfo;
}


int compareSessionAddy(Ksuse *s1, Ksuse *s2){
    return (s1->addy == s2->addy);
        //return strcmp(s1->addy, s2->addy);
}


int compareAddy(Ksuse *s, Mmaps *mInfo){
    if( s->addy >= mInfo->lowAddr && s->addy <= mInfo->highAddr){
        //printf("Session:%p\n", s->addy);
        //printf("Match found %s\n", mInfo->name);
        return 0;
    }
    else
        return -1;
}


void displayKsuseInfo(Ksuse *s){//, AmmInfo *mInfo){
    //if(a_option  == NULL){
        //if (s->seq > 0) { 
    printf("addy:%p\n", s->addy);
           // printSessSumView(s);                              
        //}
        /*else if (strcmp(a_option, "ACTIVE") == 0) { 
            if (s->seq > 0 && s->lui == 1) { 
                printSessSumView(s);                              
            }
        }
        else if(strcmp(a_option, "INACTIVE" ) == 0){
            if (s->seq > 0 && s->lui == 0) { 
                printSessSumView(s);                              
            }
        }
    }*/
}

int initKsuse(Ksuse* ksuse) {
    ksuse->sid = 0;
    ksuse->addy = NULL;
    ksuse->seq = 0;
    ksuse->pseq = 0;
    ksuse->pnm = NULL;
    ksuse->unm = NULL;
    ksuse->p1 = 0;
    ksuse->p2 = 0;
    ksuse->p3 = 0;
    ksuse->opcnum = 0;
    ksuse->opc = NULL;
    ksuse->opcP1 = NULL;
    ksuse->opcP2 = NULL;
    ksuse->opcP3 = NULL;
    ksuse->ksledClassid = 0;
    ksuse->oct = 0;
    ksuse->octName = NULL;
    ksuse->pid = 0;
    ksuse->sql = 0;
    ksuse->sqh = 0;
    ksuse->psq = 0;
    ksuse->pha = 0;
    ksuse->latch = NULL;
    ksuse->idl = 0;
    ksuse->flg = 0;
    ksuse->lui = 0;
    ksuse->state = NULL;
    ksuse->con = NULL;
    return 0;
    }


void updateKsuseMetadata(Ksuse* ksuse, unsigned int* latchFree){
    const char sessionState[][10]={"INACTIVE", "ACTIVE", "SNIPED", "SNIPED", "KILLED"};
    /*
     * Capture previous SEQ# 
     */
    ksuse->pseq = ksuse->seq;
    /*SEQ#*/
    ksuse->seq = *(unsigned short*) (ksuse->addy+KSUSESEQ);
    /*Program*/
    ksuse->pnm = (ksuse->addy+KSUSEPNM);
    /*Username*/
    ksuse->unm = (ksuse->addy+KSUSEUNM);
    /*P1, P2 and P3*/
    ksuse->p1 = *(long long*) (ksuse->addy+KSUSEP1);
    ksuse->p2 = *(long long*) (ksuse->addy+KSUSEP2);
    ksuse->p3 = *(long long*) (ksuse->addy+KSUSEP3);
    /*Event number*/
    ksuse->opcnum = (*(short*)(ksuse->addy+KSUSEOPC));
    /*Event name in string format*/
    ksuse->opc = sessKsledInfo[ksuse->opcnum].ksledNam;
    /*P1, P2 and P3 values*/
    ksuse->opcP1 = sessKsledInfo[ksuse->opcnum].ksledP1;
    ksuse->opcP2 = sessKsledInfo[ksuse->opcnum].ksledP2;
    if (strcmp(ksuse->opc, "latch free" ) == 0) {
    	ksuse->latch= *(latch + (ksuse->p2));
        /*Set latchfree so we know to print P2 as a string*/
        *latchFree = 0;
    }
    ksuse->opcP3 = sessKsledInfo[ksuse->opcnum].ksledP3;
    /*Event class id*/
    ksuse->ksledClassid = sessKsledInfo[ksuse->opcnum].ksledclassid;
    /*Event class value*/
    ksuse->ksledClassName = *(waitClassGroup + (ksuse->ksledClassid) );
    /*Command*/
    ksuse->oct = (ksuse->addy+KSUUDOCT);
    ksuse->octName = *(command + *(ksuse->oct));
    /*OS PID*/
    ksuse->pid = strtol(ksuse->addy+KSUSEPID, NULL, 0);
    /**/
    ksuse->sql = *(long long *) (ksuse->addy+KSUSESQL);
    /*SQL Hash Value*/
    ksuse->sqh = *(int*) (ksuse->addy+KSUSESQH);
    /*Previous SQL Hash Value*/
    ksuse->psq = *(long long *) (ksuse->addy+KSUSEPSQ);
    ksuse->pha = *(int*) (ksuse->addy+KSUSEPHA);
    ksuse->idl = *(int*) (ksuse->addy+KSUSEIDL);
    ksuse->flg = *(int*) (ksuse->addy+KSUSEFLG);
    /*
     * Find session current STATE
     */
    //TODO Break this into a function
    int n = 0;
    n = ksuse->idl & 11;
    if (n == 1) {
    	ksuse->lui = 1; // ACTIVE
    }
    else {
        n = 0;
        n = ksuse->flg & 4096;
        switch (n) {
        case 0: 
        	ksuse->lui = 0; // INACTIVE
            break;
        case 1: 
        	ksuse->lui = 1; //ACTIVE
            break;
        case 2: 
        	ksuse->lui =2; //SNIPED
            break;
        case 3: 
        	ksuse->lui =3; //SNIPED
            break;
        default: ksuse->lui =4; //KILLED
        }
    }
    /*Session State*/
    ksuse->state = *(sessionState + (ksuse->lui));
    /*Container name*/
    ksuse->con = (ksuse->addy+KSUUDCON);
}

/*void diff( struct timespec start, struct timespec end) {
    struct timespec temp={0,0};
    if ((end.tv_nsec - start.tv_nsec) <0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    }
    else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    printf("Time between wait %5lld.%010ld secs \n ", (long long)temp.tv_sec, temp.tv_nsec ); 
}*/

void printKsuseSummary(Ksuse* ksuse) {
    if(ksuse->seq != 0){
    printf(FORMAT_SUMMARY,
    		ksuse->sid,
		ksuse->opc,
		ksuse->pnm,
		ksuse->pid,
		ksuse->state,
		ksuse->con);
    }
}


void printKsuseVerboseLatch(Ksuse* ksuse) {
    printf(FORMAT_VERBOSE_LATCH,
	ksuse->opc,
	ksuse->opcP1,
	ksuse->p1,
	ksuse->opcP2,
	ksuse->latch,
	ksuse->opcP3,
	ksuse->p3,
	ksuse->octName,
	ksuse->pid,
	ksuse->sqh,
	ksuse->pha,
	ksuse->ksledClassName);
}


void printKsuseVerbose(Ksuse* ksuse) {
    printf(FORMAT_VERBOSE,
	ksuse->opc,
	ksuse->opcP1,
	ksuse->p1,
	ksuse->opcP2,
	ksuse->p2,
	ksuse->opcP3,
	ksuse->p3,
	ksuse->octName,
	ksuse->pid,
	ksuse->sqh,
	ksuse->pha,
	ksuse->ksledClassName);
}

/*void deallocSessStruct( Ksuse* ksuse) {
    free(ksuse);
}

void deallocSessNode( Ksuse* ksuse) {
    free(ksuse);
}*/


