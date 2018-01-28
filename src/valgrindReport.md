[oracle@localhost dsm2_code_review]$ valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./dsm2_code
==18907== Memcheck, a memory error detector
==18907== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==18907== Using Valgrind-3.12.0 and LibVEX; rerun with -h for copyright info
==18907== Command: ./dsm2_code
==18907==


dsm2:>show


 SID                EVENT                             PROGRAM                          OSPID      STATUS  CONTAINER
==18907== Conditional jump or move depends on uninitialised value(s)
==18907==    at 0x50866E4: vfprintf (in /usr/lib64/libc-2.17.so)
==18907==    by 0x508F8C8: printf (in /usr/lib64/libc-2.17.so)
==18907==    by 0x4015B8: printKsuseSummary (ksuse.c:235)
==18907==    by 0x40167E: displayAllLinkedList (list.c:48)
==18907==    by 0x401D40: main (main.c:142)
==18907==
==18907== Conditional jump or move depends on uninitialised value(s)
==18907==    at 0x50B46C1: _IO_file_xsputn@@GLIBC_2.2.5 (in /usr/lib64/libc-2.17.so)
==18907==    by 0x5086580: vfprintf (in /usr/lib64/libc-2.17.so)
==18907==    by 0x508F8C8: printf (in /usr/lib64/libc-2.17.so)
==18907==    by 0x4015B8: printKsuseSummary (ksuse.c:235)
==18907==    by 0x40167E: displayAllLinkedList (list.c:48)
==18907==    by 0x401D40: main (main.c:142)
==18907==
==18907== Conditional jump or move depends on uninitialised value(s)
==18907==    at 0x50B46CF: _IO_file_xsputn@@GLIBC_2.2.5 (in /usr/lib64/libc-2.17.so)
==18907==    by 0x5086580: vfprintf (in /usr/lib64/libc-2.17.so)
==18907==    by 0x508F8C8: printf (in /usr/lib64/libc-2.17.so)
==18907==    by 0x4015B8: printKsuseSummary (ksuse.c:235)
==18907==    by 0x40167E: displayAllLinkedList (list.c:48)
==18907==    by 0x401D40: main (main.c:142)
==18907==
==18907== Syscall param write(buf) points to uninitialised byte(s)
==18907==    at 0x5127840: __write_nocancel (in /usr/lib64/libc-2.17.so)
==18907==    by 0x50B3FB2: _IO_file_write@@GLIBC_2.2.5 (in /usr/lib64/libc-2.17.so)
==18907==    by 0x50B541B: _IO_do_write@@GLIBC_2.2.5 (in /usr/lib64/libc-2.17.so)
==18907==    by 0x50B4650: _IO_file_xsputn@@GLIBC_2.2.5 (in /usr/lib64/libc-2.17.so)
==18907==    by 0x5085984: vfprintf (in /usr/lib64/libc-2.17.so)
==18907==    by 0x508F8C8: printf (in /usr/lib64/libc-2.17.so)
==18907==    by 0x4015B8: printKsuseSummary (ksuse.c:235)
==18907==    by 0x40167E: displayAllLinkedList (list.c:48)
==18907==    by 0x401D40: main (main.c:142)
==18907==  Address 0x4022062 is in a rw- anonymous segment
==18907==
 357 Space Manager: slave idle wait      oracle@localhost.localdomain (W000)             9577     ACTIVE   CDB$ROOT
 356 Streams AQ: qmn coordinator idle    oracle@localhost.localdomain (QM02)             3114     ACTIVE   CDB$ROOT
 353 Space Manager: slave idle wait      oracle@localhost.localdomain (W004)             4708     ACTIVE   CDB$ROOT
 351 Streams AQ: qmn slave idle wait     oracle@localhost.localdomain (Q003)             3121     ACTIVE   CDB$ROOT
 350 AQPC idle                           oracle@localhost.localdomain (AQPC)             2876     ACTIVE   CDB$ROOT
 348 Data Guard: Gap Manager             oracle@localhost.localdomain (TT00)             2869     ACTIVE   CDB$ROOT
 347 rdbms ipc message                   oracle@localhost.localdomain (PXMN)             2842     ACTIVE   CDB$ROOT
 345 Space Manager: slave idle wait      oracle@localhost.localdomain (W003)            17913     ACTIVE   CDB$ROOT
 344 LGWR worker group idle              oracle@localhost.localdomain (LG01)             2830     ACTIVE   CDB$ROOT
 343 rdbms ipc message                   oracle@localhost.localdomain (CKPT)             2824     ACTIVE   CDB$ROOT
 342 DIAG idle wait                      oracle@localhost.localdomain (DIA0)             2817     ACTIVE   CDB$ROOT
 341 VKRM Idle                           oracle@localhost.localdomain (VKRM)             2811     ACTIVE   CDB$ROOT
 340 DIAG idle wait                      oracle@localhost.localdomain (DIAG)             2803     ACTIVE   CDB$ROOT
 339 rdbms ipc message                   oracle@localhost.localdomain (DBRM)             2809     ACTIVE   CDB$ROOT
 338 VKTM Logical Idle Wait              oracle@localhost.localdomain (VKTM)             2789     ACTIVE   CDB$ROOT
 337 pmon timer                          oracle@localhost.localdomain (PMON)             2782     ACTIVE   CDB$ROOT
 188 PX Deq: Slave Session Stats         oracle@localhost.localdomain (P002)             2880   INACTIVE   CDB$ROOT
 187 PX Deq: Slave Session Stats         oracle@localhost.localdomain (P001)             2867   INACTIVE   CDB$ROOT
 186 Space Manager: slave idle wait      oracle@localhost.localdomain (W005)             4773     ACTIVE   CDB$ROOT
 183 class slave wait                    oracle@localhost.localdomain (M001)            18711   INACTIVE   CDB$ROOT
 182 heartbeat redo informer             oracle@localhost.localdomain (TT02)             2873     ACTIVE   CDB$ROOT
 181 rdbms ipc message                   oracle@localhost.localdomain (TMON)             2854     ACTIVE   CDB$ROOT
 180 rdbms ipc message                   oracle@localhost.localdomain (MMON)             2846     ACTIVE   CDB$ROOT
 178 Space Manager: slave idle wait      oracle@localhost.localdomain (W002)            16994     ACTIVE   CDB$ROOT
 177 rdbms ipc message                   oracle@localhost.localdomain (RECO)             2834     ACTIVE   CDB$ROOT
 176 smon timer                          oracle@localhost.localdomain (SMON)             2828     ACTIVE   CDB$ROOT
 175 rdbms ipc message                   oracle@localhost.localdomain (LGWR)             2822     ACTIVE   CDB$ROOT
 174 pman timer                          oracle@localhost.localdomain (PMAN)             2815     ACTIVE   CDB$ROOT
 173 watchdog main loop                  oracle@localhost.localdomain (SCMN)             2805     ACTIVE   CDB$ROOT
 172 watchdog main loop                  oracle@localhost.localdomain (SCMN)             2799     ACTIVE   CDB$ROOT
 171 rdbms ipc message                   oracle@localhost.localdomain (MMAN)             2795     ACTIVE   CDB$ROOT
 170 rdbms ipc message                   oracle@localhost.localdomain (PSP0)             2786     ACTIVE   CDB$ROOT
  25 class slave wait                    oracle@localhost.localdomain (M001)            18580   INACTIVE   CDB$ROOT
  23 Space Manager: slave idle wait      oracle@localhost.localdomain (W007)             5009   INACTIVE   CDB$ROOT
  22 jobq slave wait                     oracle@localhost.localdomain (J001)            18696     ACTIVE   CDB$ROOT
  21 Streams AQ: waiting for time mana   oracle@localhost.localdomain (Q004)             3123     ACTIVE   CDB$ROOT
  19 jobq slave wait                     oracle@localhost.localdomain (J000)            18694     ACTIVE   CDB$ROOT
  16 rdbms ipc message                   oracle@localhost.localdomain (CJQ0)             3042     ACTIVE   CDB$ROOT
  13 Data Guard: Timer                   oracle@localhost.localdomain (TT01)             2871     ACTIVE   CDB$ROOT
  11 PGA memory operation                oracle@localhost.localdomain (M000)            18713   INACTIVE   CDB$ROOT
  10 rdbms ipc message                   oracle@localhost.localdomain (MMNL)             2848     ACTIVE   CDB$ROOT
   9 lreg timer                          oracle@localhost.localdomain (LREG)             2838     ACTIVE   CDB$ROOT
   8 rdbms ipc message                   oracle@localhost.localdomain (SMCO)             2832     ACTIVE   CDB$ROOT
   7 LGWR worker group idle              oracle@localhost.localdomain (LG00)             2826     ACTIVE   CDB$ROOT
   6 rdbms ipc message                   oracle@localhost.localdomain (DBW0)             2819     ACTIVE   CDB$ROOT
   5 wait for unread message on broadc   oracle@localhost.localdomain (SVCB)             2813     ACTIVE   CDB$ROOT
   4 OFS idle                            oracle@localhost.localdomain (OFSD)             2805     ACTIVE   CDB$ROOT
   3 rdbms ipc message                   oracle@localhost.localdomain (GEN1)             2799     ACTIVE   CDB$ROOT
   2 rdbms ipc message                   oracle@localhost.localdomain (GEN0)             2793     ACTIVE   CDB$ROOT
   1 pmon timer                          oracle@localhost.localdomain (CLMN)             2784     ACTIVE   CDB$ROOT
dsm2:>exit
==18907==
==18907== HEAP SUMMARY:
==18907==     in use at exit: 0 bytes in 0 blocks
==18907==   total heap usage: 2,657 allocs, 2,657 frees, 288,704 bytes allocated
==18907==
==18907== All heap blocks were freed -- no leaks are possible
==18907==
==18907== For counts of detected and suppressed errors, rerun with: -v
==18907== Use --track-origins=yes to see where uninitialised values come from
==18907== ERROR SUMMARY: 724 errors from 4 contexts (suppressed: 0 from 0)


-----------------------------------------------------


[oracle@localhost dsm2_code_review]$ valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./dsm2_code
==18969== Memcheck, a memory error detector
==18969== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==18969== Using Valgrind-3.12.0 and LibVEX; rerun with -h for copyright info
==18969== Command: ./dsm2_code
==18969==


dsm2:>report 5


Waiting for: 'wait for unread message on broadcast channel' P1:'channel context'=1924426384 P2:'channel handle'=(null) P3:'=0
        command= ospid=2813 sqlhv=0 psqlhv=0 Class:Idle
Waiting for: 'wait for unread message on broadcast channel' P1:'channel context'=1924426384 P2:'channel handle'=1924262352 P3:''=0
        command= ospid=2813 sqlhv=0 psqlhv=0 Class:Idle
Waiting for: 'wait for unread message on broadcast channel' P1:'channel context'=1924426384 P2:'channel handle'=1924262352 P3:''=0
        command= ospid=2813 sqlhv=0 psqlhv=0 Class:Idle
^CWaiting for: 'wait for unread message on broadcast channel' P1:'channel context'=1924426384 P2:'channel handle'=1924262352 P3:''=0
        command= ospid=2813 sqlhv=0 psqlhv=0 Class:Idle
dsm2:>exit
==18969==
==18969== HEAP SUMMARY:
==18969==     in use at exit: 0 bytes in 0 blocks
==18969==   total heap usage: 2,657 allocs, 2,657 frees, 288,704 bytes allocated
==18969==
==18969== All heap blocks were freed -- no leaks are possible
==18969==
==18969== For counts of detected and suppressed errors, rerun with: -v
==18969== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)


----------------------------------------------------------------------------


[oracle@localhost dsm2_code_review]$ valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./dsm2_code
==18991== Memcheck, a memory error detector
==18991== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==18991== Using Valgrind-3.12.0 and LibVEX; rerun with -h for copyright info
==18991== Command: ./dsm2_code
==18991==


dsm2:>listen null event


Waiting for: 'null event' P1:''=0 P2:''=(null) P3:'=0
        command= ospid=0 sqlhv=0 psqlhv=0 Class:Other
Waiting for: 'null event' P1:''=0 P2:''=0 P3:''=0
        command= ospid=0 sqlhv=0 psqlhv=0 Class:Other
Waiting for: 'null event' P1:''=0 P2:''=0 P3:''=0
        command= ospid=0 sqlhv=0 psqlhv=0 Class:Other
Waiting for: 'null event' P1:''=0 P2:''=0 P3:''=0
        command= ospid=0 sqlhv=0 psqlhv=0 Class:Other
Waiting for: 'null event' P1:''=0 P2:''=0 P3:''=0
        command= ospid=0 sqlhv=0 psqlhv=0 Class:Other
dsm2:>exit
==18991==
==18991== HEAP SUMMARY:
==18991==     in use at exit: 0 bytes in 0 blocks
==18991==   total heap usage: 2,657 allocs, 2,657 frees, 288,704 bytes allocated
==18991==
==18991== All heap blocks were freed -- no leaks are possible
==18991==
==18991== For counts of detected and suppressed errors, rerun with: -v
==18991== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

