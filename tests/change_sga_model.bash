#!/bin/bash

#
# Requires either AMM or ASMM as $1
#
export ORAENV_ASK=NO

changeToAMM(){
changeToAMM=`sqlplus -S / as sysdba << EOF
	alter system set sga_target=0 scope=spfile;
	alter system set sga_max_size=0 scope=spfile;
	alter system set memory_target=2048M scope=spfile;
	alter system set memory_max_target=2048M scope=spfile;
 exit
EOF`
}

changeToASMM(){
changeToASMM=`sqlplus -S / as sysdba << EOF
	alter system set sga_target=1512M scope=spfile;
	alter system set sga_max_size=1512M scope=spfile;
	alter system set memory_target=0 scope=spfile;
	alter system set memory_max_target=0 scope=spfile;
 exit
EOF`
}

shutdownDB(){
    shutdownDB=`sqlplus -S / as sysdba << EOF
    shutdown immediate;
 exit
EOF`
}

startupDB(){
    startupDB=`sqlplus -S / as sysdba << EOF
    startup;
 exit
EOF`
}

if [[ $1 == "AMM" ]]; then
	changeToAMM
	shutdownDB
	startupDB
	unset ORAENV_ASK
elif [[ $1 == "ASMM" ]]; then
	changeToASMM
	shutdownDB
	startupDB
	unset ORAENV_ASK
else
	printf "%s\n" "Unsupported SGA memory"
	unset ORAENV_ASK
	exit
fi	
	