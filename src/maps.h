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

/*
 * Information returned from mmap
 */
typedef struct maps {
    char *name;
    unsigned long lowAddr;
    unsigned long highAddr;
    int keep;
} Mmaps;

typedef struct _linkedList LinkedList;

/**
 * Reads, parses and creates a linked lists from the contents of /proc/PMON_PID/maps.
 * 
 * End result, example:
 * 
 * lowAddr:0xd6000000, highAddr:0xd7000000 and filename:/ora_AMMCDB_9404417_117
 * 
 * @param   target:  The ID of the Process.
 * @param   LinkedList: List to populate
 * 
 * @returns 0 on success, -1 on failure
 */

int readContentsOfProcessMapsFile(int target, LinkedList *list);


/**
 * Displays the content of Mmaps struct  

 * 
 * @param   struct Mamps 
 *
 */
void displayMmaps(Mmaps *maps);

/**
 * Set the KEEP flag with Node Mmaps
 *
 * @param mInfo
 */
void setKeep(Mmaps *maps);

/**
 * Checks to see if KEEP flag is set
 *
 * @param mInfo 
 * @return 0 on success
 */
int isKeepSet(Mmaps *maps);

/**
 * Maps given file into address space
 * 
 * @param maps
 * @return 
 */
int mapFileToAddr(Mmaps *maps);