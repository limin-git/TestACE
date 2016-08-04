#!/usr/sbin/dtrace -ws


BEGIN
{
	system( "./AceLock mylog myace new &" );
	ace_lock_exited = 1;
}


proc:::exec-success
/execname == "AceLock"/
{
	stop();
	acelock_pid = curpsinfo->pr_pid;
	printf( "%s %d \n", execname, acelock_pid);
	system( "./ace_lock.d %d", acelock_pid );
	ace_lock_exited = 0;
}


proc:::exit
/execname == "AceLock"/
{
	ace_lock_exited = 1;
}


proc:::exit
/execname == "ace_lock.d" && ace_lock_exited == 0/
{
	trace( "kill AceLock\n" );
	system( "prun %d", acelock_pid );
}


proc:::exit
/execname == "ace_lock.d"/
{
	exit( 0 );
}
