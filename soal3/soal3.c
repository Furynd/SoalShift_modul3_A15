#include<sys/wait.h>
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
int aws=0;
int iss=100;
int f_aws=0;
int f_iss=0;
pthread_t t1,t2;
pthread_t s1,s2;

void *aws_stall( void *ptr ) {
    printf("Agmal Ayo Bangun disabled 10 s\n");
    sleep(10);
    f_aws=0;
}

void *iss_stall( void *ptr ) {
    printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
    sleep(10);
    f_iss=0;
}

void *agmal_bangun( void *ptr ) {
    aws+=15;
    if(aws >= 100)
    {
        printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");
        exit(EXIT_SUCCESS);
    }
    else
    {
        f_iss++;
        if(f_iss==3) pthread_create( &s2, NULL, iss_stall,NULL);
    }
}

void *iraj_tidur( void *ptr ) {
    iss-=20;
    if(iss <= 0)
    {
         printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
         exit(EXIT_SUCCESS);
    }
    else
    {
        f_aws++;
        if(f_aws==3) pthread_create( &s1, NULL, aws_stall,NULL);
    }
}


int main() {

    int pilihan;

    while(1)
    {
        printf("Pilihan :\n");
        printf("1.All Status\n");
        printf("2.Agmal Ayo Bangun\n");
        printf("3.Iraj Ayo Tidur\n");
        printf("Input Pilihan : "); 
        scanf("%d",&pilihan);

        if(pilihan==1)
        {
            printf("Agmal WakeUp_Status = %d\n",aws);
            printf("Iraj Spirit_Status = %d\n\n",iss);
        }
        else if(pilihan==2)
        {
            if(f_aws<3){ pthread_join( t1, NULL); pthread_create( &t1, NULL, agmal_bangun,NULL); }
            else printf("Agmal Ayo Bangun disabled 10 s\n");
        }
        else 
        {
            if(f_iss<3){pthread_join( t2, NULL); pthread_create( &t2, NULL, iraj_tidur,NULL); }
            else printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
        }
             
    }

  return 0;
}
