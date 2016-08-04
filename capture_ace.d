#!/usr/sbin/dtrace -ws


BEGIN
{
	system( "./AceLock mylog myace new &" );
}


proc:::exec-success
/execname == "AceLock"/
{
	mypid = curpsinfo->pr_pid;
	printf( "%s %d \n", execname, mypid);
	stop();
	system( "./ace_lock.d %d", mypid);
	exit(0);
}
