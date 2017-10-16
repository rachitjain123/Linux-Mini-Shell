#include "overkill.h"
#include "execute.h"
void overkill()
{
	int i;
	for(i=0;i<count;i++)
	{
		if(pid_array[i].num!=-1)
		{
			kill(pid_array[i].num,SIGKILL);
			pid_array[i].num=-1;
		}
	}
}
