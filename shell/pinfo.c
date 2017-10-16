#include "pinfo.h"
void pinfo(char *args)
{
	char *token=strtok(args,Delimiters);
	token=strtok(NULL,Delimiters);
	char str[sz]="/proc/";
	char str2[sz];
	char str3[2];
	int num=getpid();
	sprintf(str2,"%d",num);
	if(token==NULL)
		strcat(str,str2);
	else
		strcat(str,token);
	strcat(str,"/status");
	int n1=open(str,O_RDONLY);
	if(n1<=0)
	{
		printf("Please enter valid process PID\n");
		return;
	}
	while(read(n1,str3,1)>0)
		write(1,str3,1);
	close(n1);
}