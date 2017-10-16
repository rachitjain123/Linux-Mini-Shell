#include "jobs.h"
#include "execute.h"
void jobs()
{
	int i,val=0;
	for(i=0;i<count;i++)
	{
		if(pid_array[i].num==-1)
			continue;
		else
		{
			val++;
			if(strcmp(pid_array[i].status,"Stopped")==0)
				printf("%d : %s with process id %d stopped\n",val,pid_array[i].str,pid_array[i].val);
			else
				printf("%d : %s with process id %d running\n",val,pid_array[i].str,pid_array[i].num);
		}
	}
}
