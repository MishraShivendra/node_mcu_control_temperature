/* Notice: All parts of this code is protected
   with following license:
   https://github.com/MishraShivendra/node_mcu_py_email/blob/master/LICENSE.md
   Corporate/commercial usage is restricted.
 
   Author : Shivendra Mishra
   Env: gcc 5.4 on Linux Ubuntu 16.04 
*/

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>

void
signal_handler( int signal )
{
	switch( signal ) 
	{
		case SIGQUIT:
		case SIGKILL:
			syslog( LOG_DEBUG,"Received SIGKILL/QUIT/KILL, will ignore this one.");
			break;
		case SIGHUP:
			syslog( LOG_DEBUG,"Received SIGHUP, will reload the configuration files." );
			/*Reload*/
			break;
		case SIGUSR1:
			syslog( LOG_DEBUG,"Received signal USR1, will post various readings." );
			/* Get and post readings */
			break;
		case SIGUSR2:
			syslog( LOG_DEBUG,"Received signal USR2, will post a few debug info." );
			break;
		default:
			syslog( LOG_DEBUG,"Received unknown signal, no idea what to do." );
			break;
	}
}


int
main( void ) 
{
	pid_t child_pid = 0;
	int ret = 0;
	int i = 0;
	struct rlimit *rlim;

	umask(0);
	/* Create a child */
	child_pid = fork();
	if( child_pid < 0 ) {
		fprintf( stderr, "Error forking child !\n" );
	}else if( child_pid != 0 ) {
		exit(EXIT_SUCCESS);
	}
	/* As the man says, caller will be the new lead if isn't already */
	if( setsid() < 0 ) {
		fprintf( stderr, "Failed to setsid()\n" );
	}
	/* Move to /, as we don't want to interfere in someone's umount  */
	if( chdir( "\\" ) < 0 ) {
		fprintf( stderr, "Failed to change directory to \\ \n" );
	}
	
	/* Try to find out number of FD associated  */
	if( getrlimit( RLIMIT_NOFILE, rlim ) < 0 ) {
		fprintf( stderr, "Couldn't get rlimit.\n" );
	}
	
	/* Stop talking with people, closing doors namely
	 * 0(in), 1(out), 2(err) to whatever */
	for( i = 0; i<rlim->rlim_max; i++ ) {
		close( i );
	}
	
	/* Tie 0, 1, 2 to /dev/null */
	ret = open( "/dev/null", O_RDWR );
	(void)dup( ret );
	(void)dup( ret );

	
	/* open log, after all we need to report to someone */
	openlog( NULL, LOG_CONS, LOG_DAEMON );
	
	/* Still we want to handle a few signal of humans */
	if( signal( SIGKILL, signal_handler ) < 0 ) {
		syslog( LOG_DEBUG,"Failed set signal handler for SIGKILL." );
	}
	if( signal( SIGQUIT, signal_handler ) < 0 ) {
		syslog( LOG_DEBUG,"Failed set signal handler for SIGQUIT." );
	}
	if( signal( SIGHUP, signal_handler ) < 0 ) {
		syslog( LOG_DEBUG,"Failed set signal handler for SIGHUP." );
	}
	if( signal( SIGUSR1, signal_handler ) < 0 ) {
		syslog( LOG_DEBUG,"Failed set signal handler for SIGUSR1." );
	}
	if( signal( SIGUSR2, signal_handler ) < 0 ) {
		syslog( LOG_DEBUG,"Failed set signal handler for SIGUSR2." );
	}


	/*Now after all, we are daemon ;) */
	
	while( 1 ) {
		/* push */
		//do_curl_and_get( server );
		
		/* pull */
		//push_data_to_db( db_address );
		/* Sleep for a while */
		sleep(1);
	}

}
