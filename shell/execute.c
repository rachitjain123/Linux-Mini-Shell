#include "built_in_commands.h"
#include "ls.h"
#include "pinfo.h"
#include "execute.h"
#include "jobs.h"
#include "overkill.h"
#include "fg_bg.h"
#include "signal_handlers.h"
#include "environment.h"
char buffer[4096];
void kjob(char *args)
{
	char *token;
	token=strtok(args,Delimiters);
	token=strtok(NULL,Delimiters);
	if(token==NULL)
	{
		printf("Wrong Usage\n");
		return;
	}
	int a=ToInt(token);
	token=strtok(NULL,Delimiters);
	if(token==NULL)
	{
		printf("Wrong Usage\n");
		return;
	}
	int b=ToInt(token);
	a--;
	if(pid_array[a].num!=-1 && pid_array[a].num!=0 && strcmp(pid_array[a].status,"Stopped")!=0)
		kill(pid_array[a].num,b);
	else
		printf("Process number not found\n");
}
void another_process(char **query_array,int flag)
{
	int pid=fork();
	if(pid==0)
	{
		signal(SIGTSTP,&ctrlZ); 
		signal(SIGINT,&ctrlC); 
		query_array=redirect(query_array);
		if(flag==1)
			setpgid(0,0);
		if(execvp(query_array[0],query_array))
			fprintf(stderr, "ERROR: \"%s\" command unrecognised\n", query_array[0]);
	}
	else if(!flag)
	{
		int status=0;
		waitpid(pid, &status,WUNTRACED);
		if(WSTOPSIG(status))
		{
			status=-1;
			pid_array[count].num=pid;
			pid_array[count].val=pid;
			strcpy(pid_array[count].status,"Stopped");
			strcpy(pid_array[count].str,query_array[0]);
			count=count+1;
		}
	}
	else
	{
		printf("%s with process id : %d started \n",query_array[0],pid);
		usleep(80);
		pid_array[count].num=pid;
		pid_array[count].val=pid;
		strcpy(pid_array[count].status,"Running");
		strcpy(pid_array[count].str,query_array[0]);
		count++;
	}
}
char **redirect(char **args)
{
	int fd1,fd2,fd3;
	char in[sz],out[sz],outdir[sz];
	char *query_array=args[0];
	int c=0;
	int flag1=0,flag2=0,flag3=0;
	while(query_array!=NULL)
	{
		if(strncmp(query_array,"<",1)==0)
		{
			args[c]=NULL;
			flag1=1;
			strcpy(in,args[c+1]);
		}
		if((strncmp(query_array,">",1)==0))
		{
			args[c]=NULL;
			flag2=1;
			strcpy(out,args[c+1]);
		}
		if(strncmp(query_array,">>",2)==0)
		{
			args[c]=NULL;
			flag3=1;
			strcpy(outdir,args[c+1]);
		}
		c++;
		query_array=args[c];
	}
	if(flag1==0 && flag2==0 && flag3==0)
		return args;
	if(flag1==1)
	{
		fd1=open(in,O_RDONLY,0);
		if(fd1<0)
		{
			perror("Error in opening file");
			exit(0);
		}
		dup2(fd1,0);
		close(fd1);
	}
	if(flag2==1)
	{
		fd2=open(out,O_WRONLY|O_CREAT,0666);
		if(fd2<0)
		{
			perror("Error in opening the output file");
			exit(0);
		}
		dup2(fd2,1);
		close(fd2);
	}
	if(flag3==1)
	{
		fd3=open(outdir,O_WRONLY|O_CREAT|O_APPEND,0666);
		if(fd3<0)
		{
			perror("Error in opening the output file");
			exit(0);
		}
		dup2(fd3,1);
		close(fd3);
	}
	return args;
}
void pipeHandler(char **args)
{	
	int status;
	int p[2],q[2];
	int i=0,j=0,k=0,end=0,m=0,count=0;
	int fd1,fd2,fd3;
	char *output[sz];
	char *r=args[0];
	pid_t pid;
	while(r!=NULL)
	{
		if(strncmp(args[m],"|",1)==0)
			count++;
		m++;
		r=args[m];
	}
	count++;
	int last=count-1;
	while(args[j]!=NULL && end!=1)
	{
		k=0;
		char *d=args[j];
		while((strncmp(d,"|",1)!=0))
		{
			output[k]=d;
			j++;
			d=args[j];
			if(d==NULL)
			{
				end=1;
				k++;
				break;
			}
			k++;
		}
		output[k]=NULL;
		j++;
		if(i%2==0)
			pipe(q);
		else
			pipe(p);
		pid=fork();
		if(pid<0)
			perror("Unable to fork the process");
		if(pid==0)
		{
			char **sym1;
			sym1=redirect(output);
			if(i==0)
				dup2(q[1],1);
			else if(i==last)
			{
				if(count%2==0)
					dup2(q[0],0);
				else
					dup2(p[0],0);
			}
			else
			{
				if(i%2==0)
				{
					dup2(p[0],0);
					dup2(q[1],1);
				}
				else
				{
					dup2(q[0],0);
					dup2(p[1],1);
				}
			}
			if(execvp(sym1[0],sym1)==-1)
				kill(getpid(),SIGTERM);
		}
		if(i==0)
			close(q[1]);
		else if(i==last)
		{
			if(i%2==0)
				close(q[0]);
			else
				close(p[0]);
		}
		else
		{
			if(i%2==0)
			{
				close(p[0]);
				close(q[1]);
			}
			else
			{
				close(q[0]);
				close(p[1]);
			}
		}
		waitpid(pid,NULL,0);
		i++;
	}
}
void execute(char *args,char *home)
{
//
	char *tokens[sz];
	char temp3[sz];
	strcpy(temp3,args);
	if((tokens[0]=strtok(temp3,Delimiters))==NULL)
		return;
	int numTokens=1;
	while((tokens[numTokens]=strtok(NULL,Delimiters))!=NULL)
		numTokens++;
	int j=0,is_redir_piping=0;
	while(tokens[j]!=NULL)
	{
		if((strcmp(tokens[j],">")==0)||(strcmp(tokens[j],"<")==0)||(strcmp(tokens[j],"|")==0)||(strcmp(tokens[j],">>")==0))
			is_redir_piping=1;
		j++;
	}
	char temp[sz],built_in[sz];
	strcpy(temp,args);
	strcpy(built_in,args);

	int flag=0,i=0;

	char *token;
	token=strtok(temp,Delimiters);

	if(token==NULL)
		return;

	char **query_array=(char **)malloc(100 * sizeof(char *));

	while(token!=NULL)
	{
		if(strcmp(token,"&"))
		{
			query_array[i]=(char *)malloc(strlen(token)+1);
			strcpy(query_array[i], token);
			i++;
		}
		else
			flag=1;
		token=strtok(NULL,Delimiters);	
	}
	query_array[i]=NULL;

	char *token2=strtok(args,Delimiters);
	if(strcmp(token2,"cd")==0)
		cd(built_in,home);
	else if(strcmp(token2,"pwd")==0 && is_redir_piping!=1)
		pwd();
	else if(strcmp(token2,"echo")==0 && is_redir_piping!=1)
		echo(built_in);
	else if(strcmp(token2,"quit")==0)
		exit(0);
	else if(strcmp(token2,"kjob")==0)
		kjob(built_in);
	else if(strcmp(token2,"ls")==0 && is_redir_piping!=1)
		ls(built_in);
	else if(strcmp(token2,"pinfo")==0)
		pinfo(built_in);
	else if(strcmp(token2,"jobs")==0)
		jobs();
	else if(strcmp(token2,"overkill")==0)
		overkill();
	else if(strcmp(token2,"fg")==0)
		fg(built_in);
	else if(strcmp(token2,"bg")==0)
		bg(built_in);
	else if(strcmp(token2,"setenv")==0)
		set(built_in);
	else if(strcmp(token2,"unsetenv")==0)
		unset(built_in);
	else
	{
		i=0;
		int n11=0;
		while(tokens[i]!=NULL && tokens[i]!="&")
		{
			if(strncmp(tokens[i],"|",1)==0)
			{
				n11=1;
				pipeHandler(query_array);
				break;
			}
			i++;
		}
		if(n11==0)
			another_process(query_array,flag);
	}
	i=0;
	while(query_array[i]!=NULL)
	{
		free(query_array[i]);
		i++;
	}
}
