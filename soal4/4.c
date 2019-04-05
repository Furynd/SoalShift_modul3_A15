#include<sys/wait.h>
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
    pthread_t thread1,thread2;
    pthread_t zip1,zip2;
    pthread_t hps1,hps2;
    pthread_t unzip1,unzip2;

void *make_file1( void *ptr ) {
    system("ps -aux | head -11 > ~/Documents/FolderProses1/SimpanProses1.txt");
}
void *make_file2( void *ptr ) {
    system("ps -aux | head -11 > ~/Documents/FolderProses2/SimpanProses2.txt");
}

void *zip_file1( void *ptr ) {
    pthread_join( thread1, NULL);
    system(" cd ~/Documents/FolderProses1 && zip SimpanProses1.zip SimpanProses1.txt");
}
void *zip_file2( void *ptr ) {
    pthread_join( thread2, NULL);
    system(" cd ~/Documents/FolderProses2 && zip SimpanProses2.zip SimpanProses2.txt");
}

void *hapus1( void *ptr ) {
    pthread_join( zip1, NULL);
    system(" rm ~/Documents/FolderProses1/SimpanProses1.txt");
    printf("Menunggu 15 detik untuk mengekstrak kembali\n");
    sleep(15);
}
void *hapus2( void *ptr ) {
    pthread_join( zip2, NULL);
    system(" rm ~/Documents/FolderProses2/SimpanProses2.txt");
    printf("Menunggu 15 detik untuk mengekstrak kembali\n");
    sleep(15);
}

void *unzip_file1( void *ptr ) {
    pthread_join( hps1, NULL);
    system("cd ~/Documents/FolderProses1 && unzip SimpanProses1.zip");
}
void *unzip_file2( void *ptr ) {
    pthread_join( hps2, NULL);
    system("cd ~/Documents/FolderProses2 && unzip SimpanProses2.zip");
}

int main() {

    pthread_create( &thread1, NULL, make_file1,NULL);
    pthread_create( &thread2, NULL, make_file2,NULL);

    pthread_create( &zip1, NULL, zip_file1,NULL);
    pthread_create( &zip2, NULL, zip_file2,NULL);

    pthread_create( &hps1, NULL, hapus1,NULL);
    pthread_create( &hps2, NULL, hapus2,NULL);

    pthread_create( &unzip1, NULL, unzip_file1,NULL);
    pthread_create( &unzip2, NULL, unzip_file2,NULL);

    pthread_join( unzip1, NULL);
    pthread_join( unzip2, NULL);

  return 0;
}
