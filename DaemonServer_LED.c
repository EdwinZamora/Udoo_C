#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>	// this includes the "sockaddr_in" structure
#include <errno.h>
#include <stddef.h>
#include <wchar.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <pthread.h>



void * ClientHandling (void * Client);

int main (int argc, char ** argv)
{
	int32_t ListenSocket;
	struct sockaddr_in *SocketSettings;
	struct sockaddr_in *NewClientSocketSettings;
	int32_t ErrorChecking = 0;
	int32_t NewClientSocket;
	pthread_t ClientThread;
    int32_t TargetPort;
    socklen_t NewClientSize;
	int32_t * Client;

    if(argc != 2)
    {
        printf("Wrong amount of arguments.\nUsage:./server_daemon <PORT>\n");
        return -1;
    }


	/* Create the server project*/
	ListenSocket = socket(AF_INET,SOCK_STREAM,0);

    /* just in case the socket failed */
	if(ListenSocket < 0) 
	{

		printf("Error creating socket\n");
		return(-1);
	}


	TargetPort = atoi(argv[1]);
    
    printf("Port %d\n",TargetPort);

    /* Create the socket settings and configure */
	SocketSettings = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
	
	SocketSettings->sin_family = AF_INET;
	SocketSettings->sin_addr.s_addr = INADDR_ANY;
	SocketSettings->sin_port = htons(TargetPort);

	/* Bind the socket to the settings */
	ErrorChecking = bind(ListenSocket, (struct sockaddr*)SocketSettings,sizeof(struct sockaddr_in));

	if(ErrorChecking  < 0)
	{

		printf("Error binding socker \n");
		return (-1);

	}

	/* Set the socket to listen mode*/
	listen(ListenSocket, 0);

	printf("Waiting for a client...\n");


	/* Create the client socket settings */
	NewClientSocketSettings = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));

    NewClientSize = sizeof(struct sockaddr_in);

    while(1)
    {
        /* Accept incoming connections */
    	NewClientSocket = accept(ListenSocket, (struct sockaddr*)NewClientSocketSettings,&NewClientSize);

	    printf("Client Accepted!!\nCreating Thread\n");
        
		Client = malloc(sizeof(int32_t));

		*Client = NewClientSocket;

        ErrorChecking = pthread_create(&ClientThread,NULL,ClientHandling,(void*)Client); 

        if(ErrorChecking < 0)
        {
            printf("Error creating client thread!\nExiting");
            /* free all resources */
            close(ListenSocket);
            close(NewClientSocket);
            free(SocketSettings);
            free(NewClientSocketSettings);

            return -1;
        }
    }
	/* Close both sockets and exit */
	close(ListenSocket);
	close(NewClientSocket);

	free(SocketSettings);
	free(NewClientSocketSettings);

	return(0);
}

void * ClientHandling(void * Client)
{
    int8_t * ClientBuffer;
    int32_t ClientReceivedSize;
    int32_t* ClientSocket = (int32_t*)Client;

    ClientBuffer = (int8_t*)malloc(200);

    while(1)
    {
        ClientReceivedSize = read(*ClientSocket,ClientBuffer,200);
        
        if(ClientReceivedSize > 0)
        {
            (void)write(*ClientSocket,ClientBuffer,ClientReceivedSize);
        }
        else
        {
            printf("Connection Lost\nClosing thread");
            fflush(stdout);
            close(*ClientSocket);
            free(ClientBuffer);
			free(ClientSocket);
            return NULL;
        }

    }

}