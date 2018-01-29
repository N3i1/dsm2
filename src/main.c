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

#define LINE_BUFF 50
#define PROMPT "dsm2:>"
#define FORMAT_SUMMARY_TITLE "%4s %20.30s %35.45s %30s %11s %10s\n"

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

void sigHandler(int sig);
void showUsage();

volatile sig_atomic_t stop;

int main(int argc, char** argv) {

   int i = 1, reportId = 0, latch = 0;
   int *latchFree = &latch;
   void *addrBase = NULL;
   Ksuse *ksuse;

   LinkedList ksuse_ll;
   LinkedList pmonFileMaps_ll;

   initList(&ksuse_ll);
   initList(&pmonFileMaps_ll);
   char *listenerFor = NULL;

  /*
   * Build linked list from KsuseInfo.h
   */
   if( *(users + i) == NULL) {
      printf("array of pointer id NULL\n");
   }
   else {
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
      if ( addMapstoAddySpace(&pmonFileMaps_ll) != 0 ) {
         printf("Error mapping files into addy space\n");
         exit(EXIT_FAILURE);
      }
    //displayAllLinkedList(&pmonFileMaps_ll, (DISPLAY)displayMmaps);
   }
   else {
      printf("Unsupported memory model");
      exit(EXIT_FAILURE);
   }
   
   while (1) {
      char prompt[LINE_BUFF] = {}, command1[LINE_BUFF] ={}, command2[LINE_BUFF] ={};
      
      fflush(stdout);
      printf(PROMPT);

      if ( fgets(prompt, sizeof(prompt), stdin) == NULL ) {
         printf("Error using fgets\n");
         break;
      }
      char fq, lq;
      sscanf(prompt, "%s %c %[^\"]*s %c", &command1, &fq, &command2, &lq);

      if ( strcmp(command1, "show") == 0 ) {
         printf(FORMAT_SUMMARY_TITLE, "SID", "EVENT", "PROGRAM", "OSPID", "STATUS", "CONTAINER");

         updateLinkedList(&ksuse_ll, (UPDATE)updateKsuseMetadata, &latchFree);

         displayAllLinkedList(&ksuse_ll, (DISPLAY) printKsuseSummary);
      }
      else if (strcmp(command1, "report") == 0) {
         reportId = atoi(&fq);
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
         ksuse = node->data;
         updateKsuseMetadata(ksuse, latchFree);
         if ( latch == 0 ) {
            printKsuseVerboseLatch(ksuse);
            latch = 1;
         }
         else {
            printKsuseVerbose(ksuse);
         }
         /*Now we go into the reporting loop*/
         while (!stop) {
            do {
               updateKsuseMetadata(ksuse, latchFree);
            } while (ksuse->seq == ksuse->pseq); //If SEQ# doesn't change then we know there's not been a new event
         
            //Check to ensure our session hasn't ended
            if (ksuse->seq < ksuse->pseq) {
               break;
            }
            if ( latch == 0 ) {
               printKsuseVerboseLatch(ksuse);          
               latch = 1;
            } 
            else {
               printKsuseVerbose(ksuse);
            }
         }
         stop = 0;
      }
      else if( strcmp(command1, "listen" ) == 0) {
          /*
         Set signal handler: CTL-C will return us back to dsm2 prompt
         and not exit the program
         */
         signal(SIGINT, sigHandler);
         while (!stop) {
            updateLinkedList(&ksuse_ll, (UPDATE)updateKsuseMetadata, &latchFree);
            Node* node = getMatchingNode(&ksuse_ll, (COMPARE)findListenFor, &command2);
            if( node != NULL ){
               ksuse = node->data;
               int h = 0;
               for(h=0; h < 5; h++){
                  updateKsuseMetadata(ksuse, latchFree);
                  if ( latch == 0 ) {
                     printKsuseVerboseLatch(ksuse);
                     latch = 1;
                  }
                  else {
                     printKsuseVerbose(ksuse);
                  }
               }
            }
            break;
         }
         stop = 0;
      }
      else if ( strcmp(command1, "exit" ) == 0) {
         break;
      } 
      else if ( strcmp(command1, "help" ) == 0) {
         showUsage();
         continue;
      } 
      else {
         printf("run help, nothing entered\n");
         continue;
      }
   }

  /*
    Memory cleanup
  */
   //free(ksuse);
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
   printf("\tshow \n");
   printf("Report on specific session. This shows a very detailed view\n");
   printf("\treport SID\n");
   printf("Report only for specific wait event\n");
   printf("\tlisten \"wait event name\"\n ");
   printf("To exit:\n");
   printf("exit || ctl-c\n");
}
