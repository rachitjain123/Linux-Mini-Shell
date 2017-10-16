#include "environment.h"
void set(char *args)
{
	char *token=strtok(args,Delimiters);
	char *tkn1=strtok(NULL,Delimiters);
	char *tkn2=strtok(NULL,Delimiters);
	char *tkn3=strtok(NULL,Delimiters);
	if(tkn1==NULL && tkn2==NULL)
	{
		printf("Wrong Usage\n");
		return;
	}
	if(tkn3!=NULL)
	{
		printf("Wrong Usage\n");
		return;
	}
	if(getenv(tkn1)!=NULL)
		printf("Variable Overwritten\n");
	else
		printf("Variable Created\n");

	if(tkn2==NULL)
		setenv(tkn1,"",1);
	else
		setenv(tkn1,tkn2,1);
}
void unset(char *args)
{
	char *token=strtok(args,Delimiters);
	char *tkn1=strtok(NULL,Delimiters);
	if(tkn1==NULL)
	{
		printf("Wrong Usage\n");
		return;
	}
	if(getenv(tkn1)==NULL)
	{
		printf("Variable does not exists\n");
		return;
	}
	unsetenv(tkn1);
	printf("Variable Destroyed\n");
}