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
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#include "memmaps.h"

int readContentsOfProcessMapsFile(int target, LinkedList *list){
  
  FILE *fd; // /proc/<target>/maps
  char name[128], *line = NULL, *match = NULL;
  size_t len = 0;
  char* filename = NULL;
  unsigned long start, end;
  char read, write, exec, cow;
  int offset, dev_major, dev_minor, inode;
  Mmaps *maps = NULL;
  
  snprintf(name, sizeof (name), "/proc/%u/maps", target);
  
  if ( (fd = fopen(name, "r") ) == NULL) 
  {
    printf("error");
    return (EXIT_FAILURE);
  }
  
  while( getline(&line, &len, fd) != -1) 
  {
    filename = realloc(filename, len);

    sscanf(line, "%p-%p %c%c%c%c %x %x:%x %u %[^\n]", &start, &end, &read,
            &write, &exec, &cow, &offset, &dev_major, &dev_minor, &inode, filename);
    
    match = strstr(filename, "/ora_");
    if( match != NULL ) {

      maps = (Mmaps*) malloc(sizeof(Mmaps));
      /*
      Address 0x51fa838 is 0 bytes after a block of size 8 alloc'd 
      Address 0x51fa842 is 10 bytes after a block of size 8 alloc'd
      Added char 8 to fileName...
      */
      maps->fileName = malloc(sizeof(match)*(char)+8);  
      strcpy(maps->fileName, match);
      maps->lowAddr = start;
      maps->highAddr = end;
      maps->keep = 0;
      maps->length = 0;
      addHead(list, maps);
    }
  }
  
  if ( fclose(fd) == -1 ) 
  {
    printf("Failed to close\n");
    return (EXIT_FAILURE);
  }

  free(match);
  free(filename);
  free(line);
  
  return (EXIT_SUCCESS);
}

void releaseNodeMmaps(Mmaps *maps) {
  free(maps->fileName);
}

void releaseNodeKsuse(Ksuse *ksuse) {
  return 0;
}


void displayMmaps(Mmaps *maps) {
  printf("## keep: %d lowAddr:%p, highAddr:%p and filename:%s\n",
          maps->keep, maps->lowAddr, maps->highAddr, maps->fileName);
}


void setKeep(Mmaps *maps) {
  maps->keep = 1;
}


int isKeepSet(Mmaps *maps) {
  if (maps->keep == 1) 
  {
    //printf("keep: %d lowAddr:%p, highAddr:%p\n", mInfo->keep, mInfo->lowAddr, mInfo->highAddr);
    return (EXIT_SUCCESS);
  } 
  else 
  {
    return (EXIT_FAILURE);
    //printf("Remove: %d Addr:%p\n", mInfo->keep, mInfo);
  }
}
