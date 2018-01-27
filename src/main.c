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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

#include "list.h"
#include "memmaps.h"
#include "ksuse.h"

#include "ksuseInfo.h"

#define LINE_BUFF 20
#define PROMPT "dsm2:>"
#define PMON_PID 3211

#define SHMID 0
#define SGA_ADDY 0
 
#define AMM 1
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

void sigHandler(int sig);
void showUsage();

volatile sig_atomic_t stop;

int main(int argc, char** argv) {

  char prompt[LINE_BUFF], command1[LINE_BUFF], command2[LINE_BUFF];
  int i = 1, reportId = 0, latch = 0;
  void *addrBase = NULL;
  Ksuse *ksuse, *reportUser;

  LinkedList ksuse_ll;
  LinkedList pmonFileMaps_ll;

  initList(&ksuse_ll);
  initList(&pmonFileMaps_ll);

  /*
   * Build linked list from KsuseInfo.h
   */

  while ( *(users + i) != NULL ) {
    /*create struct for each session within the database*/
    ksuse = (Ksuse*) malloc(sizeof (Ksuse));
    /*Set the values within the struct to defaults*/
    initKsuse(ksuse);
    /*Use the counter "i" to identify SID*/
    ksuse->sid = i;
    /*Associate each SID to it's given hex addy*/
    ksuse->addy = *(users + i);
    ksuse->longAddy = (unsigned long) *(users + i);
    /*Add each element into our ksuse linked list*/
    addHead(&ksuse_ll, ksuse);
    i++;
  }

  //displayAllLinkedList(&ksuse_ll, (DISPLAY)displayKsuseInfo);
  ksuse = NULL;
   
  

  if ( ASMM == 1 ) {
    addrBase = (void *) shmat(SHMID, (void *)SGA_ADDY, SHM_RDONLY );

    if ( addrBase == (void *) -1 ) {
      printf("Shmat error: Issue attachin to SGA_ADDY: %p \n", SGA_ADDY);
      return(EXIT_FAILURE);
    }
  }
  else if ( AMM == 1) {
    if ( readContentsOfProcessMapsFile(PMON_PID, &pmonFileMaps_ll ) != 0 ) {
      printf("Error, check pmon number!\n");
      exit(EXIT_FAILURE);
    }
   //displayAllLinkedList(&pmonFileMaps_ll, (DISPLAY)displayMmaps);
    if ( crosscheckPmonAddy(&ksuse_ll, &pmonFileMaps_ll) != 0 ) {
      printf("Error: Cross checking of memory addresses has failed\n");
      exit(EXIT_FAILURE);
    }
    //displayAllLinkedList(&pmonFileMaps_ll, (DISPLAY)displayMmaps);
    if ( removeNoneMatchedNodes(&pmonFileMaps_ll) != 0 ) {
      printf("Error: Cannot delete Node\n");
      exit(EXIT_FAILURE);
    }
    //displayAllLinkedList(&pmonFileMaps_ll, (DISPLAY)displayMmaps);
  }

  while (1) {
    fflush(stdout);
    printf(PROMPT);

    if ( fgets(prompt, sizeof(prompt), stdin) == NULL ) {
      printf("Error using fgets\n");
      exit(EXIT_FAILURE);
    }
    
    sscanf(prompt, "%s %s", &command1, &command2);

    if (strcmp(command1, "show") == 0) {
      printf("run show\n");
    } 
    else if (strcmp(command1, "report") == 0) {
      printf("run report\n");
      reportId = atoi(command2);
      if ( reportId == 0 ) {
        printf("Plese enter SID\n");
        continue;
      }
      /*
      Set signal handler: CTL-C will return us back to dsm2 prompt
      and not exit the program
      */
      signal(SIGINT, sigHandler);
      /*Retrieve ptr to a node within our linked list which matches our SID*/
      Node* node = getMatchingNode(&ksuse_ll, (COMPARE)findKsuseBySID, &reportId);
      reportUser = node->data;
      /*Now we go into the reporting loop*/
      /*while (!stop) {
      
        do {
            printf("Update ksuse\n");
        }
          //If SEQ# doesn't change then we know there's not been a new event
           
          while (ksuse->seq == ksuse->pseq);

          //Check to ensure our session hasn't ended
        if (ksuse->seq < ksuse->pseq) {
          break;
        }
        if ( latch == 0 ) {
          
          latch = 1;
        } 
        else {
            continue;
        }
      }*/
    }
    else if (strcmp(command1, "exit") == 0) {
        break;
    } 
    else if (strcmp(command1, "help") == 0) {
      printf("run help\n");
      continue;
    } 
    else {
      printf("run help, nothing entered");
      continue;
    }
  }

  /*
    Memory cleanup
  */
  free(ksuse);
  if ( deleteAllNodesInList(&pmonFileMaps_ll, (REMOVE) releaseNodeMmaps) != 0 ) {
    printf("Error unmapping pmon files");
  }
    
  if ( deleteAllNodesInList(&ksuse_ll, (REMOVE) releaseNodeKsuse) != 0 ) {
      printf("Error unmapping ksuse struct");
  }




return (EXIT_SUCCESS);

}


void sigHandler(int sig) {
  stop = 1;
}

void showUsage() {
  printf("Show session summary:\n");
  printf("\tshow active || inactive || default is ALL\n");
  printf("Report on specific session. This shows a very detailed view");
  printf("report SID");
  printf("To exit:\n");
  printf("exit || ctl-c");
}
