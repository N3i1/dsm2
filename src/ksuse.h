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

typedef struct _ksuse {
   int sid;
   void *addy;
   unsigned long longAddy;
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

typedef struct _maps Mmaps;

void initKsuse(Ksuse* ksuse);
int compareAddy(Ksuse *ksuse, Mmaps *maps);
void displayKsuseInfo(Ksuse *ksuse);
int findKsuseBySID(Ksuse* ksuse_ll, Ksuse* ksuse);
void updateKsuseMetadata(Ksuse* ksuse, unsigned int* latchFree);
void printKsuseVerboseLatch(Ksuse* ksuse);
void printKsuseVerbose(Ksuse* ksuse);
void printKsuseSummary(Ksuse* ksuse);
int findListenFor(Ksuse* ksuse_ll, char* lookinFor);