#include "fg_bg.h"
#include "execute.h"
int ToInt(char *str)
{
	int mult=1;
	int re=0;
	int len=strlen(str);
	for(int i=len-1;i>=0;i--)
	{
		re=re+((int)str[i]-48)*mult;
		mult=mult*10;
	}
	return re;
}
void fg(char *args)
{
	int status=0,flag=0;
	char *token=strtok(args,Delimiters);
	token=strtok(NULL,Delimiters);
	if(token==NULL)
	{
		printf("Wrong Usage\n");
		return;
	}
	int num2=ToInt(token);
	num2--;
	if(pid_array[num2].num!=0 && pid_array[num2].num!=-1)
	{
		int i=num2;
		flag=1;
		int a=pid_array[i].num;
		kill(pid_array[i].num,SIGCONT);
		pid_array[i].num=-1;
		waitpid(a,&status,WUNTRACED);
		if(WSTOPSIG(status))
		{
			status=-1;
			pid_array[i].num=a;
			strcpy(pid_array[i].status,"Stopped");
		}
	}
	if(flag!=1)
		printf("Process number %d not found\n",num2);

}
void bg(char *args)
{
	int status=0,flag=0;
	char *token=strtok(args,Delimiters);
	token=strtok(NULL,Delimiters);
	if(token==NULL)
	{
		printf("Wrong Usage\n");
		return;
	}
	int num2=ToInt(token);
	num2--;
	if(pid_array[num2].num!=0 && pid_array[num2].num!=-1)
	{
		int i=num2;
		flag=1;
		int a=pid_array[i].num;
		kill(pid_array[i].num,SIGCONT);
		strcpy(pid_array[i].status,"Running");

	}
	if(flag!=1)
		printf("Process number %d not found\n",num2);
}