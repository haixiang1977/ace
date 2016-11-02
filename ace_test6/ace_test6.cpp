/*
 * ace_test6.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: liu
 */
/* ACE Process has below method: */
/* prepare(): called by spawn() before the child process is spawned */
/* spawn(): create a new process address space and runs a specific program image */
/* unmanage(): called by Process Manager when a process it manages exits */
/* getpid(): return the process id of the new child process */
/* exit_code (): return the exit code of the child process */
/* wait(): wait for the process to exit */
/* terminate(): terminate the process abruptly, without cleanup */

#include "ace/OS.h"
#include "ace/Process.h"
#include "ace/Process_Manager.h"

int main(int argc, char *argv[])
{
	ACE_Process_Options options;

	options.command_line("ls -l");

	ACE_Process child;
	child.spawn(options);
	child.wait();

	return 0;
}
