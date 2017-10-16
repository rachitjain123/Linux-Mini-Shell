#include "execute.h"
#include "terminal.h"
#include "colors.h"
#include "signal_handlers.h"
#include <termios.h>
#include <glob.h>
#include <pwd.h>
#include <sys/stat.h>
void SIGINT_handler()
{
	signal(SIGINT,SIG_IGN);
	printf("Handled\n");
}
void SIGCHLD_handler()
{
	printf("Process exited!\n");
}
int main()
{
	shellpid=getpid();
	int i,j,k;
	struct passwd *p=getpwuid(getuid());
	struct utsname node;
	uname(&node);
	char *home=malloc(sz*sizeof(char));
	getcwd(home,sz);
	char line[2*sz];
	char **commands=malloc(sz*sizeof(char*));
	int wait_ret;
	int status;

	struct sigaction sVal;
	sVal.sa_flags = SA_RESTART;
	sVal.sa_sigaction = &HandleSignal;
	sigfillset(&sVal.sa_mask);
	if(sigaction(SIGINT, &sVal, NULL)<0)
		perror("Error");
	if(sigaction(SIGCHLD, &sVal, NULL)<0)
		perror("Error");
	signal(SIGINT, ctrlC);
	signal(SIGTSTP, ctrlZ);

	while(1)
	{
		for(i=0;i<count;i++)
		{
			if(pid_array[i].num==-1)
				continue;
			wait_ret=waitpid(pid_array[i].num,&status,WNOHANG);
			if(wait_ret!=0 && wait_ret!= -1)
			{
				if(WIFEXITED(status)||WIFSIGNALED(status))
				{
					printf("%s with process id %d  exited successfully\n",pid_array[i].str,pid_array[i].num);
					pid_array[i].num=-1;
				}
			}
		}
		int num_commands=0;
		fprintf(stdout, RED "%s@" RESET, p->pw_name);
		fprintf(stdout, YEL "%s" RESET, node.nodename);
		printf(" ");
		char *current_dir=malloc(sz*sizeof(char));
		getcwd(current_dir,sz);
		int len1=strlen(current_dir);
		int len2=strlen(home);
		for(i=0;i<len2;i++)
		{
			if(i==(len1)||current_dir[i]!=home[i])
				break;
		}
		if(i!=(len2))
			printf("%s",current_dir);
		else
		{
			printf("~");
			for(i=len2;i<=len1;i++)
				printf("%c",current_dir[i]);
		}
		printf(">");
		fgets(line,sz,stdin);
		if(strlen(line)==0)
			perror("Error");
		char *token;
		token=strtok(line,";");
		while(token!=NULL)
		{
			commands[num_commands++]=token;
			token=strtok(NULL,";");
		}
		for(i=0;i<num_commands;i++)
			execute(commands[i],home);
	}
}