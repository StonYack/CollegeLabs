#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char *argv[])
{
	int PID = 0;
	int childReturnStatus;
	char * args[100];
	char cmd[100];
	FILE *file;
	
	PID = fork();
	if(PID == -1)
	{
		printf("There is a forking error\n");
	}
	else
	{	
		if(PID == 0)
		{
			strcpy(cmd, "/usr/bin/");
			strcat(cmd, argv[1]);
			puts(cmd);
			args[0] = cmd;
			for(int i = 2; argv[i] != NULL; i++)
			{
				args[i-1] = argv[i];
				args[i] = NULL;
				puts(args[i-1]);
			}
			close(1);
			fopen(".temp.txt", "w");
			execv(cmd, args);
		}
		else
		{
			wait(&childReturnStatus);
			//read(clientfd, buffer, 255
		}
	}
	printf("Done\n");
	return 0;
}
