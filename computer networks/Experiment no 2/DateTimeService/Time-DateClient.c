#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<time.h>
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
    int returnValue=bind(sockFd,serverAddr,sizeof(serverAddr));
    if(returnValue<0){
        perror("Error! in binding the socket!( from bindSocket function)");
        exit(EXIT_FAILURE);
        
    }
    return returnValue;
}

void clean(char *buffer){
	int i=0;
	while(buffer[i]){
		buffer[i++]='\0';
	}
}

void append(char *buffer,char *data){
	int i=len(buffer);
	int j=0;
	while(data[j]){
		buffer[i++]=data[j++];
	}
}


int chat(int sockFd){
    char buffer[BUFFER_SIZE];


    ssize_t bytesRead=read(sockFd,buffer,sizeof(buffer)-1);
    if(bytesRead<0){
        printf("End of chat\n");
        return 0;

    }
    buffer[bytesRead]='\0';
    printf("Message from server :%s\n",buffer);
    if(strcmp(buffer,"Over")==0){
        printf("End of Chat");
        return 0;
    }
    

    
    time_t  t;
    time(&t);
    clean(buffer);
    printf("Message from client : Time sent to server\n");
    append(buffer,"Time from client\0");
    append(buffer,ctime(&t));    
    write(sockFd,buffer,strlen(buffer));

    if (strcmp(buffer, "Over\n") == 0) {
        printf("End of Chat\n");
        return 0;
    }

    
    return 1;
}

int connectSocket(int sockFd,struct sockaddr * addr,socklen_t size){
    int returnValue=connect(sockFd,addr,size);
    if(returnValue<0){
        perror("Error ! in connecting socket (from connect socket function)");
        exit(EXIT_FAILURE);
    }
    return returnValue;
}

int main(){
    int clientSocket;
    struct sockaddr_in clientAddr;
    clientSocket=createSocket(SOCK_STREAM);

    clientAddr=setUp(AF_INET,PORT,LocalHost);

    connectSocket(clientSocket,(struct sockaddr *)&clientAddr,sizeof(struct sockaddr_in));

    chat(clientSocket);
    close(clientSocket);
    return 0;
}
