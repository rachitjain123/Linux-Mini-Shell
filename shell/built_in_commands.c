#include "built_in_commands.h"
void echo(char *args)
{
	int fl=0,i,len=strlen(args);
	for(i=0;i<len;i++)
	{
		if(fl==0 && args[i]==' ')
			fl=1;
		else if(fl==1 && args[i]!=' ')
		{
			printf("%c",args[i]);
			fl=2;
		}
		else if(fl==2)
			printf("%c",args[i]);
	}
}
void cd(char *args,char *home)
{
	char *token=strtok(args,Delimiters);
	token=strtok(NULL,Delimiters);
	if(token==NULL||token[0]=='~')
		chdir(home);
	else
	{
		int fl=chdir(token);
		if(fl<0)
			fprintf(stderr,"Directory does not exist :(\n");
	}
}
void pwd()
{
	char *current_directory=malloc(sz*sizeof(char));
	getcwd(current_directory,sz);
	printf("%s\n",current_directory);
}
