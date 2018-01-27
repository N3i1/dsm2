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
#include <time.h>

#include "ksuse.h"
#include "memmaps.h"
#include "list.h"

void initKsuse(Ksuse* ksuse) {
  ksuse->sid = 0;
  ksuse->addy = NULL;
  ksuse->longAddy = 0;
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
}


int compareAddy(Ksuse *ksuse, Mmaps *maps) {
  if ( ksuse->longAddy >= maps->lowAddr && ksuse->longAddy <= maps->highAddr )
  {
    return(EXIT_SUCCESS);
  } 
  else
  {
    return(EXIT_FAILURE);
  }
}

void displayKsuseInfo(Ksuse *ksuse) {//, AmmInfo *mInfo){
  //if(a_option  == NULL){
  //if (s->seq > 0) { 
  printf("addy:%p longAddy:%ul\n", ksuse->addy, ksuse->longAddy);
  }