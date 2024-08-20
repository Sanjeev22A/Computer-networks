#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>

#define PORT 8080
#define LocalHost "127.0.0.1"
#define BUFFER_SIZE 1024
#define BACKLOGS 5

int len(char *buffer){
	int i=0;
	for(;buffer[i];i++){
		continue;
	}
	return i;
}
int createSocket(int Stream){
    int serverSocket=socket(AF_INET,Stream,0);
    if(serverSocket<0){
        perror("Error! in creating the socket!(from create socket function)");
        exit(EXIT_FAILURE);
    }
    return serverSocket;
}

struct sockaddr_in setUp(int family,int port,char *localhost){
    struct sockaddr_in serverAddr;
    serverAddr.sin_family=family;
    serverAddr.sin_port=htons(port);
    serverAddr.sin_addr.s_addr=inet_addr(localhost);
    return serverAddr;
}

int bindSocket(int sockFd,struct sockaddr *serverAddr,socklen_t bufferSize){
    int returnValue=bind(sockFd,serverAddr,bufferSize);
    if(returnValue<0){
        perror("Error! in binding the socket!( from bindSocket function)");
        exit(EXIT_FAILURE);
        
    }
    return returnValue;
}

int startServer(int sockfd,int backlogs){
    int returnValue=listen(sockfd,backlogs);
    if(returnValue<0){
        perror("Error ! in starting the server (from start server function)");
        exit(EXIT_FAILURE);
    }
    printf("Server started at port :%d\n",PORT);
    return returnValue;
}

int acceptConnection(int sockfd,struct sockaddr *clientaddr,int *length){
    int returnValue=accept(sockfd,clientaddr,length);
    if(returnValue<0){
        perror("Error ! while accepting connection (from acceptConnection function)");
        exit(EXIT_FAILURE);
    }
    return returnValue;
}

void clear(char *buffer){
	for(int i=0;buffer[i];i++){
		buffer[i]='\0';
	}
}

int sendFile(int sockFd,FILE *Fp){
	char buffer[BUFFER_SIZE];
	while(fgets(buffer,BUFFER_SIZE,Fp)!=NULL){
		printf("%s",buffer);
		int returnVal=write(sockFd,buffer,len(buffer));
		if(returnVal==-1){
			perror("Error while writing to the file(from sendFile function)\n");
			exit(EXIT_FAILURE);
		}
		memset(buffer,0,BUFFER_SIZE);
	}
	printf("File transfer complete\n");
	return 1;
}

void transferFile(int sockFd){
	char filename[100];
	printf("Enter filename : ");
	scanf("%s",filename);
	FILE *fp=fopen(filename,"r");
	sendFile(sockFd,fp);
}


int main(){
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t clientLength;
    int serverSocket=createSocket(SOCK_STREAM);

    server_addr=setUp(AF_INET,PORT,LocalHost);

    bindSocket(serverSocket,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_in));

    startServer(serverSocket,BACKLOGS);

    while(1){
        int clientSocket=acceptConnection(serverSocket,(struct sockaddr *)&server_addr,&clientLength);

        transferFile(clientSocket);

        close(clientSocket);
        break;

    }   

    close(serverSocket);

}

