#!/usr/sbin/dtrace -ws


BEGIN
{
	printf( "\n run %d \n", $1 );
	system( "prun %d", $1 );
}


lockstat:::
/execname == "AceLock"/
{
	@mylock[probename] = count();
}


proc:::fault
/execname == "AceLock"/
{
	stop();
	ustack();
	exit(0);
}


pid$1::*Mutex*:entry,
pid$1::*lock*:entry,
pid$1::*pthread*:entry,
pid$1::*mutex*:entry,
pid$1::*ACE_Thread_Mutex*:entry,
pid$1::*ACE_Task_Base*:entry,
pid$1::*ACE_Thread_Timer_Queue_Adapter*:entry
{
	@functions[probefunc] = count();
}


END
{
	printa( @mylock );
	printa( @functions );
	system( "prun %d", $1 );
	system( "rm -f /u01/transactive/core_files/*AceLock*" );
}

