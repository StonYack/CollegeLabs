// server
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/wait.h>

#define PORT 42000
void error(const char *msg)
{
     perror(msg);
      exit(1);
}

int main(int argc, char** argv)
{
	FILE * file;
	int PID = 0;
	int childReturnStatus;
	char * args[100];
	char cmd[100];
	char *temp;
 	char * input = "";
	int ret = 0;
	int opt;
 	int size = 0;
 	int P = -1;
 	int in = 0;
 	while ((opt = getopt(argc, argv, "p:")) != -1)
 	{
 	    switch (opt)
 	    {
 	        case 'p':
 			 P = atol(optarg);
 	              	 printf("Port: %d \n", P);
 	             	 break;
 	
 	    }
	}
 	    if (P == -1)
 	    {
 	        ret = 1;
 	    }
 	    else
 	    {
 	        printf("PORT: %d\n", P);
 	        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
 	        printf("sockfd: %d\n", sockfd);
 	        if (sockfd < 0)
 	            error("ERROR opening socket");
 	        
 	        printf("Finished Sock init\n");
 	
 	        struct sockaddr_in serv_addr;
 	        bzero((char *) &serv_addr,sizeof(serv_addr));
 	        serv_addr.sin_family = AF_INET;
 	        serv_addr.sin_addr.s_addr = INADDR_ANY;
 	        serv_addr.sin_port = htons(PORT);
 	        
 	        printf("Finished Sock setup\n");
 	
 	        if (bind(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
 	            error("ERROR on binding");
 	
 	        printf("Finished bind\n");
 	
 	        listen(sockfd,5);
 	
 	        printf("Finished listen\n");
 	
 	        struct sockaddr_in cli_addr;
 	        socklen_t clilen = sizeof(cli_addr);
 	        int clientfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
 	        if (clientfd < 0)
 	            error("ERROR on accept");
 	        char buffer[256];
 	        bzero(buffer,256);
 	        // Read data from the buffer (Receive)
 	        ssize_t n = read(clientfd, buffer, 255);
 	        if (n < 0) error("ERROR reading from socket");
 	        // Do something with the data
 	        printf("Here is the message: %s\n",buffer);
 	        // Respond in some way (Send)
 	        n = write(clientfd,"I got your message",18);
 	        if (n < 0) error("ERROR writing to socket");
 	        printf("closing\n");
		bzero(buffer, 256);
 	        while(1) 
 	        {
 	            read(clientfd, &in, sizeof(in));
 	            if (in == 1)
 	            {
 	                printf("Get\n");
			read(clientfd, buffer, 255);
			printf("Name: %s\n", buffer);
			file = fopen(buffer, "r");
			if(file == NULL)
			{
				printf("No file\n");
			}
			fseek(file, 0, SEEK_END);
			size = ftell(file);
			rewind(file);
			printf("file rewound\n");
			read(clientfd, &buffer, 255);
			write(clientfd, &size, sizeof(size));
			printf("Size: %d\n", size);
			//bzero(buffer, 256);
			while(size >= 255)
			{
				n = fread(buffer, 1, sizeof(buffer), file);
				write(clientfd, buffer, n);
				size = size - n;
			}
			if(size > 0)
			{
				fread(buffer, 1, size, file);
				write(clientfd, buffer, strlen(buffer));
			}
			printf("leaving get\n");
			fclose(file);
 	            }
 	            if(in == 2)
 	            {
 	                printf("Put\n");
			read(clientfd, buffer, 255);
			printf("Name: %s\n", buffer);
			write(clientfd, "good", 4);
			read(clientfd, &size, sizeof(size));
			file  = fopen(buffer, "w");
			printf("Size: %d\n", size);
 	                while(size >= 255)
			{
 	                    n = read(clientfd, buffer, 255);
 	                    fwrite(buffer, 1, 255, file);
 	                    size = size - n;
 	                }
 	                if(size > 0)
 	                {
 	                    read(clientfd, buffer, size);
 	                    fwrite(buffer, 1,size, file);
 	                }
			fclose(file);
 	            }
		    if(in == 3)
		    {
			 DIR *d;
	                 struct dirent *dir;
         	         d = opendir(".");
               		 if(d)
               		 {
                   	 	 while((dir = readdir(d)) != NULL)
                   		 {
                       			 printf("%s\n", dir->d_name);
                   		 }
                   	 closedir(d);
               	   	 }

 	            }
		    if(in == 4)
		    {
			PID = fork();
			if(PID == -1)
			{
				printf("There is a FORKING ERROR\n");
			}
			if(PID == 0)
			{
		 		printf("New Command\n");
				int n = read(clientfd, buffer, 255);
				printf("Command: %s %d\n", buffer, n);
				strcpy(cmd, "/usr/bin/");
		                temp = strtok(buffer," ");
        		        strcat(cmd, temp);
             			puts(cmd);
               			args[0] = cmd;

		                for(int i = 2; args[i] != NULL; i++)
        		        {
   		                     args[i-1] = strtok(NULL, "");
    		                     args[i] = NULL;
 		                     puts(args[i-1]);
              			}

				printf("Args: %s %d\n", cmd, n);
				close(1);
				fopen(".temp.txt", "w");
				execv(cmd, args);
			}
			else
			{
				wait(&childReturnStatus);
			 	strcpy(buffer, ".temp.txt");
		                write(clientfd, " ", 255);
				write(clientfd, buffer, strlen(buffer));
	 	                printf("buffer: %s\n", buffer);
		                file = fopen(buffer, "r");
	       		        fseek(file, 0, SEEK_END);
      			     	size = ftell(file);
 	           		rewind(file);
         	  	        printf("file rewound\n");
		              	write(clientfd, &size , sizeof(size));
		                printf("Size: %d\n", size);
			}
			printf("Command Executed\n");

               		 while(size >= 255)
               		 {
               		   	n = fread(buffer, 1, sizeof(buffer), file);
                		write(sockfd, &file, n);
             	  	        size = size - n;
             		 }
              		 if(size > 0)
               		 {
                  		fread(buffer, 1, size, file);
                   		write(sockfd, &file, size);
                	 }
               		 printf("temp sent\n");
               		 fclose(file);
			 remove(file);
		     }
		  in = 0;
 	        }
		// Close the connection
 	        close(clientfd);
 	        close(sockfd);
		
	}

 	    return ret;
}
