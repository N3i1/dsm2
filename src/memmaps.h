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

typedef struct _maps {
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
int releaseNodeKsuse(Ksuse *ksuse);
void displayMmaps(Mmaps *maps);
int isKeepSet(Mmaps *maps);
void setKeep(Mmaps *maps);
int mapFileToAddr(Mmaps *maps);