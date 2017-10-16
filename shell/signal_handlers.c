#include "signal_handlers.h"
#include "execute.h"
#include "terminal.h"
void HandleSignal(int sig, siginfo_t *si, void *context)
{
	int status, pid,i,pos;
	switch(sig)
	{
		case SIGINT:
		break;
		case SIGCHLD:
		pid = waitpid(-1,NULL,WNOHANG);
		if(pid>0)
		{
			kill(pid,SIGTERM);
			int i;
			for(i=0;i<count;i++)
			{
				if(pid_array[i].num==pid)
				{
					printf("%s with process id %d  exited successfully\n",pid_array[i].str,pid_array[i].num);
					pid_array[i].num=-1;
				}
			}
		}
		break;
	}
}
void ctrlC(int signo)
{
	if(signo==SIGINT)
	{
		if(getpid()!=shellpid)
			kill(getpid(),9);
		else
			printf("\n");
	}
}

void ctrlZ(int signo)
{
	if(signo==SIGTSTP)
	{
		if(getpid()!=shellpid)
			kill(getpid(),SIGTSTP);
		else
			printf("\n");
	}
}
