#include "ls.h"
void ls(char *args)
{
	int fl,cnt=0,i;
	char *token=strtok(args,Delimiters);
	token=strtok(NULL,Delimiters);
	char str[sz][sz];
	if(token==NULL)
		fl=1;
	else
	{
		if(strcmp(token,"-a")==0)
			fl=2;
		else if(strcmp(token,"-l")==0)
			fl=3;
		else if(strcmp(token,"-al")==0||strcmp(token,"-la")==0)
			fl=4;
		else
			fl=1;
	}
	if(fl==1||fl==2)
	{
		if(fl==2)
			token=strtok(NULL,Delimiters);
		while(token!=NULL)
		{
			if(token!=NULL)
				strcpy(str[cnt],token);
			token=strtok(NULL,Delimiters);
			cnt++;
		}
		if(cnt==0)
		{
			strcpy(str[0],".");
			cnt++;
		}
		for(i=0;i<cnt;i++)
		{
			DIR *present_directory;
			struct dirent *file;
			printf("For %s directory :\n\n",str[i]);
			if((present_directory=opendir(str[i]))==NULL)
			{
				fprintf(stderr,"Cannot open %s diectory",str[i]);
				return;
			}
			while(1)
			{
				if((file=readdir(present_directory))==NULL)
					break;
				char str[sz];
				strcpy(str,file->d_name);
				if(str[0]!='.')
					printf("%s\n",str);
				else
				{
					if(fl==2)
						printf("%s\n",str);
				}
			}
			closedir(present_directory);
		}
	}
	else
	{
		token=strtok(NULL,Delimiters);
		while(token!=NULL)
		{
			if(token!=NULL)
				strcpy(str[cnt],token);
			token=strtok(NULL,Delimiters);
			cnt++;
		}
		if(cnt==0)
		{
			strcpy(str[0],".");
			cnt++;
		}
		for(i=0;i<cnt;i++)
		{
			int flag;
			struct dirent* current_directory;
			struct stat stats;
			struct tm lt;  
			struct group *user_group=getgrgid((long)stats.st_gid);
			struct passwd *pwd;
			DIR* directory=opendir(str[i]);
			printf("Directory -> %s\n\n", str[i]);
			if(directory==NULL)     
			{ 
				printf("dir_list : %s : %s \n", str[i], strerror(errno));
				return ;
			}   
			while((current_directory=readdir(directory)))
			{ 
				char strin[sz];
				strcpy(strin,current_directory->d_name);
				if(strin[0]=='.')
				{
					if(fl==4)
					{
						stat(current_directory->d_name,&stats);  
						if((stat(current_directory->d_name,&stats))==0)
							pwd=getpwuid(stats.st_uid);
						if((stats.st_mode & S_IFMT)==S_IFLNK)
						{
							printf("l");
							flag=1;
						}
						else if((stats.st_mode & S_IFMT)==S_IFDIR)
							printf("d");
						else 
							printf("-");
						long int no_of_links=(long)stats.st_nlink;
						mode_t val;
						val=(stats.st_mode & ~S_IFMT);
						if(val & S_IRUSR) 
							printf("r"); 
						else 
							printf("-");
						if (val & S_IWUSR) 
							printf("w");
						else 
							printf("-");    
						if(val & S_IXUSR)  
							printf("x") ;
						else 
							printf("-");
						if(val & S_IRGRP)  
							printf("r") ;
						else 
							printf("-");
						if(val & S_IWGRP)  
							printf("w") ;
						else 
							printf("-");
						if(val & S_IXGRP)  
							printf("x") ;
						else 
							printf("-");
						if(val & S_IROTH)  
							printf("r") ;
						else 
							printf("-");
						if(val & S_IWOTH)  
							printf("w") ;
						else 
							printf("-");
						if(val & S_IXOTH)  
							printf("x") ;
						else 
							printf("-");
						printf(" %ld ",no_of_links);
						printf(" %4s ",pwd->pw_name);
						printf("%4s ",user_group->gr_name);
						time_t t=stats.st_mtime;
						localtime_r(&t,&lt);
						char timebuf[90];
						strftime(timebuf,sizeof(timebuf),"%c",&lt);
						if(pwd!=0) 
							printf("%ld \t%s %s\n", (long)stats.st_size,timebuf,current_directory->d_name);
						if(pwd==0)
							printf("%d %ld \t%s %s\n", stats.st_uid,(long)stats.st_size,timebuf,current_directory->d_name);
					}
				}
				else
				{
					stat(current_directory->d_name,&stats);  
					if((stat(current_directory->d_name,&stats))==0)
						pwd=getpwuid(stats.st_uid);
					if((stats.st_mode & S_IFMT)==S_IFLNK)
					{
						printf("l");
						flag=1;
					}
					else if((stats.st_mode & S_IFMT)==S_IFDIR)
						printf("d");
					else 
						printf("-");
					long int no_of_links=(long)stats.st_nlink;
					mode_t val;
					val=(stats.st_mode & ~S_IFMT);
					if(val & S_IRUSR) 
						printf("r"); 
					else 
						printf("-");
					if (val & S_IWUSR) 
						printf("w");
					else 
						printf("-");    
					if(val & S_IXUSR)  
						printf("x") ;
					else 
						printf("-");
					if(val & S_IRGRP)  
						printf("r") ;
					else 
						printf("-");
					if(val & S_IWGRP)  
						printf("w") ;
					else 
						printf("-");
					if(val & S_IXGRP)  
						printf("x") ;
					else 
						printf("-");
					if(val & S_IROTH)  
						printf("r") ;
					else 
						printf("-");
					if(val & S_IWOTH)  
						printf("w") ;
					else 
						printf("-");
					if(val & S_IXOTH)  
						printf("x") ;
					else 
						printf("-");
					printf(" %ld ",no_of_links);
					printf(" %4s ",pwd->pw_name);
					printf("%4s ",user_group->gr_name);
					time_t t=stats.st_mtime;
					localtime_r(&t,&lt);
					char timebuf[90];
					strftime(timebuf,sizeof(timebuf),"%c",&lt);
					if(pwd!=0) 
						printf("%ld \t%s %s\n", (long)stats.st_size,timebuf,current_directory->d_name);
					if(pwd==0)
						printf("%d %ld \t%s %s\n", stats.st_uid,(long)stats.st_size,timebuf,current_directory->d_name); 
				}
			}
			closedir(directory);
		}
	}
}
