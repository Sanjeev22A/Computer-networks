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
    int returnValue=bind(sockFd,serverAddr,sizeof(serverAddr));
    if(returnValue<0){
        perror("Error! in binding the socket!( from bindSocket function)");
        exit(EXIT_FAILURE);
        
    }
    return returnValue;
}




int connectSocket(int sockFd,struct sockaddr * addr,socklen_t size){
    int returnValue=connect(sockFd,addr,size);
    if(returnValue<0){
        perror("Error ! in connecting socket (from connect socket function)");
        exit(EXIT_FAILURE);
    }
    return returnValue;
}

int len(char *buffer){
	int i=0;
	for(;buffer[i];i++){
		continue;
	}
	return i;
}

void clean(char *buffer){
	for(int i=0;buffer[i];i++){
		buffer[i]='\0';
	}
}

int receiveFile(int sockFd,FILE *fp){
	char buffer[BUFFER_SIZE];
	while(read(sockFd,buffer,sizeof(buffer)) >0){
		printf("%s\n",buffer);
		fprintf(fp,"%s",buffer);
		//clean(buffer);
		bzero(buffer,BUFFER_SIZE);
	}
	printf("File successfully written\n");
	return 1;		
}

void receiveTrigger(int sockFd){
	char filename[100];
	printf("Enter a destination file :");
	scanf("%s",filename);
	FILE *fp=fopen(filename,"w");
	receiveFile(sockFd,fp);
}

int main(){
    int clientSocket;
    struct sockaddr_in clientAddr;
    clientSocket=createSocket(SOCK_STREAM);

    clientAddr=setUp(AF_INET,PORT,LocalHost);

    connectSocket(clientSocket,(struct sockaddr *)&clientAddr,sizeof(struct sockaddr_in));
    receiveTrigger(clientSocket);
    
    close(clientSocket);
    return 0;
}
