#!/usr/sbin/dtrace -ws


BEGIN
{
	system( "./AceLock mylog myace new &" );
}


proc:::exec-success
/execname == "AceLock"/
{
	stop();
	acelock_pid = curpsinfo->pr_pid;
	printf( "%s %d \n", execname, acelock_pid);
	system( "./ace_lock.d %d", acelock_pid );
	exit(0);
}


END
{
	system( "prun %d", acelock_pid );
}
