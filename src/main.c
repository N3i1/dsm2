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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <time.h>
#include <syslog.h>

#include "ksuse.h"
#include "linker.h"
#include "maps.h"

/*
 * External header file 
 */
#include "ksuseInfo.h"

/*
 * 
 */
#define AMM 0
#define ASMM 0


#if ORA_VER_12101_AMM
    #define AMM 1
#elif ORA_VER_12102_AMM
    #define AMM 1
#elif ORA_VER_12201_AMM
    #define AMM 1
#elif ORA_VER_12101_ASMM
    #define ASMM 1
#elif ORA_VER_12102_ASMM
    #define ASMM 1
#elif ORA_VER_12201_ASMM
    #define ASMM 1
#endif

#define PROMPT "dsm2:>"

#define LINE_BUFF 20

#define FORMAT_SUMMARY_TITLE "%4s %20.30s %35.45s %30s %11s %10s\n"

volatile sig_atomic_t stop;

void sigHandler(int sig);
void showUsage();

int main(int argc, char** argv) {
    
    Ksuse *ksuse;	/* Struct for metadata, mirrors x$ksuse*/
    char line[LINE_BUFF];
    LinkedList ksuse_ll;
    LinkedList pmonFileMaps_ll;
    //struct timespec start={0,0};
    //struct timespec end={0,0};

    int i = 1;
    int lf = 1;
    int* latchFree = &lf;
    void *addrBase = NULL;
    
    initList(&ksuse_ll);
    initList(&pmonFileMaps_ll);
    
    /*
     * Build linked list from KsuseInfo.h
     *
     */
    while(*(users + i) != NULL){
    	/*create struct for each session within the database*/
    	ksuse = (Ksuse*) malloc(sizeof(Ksuse));
        /*Set the values within the struct to defaults*/
    	if ((initKsuse(ksuse)) != 0)
        	printf("Error initSess");
        /*Use the counter "i" to identify SID*/
    	ksuse->sid = i;
    	/*Associate each SID to it's given hex addy*/
    	ksuse->addy = *(users + i);
        /*Add each element into our ksuse linked list*/
        addHead(&ksuse_ll, ksuse);
        i++;
    }
    
    /*Finished with ptr "ksuse", but we'll use this again later*/
    ksuse = NULL;
    //displayALLLinkedList(&ksuse_ll, (DISPLAY)displayKsuseInfo);

    /*
     * Based of the SGA memory model being used we can attach to the SGA in one
     * of two ways.
     *
     * ASMM = Automatic Shared Memory Management - SysV IPC
     * AMM = Automatic Memory Management - POSIX
     *
     */
    if (ASMM == 1){
    	/*
    	 * Map the file containing the SGA metadata into our address space
    	 * */
        
        addrBase=(void *)shmat(SHMID, (void *)SGA_ADDY, SHM_RDONLY);
            if (addrBase == (void *) -1) {
                printf("Shmat error: error attaching to SGA\n");
                return (EXIT_FAILURE);
            }
    }
    else if(AMM == 1){
        /*
         * In order to extract the metadata from a database running AMM we need
         * to do a number of things...
         *
         * We first need to parse /proc/PMON_PID/maps for the Oracle database
         * and extract a number of key variables which we will then add to a 
         * linked list, this is for ease of manipulation later on. Once we have 
         * our linked lists we can then workout which files we need to map 
         * into out process address space in order to see the required metadata.
         *
         */
    	//TODO Split readContentsOfProcessMapsFile into smaller functions
    	/*
    	 * Step 1: Parse and create linked list of key variables
    	 */
        if(readContentsOfProcessMapsFile(PMON_PID ,&pmonFileMaps_ll) != 0) {
            printf("Error, check pmon number!\n");
            exit(EXIT_FAILURE);
        }
        //displayALLLinkedList(&pmonFileMaps_ll, (DISPLAY)displayMmaps);
        /*
         * Step 2: Cross check pmonFileMaps_ll against the starting address
         * of each session, if it's within the ADDR range then we set a keep flag.
         */
        if(CrosscheckNodes(&ksuse_ll, &pmonFileMaps_ll) != 0){
          printf("Error: CrosscheckNodes has failed\n");
            exit(EXIT_FAILURE);
        }
        //displayALLLinkedList(&pmonFileMaps_ll, (DISPLAY)displayMmaps);
        /*
         * Step 3: Trim down pmonFileMaps_ll to only the files we need.
         */
        if(deleteNode(&pmonFileMaps_ll) != 0){
            printf("Error: Cannot delete Node\n");
           exit(EXIT_FAILURE);
       }
        //displayALLLinkedList(&pmonFileMaps_ll, (DISPLAY)displayMmaps);
        /*
         * Step 4: Map the contents remaining within pmonFileMaps_ll into our
         * processes address space.
         */
       if(addMapstoAddySpace(&pmonFileMaps_ll) != 0){
          printf("Error mapping files into addy space\n");
          exit(EXIT_FAILURE);
        }
    }
    else {
        printf("Error: Unknown SGA memory type\n");
        exit(EXIT_FAILURE);
    }
    
    /*
     * If we get to this point then we either have ASMM or AMM file/s mapped
     * into our address space.
     * 
     * Main while loop for prompt
     */

    while(1){
        if (feof(stdin))
            break;
        
        printf(PROMPT);
        fflush(stdout);
        fflush(stdin);
        fgets(line, sizeof(line), stdin);
        
        char command1[LINE_BUFF], command2[LINE_BUFF];
        int sid = 0;
        sscanf(line,"%s %s[^\n]", &command1, &command2);
        
        command1[strlen(line)-1] = '\0';
        command2[strlen(line)-1] = '\0';
        
        int flag;
        
        if(strcmp(command1, "show") == 0){
            /*if((strcmp(command2, "active") == 0))
                flag = 1;
            else if((strcmp(command2, "inactive") == 0))
            	flag = 2;
            else{
            	/*By default we will show ALL session
            	flag =0;
            }*/

            printf(FORMAT_SUMMARY_TITLE, "SID", "EVENT", "PROGRAM", "OSPID", "STATUS", "CONTAINER");

            updateLinkedList(&ksuse_ll, (UPDATE)updateKsuseMetadata, &latchFree);

            //TODO Add in flag handler to handle command2
            displayAllLinkedList(&ksuse_ll, (DISPLAY)printKsuseSummary);
        }
        else if(strcmp(command1, "report") == 0){
            int sid = atoi(command2);
            if(sid == 0){
                printf("Please specify a SID, report <SID>\n");
                continue;
            }
            else{
                /*
                 * Set signal handler: CTL-C will return us back to dsm2 prompt
                 * and not exit the program
                 */
                signal(SIGINT, sigHandler);
                /*Retrieve ptr to a node within our linked list which matches our SID*/
                Node* node = getMatchingNode(&ksuse_ll, (COMPARE)findKsuseBySID, &sid);
                ksuse = node->data;
                /*Update and display metadata*/
                updateKsuseMetadata(ksuse, &latchFree);
                if (lf == 0){
                    printKsuseVerboseLatch(ksuse);
                    lf =1;
                }
                else
                    printKsuseVerbose(ksuse);
                /*Now we go into the reporting loop*/
                //TODO Ctl-C causing can cause segment fault!
                while(!stop){
                    do {
                        updateKsuseMetadata(ksuse, &latchFree);
                    }
                    /* If SEQ# doesn't change then we know there's not been a 
                     * new event, so we can just keep looping and only catch 
                     * changes
                     */
                    while (ksuse->seq == ksuse->pseq);
                        /*Check to ensure our session hasn't ended*/
                        if (ksuse->seq <  ksuse->pseq) {
                            printf("Session has ended"); 
                            //TODO Let's not exist, return to prompt
                            break;
                        }
                        //TODO Allow user to choose delay
                        if (lf == 0){
                            printKsuseVerboseLatch(ksuse);
                            lf =1;
                        }
                        else
                            printKsuseVerbose(ksuse);
                }
            }
        }
        else if(strcmp(command1, "exit") == 0){
            break;
        }
        else if(strcmp(command1, "help") == 0){
            showUsage;
            continue;
        }
        else{
            //TODO create/show helper function
        	//TODO CTL-C handling
            continue;
        }
    }
    
    /*
     Cleanup before exiting
     */
    if (AMM == 1) {
        if( removeMapsFromAddySpace(&pmonFileMaps_ll) != 0){
        printf("Error unmapping AMM");
        }
    }
    else {
        if( shmdt(addrBase) == -1 ){
            printf("Error detaching from ASMM");
        }
    }
    
    if ( deleteNodeInList(&ksuse_ll) != 0){
        printf("Error unmapping ksuse");
    }
    if ( deleteNodeInList(&pmonFileMaps_ll) != 0){
        printf("Error unmapping pmon files");
    }
    printf("Goodbye");
    return (EXIT_SUCCESS);
}

void sigHandler(int sig) {
	stop = 1;
}

void showUsage(){
    printf("Show session summary:\n");
    printf("\tshow active || inactive || default is ALL\n");
    printf("Report on specific session. This shows a very detailed view");
    printf("report SID");
    printf("To exit:\n");
    printf("exit || ctl-c");
}
