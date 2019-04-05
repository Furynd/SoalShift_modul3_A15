#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<sys/wait.h>
#include<pthread.h>
#include<sys/types.h>
#define PORT 8080
int sock = 0, valread;
char buffer[1024] = {0};
char hello[1000] ;
pthread_t t,k;

void *kirim( void *ptr ) {
    memset(hello,'\0',sizeof(hello));
    if(scanf("%s",hello)>0)
    {
        send(sock , hello , strlen(hello) , 0 );
        getchar();
    }
}
void *terima( void *ptr ) {
    memset(buffer,'\0',sizeof(buffer));
    if(read( sock , buffer, 1024))
    printf("SERVER : %s\n",buffer );
}
  
int main(int argc, char const *argv[]) {
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.1.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while(1)
    {
        pthread_create( &k, NULL, kirim,NULL);
        pthread_create( &t, NULL, terima,NULL);
        pthread_join(k,NULL);
        pthread_join(t,NULL);
    }
    return 0;
}