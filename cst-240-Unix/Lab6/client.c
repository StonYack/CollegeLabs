// client
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#define PORT 42000

		
void error(const char *msg)
{
     perror(msg);
      exit(1);
}
int main(int argc, char** argv)
{
    char * args[100];
    char cmd[100];
    
    char *temp;
    int size = 0;
    int opt;
    int P = -1;
    char * H = "";
    char * M = "";
    char * input = "";
    const int get = 1;
    const int put = 2;
    const int ls  = 3;
    const int def = 4;
    int value = 0;
    int quit = 0;
    char buffer[256];
    char * name = "";
    char * tok = "";
    FILE * file;
    while ((opt = getopt(argc, argv, "h:p:")) != -1)
    {	
	switch(opt)
	{
		case 'h':
			H = optarg;
			printf("Host: %s \n", H); 
			break;
		case 'p':
			P = atol(optarg);
			printf("Port: %d \n", P);
			break;	
	}
    }
    if (P == -1 || H == NULL)
    {
        value = 1;
    }
    else
    {
        printf("Host: %s PORT: %d %s\n", H, P, M);
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            error("ERROR opening socket");
        struct hostent *server = gethostbyname("localhost");
        if (server == NULL) {
            error("ERROR, no such host\n");
        }
        struct sockaddr_in serv_addr;
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr_list[0],
                (char *)&serv_addr.sin_addr.s_addr,
                server->h_length);
        serv_addr.sin_port = htons(PORT);

        printf("setup complete\n");
        
        if (connect(sockfd,
                (struct sockaddr *) &serv_addr,
                sizeof(serv_addr)) < 0)
                    error("ERROR connecting");
        printf("connect complete\n");
        // Prepare message
        bzero(buffer,256);
        printf("buffer NULL \n");
        strcpy(buffer,"My brain hurts!");
        printf("Message ready\n");
        // Write message to server
        ssize_t n = write(sockfd,buffer,strlen(buffer));
        //printf("Message writing %u\n", n);
        if (n < 0)
            error("ERROR writing to socket");
        // Clear the buffer
        bzero(buffer,256);
        // Wait for server to respond
        n = read(sockfd,buffer,255);
        if (n < 0)
            error("ERROR reading from socket");
        // Print the response
        printf("%s\n",buffer);
	bzero(buffer, 256);
        while(quit != 1)
        {
            input = readline("Enter command: ");
	    tok = strtok(input, " ");
	    printf("%s is the input\n", input);
            printf("token: %s\n", tok);
	    strcpy(cmd, input);
            if(!strcmp(input, "get"))
            {	
		write(sockfd, &get, sizeof(put));
                name = strtok(NULL, "\n");
		strcpy(buffer, name);
                printf("buffer: %s\n", buffer);
		write(sockfd, &buffer, 255);
		write(sockfd, "Dummy Message", 13);
		read(sockfd, &size, sizeof(size));	
                printf("running get\n");
                file = fopen(buffer, "w");
                printf("Size: %d\n", size);
		while(size >= 255)
		{
			n = read(sockfd, buffer, 255);
			fwrite(buffer, 1, 255, file);
			size = size - n;
		}
		if(size > 0)
		{
			read(sockfd, buffer, size);
			fwrite(buffer, 1, size, file);
		}
                printf("leaving get\n");
                fclose(file);
            }
            else if(!strcmp(input, "put"))
            {
                write(sockfd, &put, sizeof(put));
                printf("Sending: %d\n", put);
                name = strtok(NULL, "\n");
                strcpy(buffer, name);
                write(sockfd, buffer, strlen(buffer));
                printf("buffer: %s\n", buffer);
                file = fopen(buffer, "r");
                fseek(file, 0, SEEK_END);
                size = ftell(file);
                rewind(file);
		printf("file rewound\n");
                read(sockfd, buffer, strlen(buffer));
                write(sockfd, &size , sizeof(size));
                printf("Size: %d\n", size);
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
		printf("File put\n");
                fclose(file);
            }
           // else if(!strcmp(input, "ls"))
           // {
           // 	write(sockfd, &ls, sizeof(ls));
	   // }
	    else if(!strcmp(input, "quit"))
	    {
		quit = 1;
	    }
	    else 
	    {
		int i = 0;
		write(sockfd, &def, sizeof(def));
		read(sockfd, &buffer, 255);
		write(sockfd, cmd, 100);
		//write(sockfd, args, strlen(args));
		read(sockfd, &size, sizeof(size));
                printf("running get\n");
                file = fopen(buffer, "w");
                printf("Size: %d\n", size);
                while(size >= 255)
                {
                        n = read(sockfd, buffer, 255);
                        printf("%s", buffer);
                        size = size - n;
                }
                if(size > 0)
                {
                        read(sockfd, buffer, size);
                        printf("%s", buffer);
                }
                printf("leaving get\n");
                fclose(file);	
           }
	}
        bzero(buffer,256);
        close(sockfd);
    }
    return value;
}
