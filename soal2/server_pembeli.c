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
#include <sys/ipc.h>
#include <sys/shm.h>
#define PORT 8080
int server_fd, new_socket, valread;
char buffer[1024] = {0};
char hello[100] ;
pthread_t t,k,w;

void *kirim( void *ptr ) {
    send(new_socket , hello , strlen(hello) , 0 );
}
void *terima( void *ptr ) {
    int *value=(int *) ptr;
    memset(buffer,'\0',sizeof(buffer));
    if(read( new_socket , buffer, 1024))
    {
        memset(hello,'\0',sizeof(hello));
        if(!strcmp(buffer,"beli"))
        {
            if(*value > 0)
            {   
                strcpy(hello,"transaksi berhasil");
                *value = *value-1;
            }
            else
            {
                strcpy(hello,"transaksi gagal");
            }
        }
        else  strcpy(hello,"Kode tidak dikenali");
        pthread_create( &k, NULL, kirim,NULL);
        pthread_join(k,NULL);
    }
}

int main(int argc, char const *argv[]) {
    // Shared Memory
    key_t key = 1234;
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    *value = 3;

    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
  
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    //address.sin_addr.s_addr = INADDR_ANY;
    address.sin_addr.s_addr = inet_addr("127.1.0.1");
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        pthread_create( &t, NULL, terima,(void *) value);
        pthread_join(t,NULL);
    }

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}