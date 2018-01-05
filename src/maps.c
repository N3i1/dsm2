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
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "maps.h"
#include <sys/types.h>

#include <alloca.h>

int readContentsOfProcessMapsFile(int target, LinkedList *list){
    FILE *fd; // /proc/<target>/maps
    char name[128], *line = NULL, *match = NULL;
    size_t len;
    Mmaps *maps = NULL;

    snprintf(name, sizeof(name), "/proc/%u/maps", target);

    if((fd = fopen(name, "r")) == NULL){
        printf("error");
        return -1;
    }

    while (getline(&line, &len, fd) != -1) {
    	unsigned long start, end;
        char read, write, exec, cow, *filename;
        int offset, dev_major, dev_minor, inode;
        
        filename = alloca(len);
        memset(filename, '\0', len);
        
        sscanf(line,"%p-%p %c%c%c%c %x %x:%x %u %[^\n]", &start, &end, &read,
        &write, &exec, &cow, &offset, &dev_major, &dev_minor, &inode, filename);
        match = strstr(filename, "/ora_");
        if(match != NULL){
            maps = (Mmaps*)malloc(sizeof(Mmaps));
            maps->name = malloc(sizeof(maps->name));
            strcpy(maps->name, match);
            maps->lowAddr = start;
            maps->highAddr = end;
            maps->keep = 0;
            addHead(list, maps);
            //printf("## After AddHead: Node:%p lowAddr:%p, highAddr:%p and filename:%s\n", maps, maps->lowAddr, maps->highAddr, maps->name);
        }    
    }
    return 0;
}

void displayMmaps(Mmaps *maps){
    printf("## keep: %d lowAddr:%p, highAddr:%p and filename:%s\n", 
    maps->keep, maps->lowAddr, maps->highAddr, maps->name);
}


void setKeep(Mmaps *maps){
    maps->keep = 1;
}

int isKeepSet(Mmaps *maps){
    if(maps->keep == 1){
        //printf("keep: %d lowAddr:%p, highAddr:%p\n", mInfo->keep, mInfo->lowAddr, mInfo->highAddr);
        return 0;
    }
    else{
        return 1;
        //printf("Remove: %d Addr:%p\n", mInfo->keep, mInfo);
    }
}

int mapFileToAddr(Mmaps *maps){
    struct stat sb;
    size_t length = 0;
    int amm_fd = 0;
    int amm_size;
    void* ammAddr = NULL;
    void* ammBase = NULL;
    /*Open file in read only mode*/

    amm_fd = shm_open(maps->name, O_RDONLY, 0666);
    
    if (amm_fd == -1) {
        printf("dsm2: Shared memory failed:\n");
        return -1;
    }
    /*Retrieve meta data about the given file*/
    if (fstat(amm_fd, &sb) == -1) {          
        printf("dsm2: fstat failed:\n");
        return -1;
    }
    /*We use the lowAddr as our point in which to map too*/
    ammAddr = maps->lowAddr;
    length = sb.st_size;
    amm_size = length;
    /*map file into our processes address*/
    ammBase = mmap(ammAddr, amm_size, PROT_READ, MAP_SHARED | MAP_FIXED , amm_fd, 0);
    if (ammBase == MAP_FAILED) {
        printf("dsm2: Map failed:\n");
        printf("Unable to map: %s, Addy: %p \n", maps->name, maps->lowAddr);
        return -1;
    }
    return 0;
}
