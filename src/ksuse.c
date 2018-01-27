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