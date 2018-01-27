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

#include "list.h"
#include "memmaps.h"
#include "ksuse.h"

#define LINE_BUFF 20
#define PROMPT "dsm2:>"
#define PMON_PID 2974

const int *users[]={
0x0000000073EC55E0,
0x0000000073EC3028,
0x0000000073EC0A70,
0x0000000073EBE4B8,
0x0000000073EBBF00,
0x0000000073EB9948,
0x0000000073EB7390,
0x0000000073EB4DD8,
0x0};

int main(int argc, char** argv) {
  
  char prompt[LINE_BUFF], command1[LINE_BUFF], command2[LINE_BUFF];
  int i;
  Ksuse *ksuse;

  LinkedList ksuse_ll;
  LinkedList pmonFileMaps_ll;

  initList(&ksuse_ll);
  initList(&pmonFileMaps_ll);

  /*
   * Build linked list from KsuseInfo.h
   */

  while ( *(users + i) != NULL ) 
  {
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

  ksuse = NULL;
   
  if ( readContentsOfProcessMapsFile(PMON_PID, &pmonFileMaps_ll ) != 0 ) 
  {
      printf("Error, check pmon number!\n");
      exit(EXIT_FAILURE);
  }
   // displayAllLinkedList(&pmonFileMaps_ll, (DISPLAY)displayMmaps);
  if ( CrosscheckNodes(&ksuse_ll, &pmonFileMaps_ll) != 0 ) 
  {
      printf("Error: CrosscheckNodes has failed\n");
      exit(EXIT_FAILURE);
  }

  
  while (1)
  {
    fflush(stdout);
    printf(PROMPT);

    if ( fgets(prompt, sizeof(prompt), stdin) == NULL )
    {
      printf("Error using fgets\n");
      exit(EXIT_FAILURE);
    }
    
    sscanf(prompt, "%s %s", &command1, &command2);

    if (strcmp(command1, "show") == 0) 
    {
      printf("run show\n");
    } 
    else if (strcmp(command1, "report") == 0) 
    {
      printf("run report\n");
      continue;
    }
    else if (strcmp(command1, "exit") == 0) 
      {
        break;
      } 
    else if (strcmp(command1, "help") == 0) 
    {
      printf("run help\n");
      continue;
    } 
    else 
    {
      printf("run help, nothing entered");
      continue;
    }
  }
    
  if (deleteAllNodesInList(&pmonFileMaps_ll, (REMOVE) releaseNodeMmaps) != 0)
    {
        printf("Error unmapping pmon files");
    }
    
    if (deleteAllNodesInList(&ksuse_ll, (REMOVE) releaseNodeMmaps) != 0)
    {
        printf("Error unmapping ksuse struct");
    }

    free(ksuse);

return (EXIT_SUCCESS);

}



