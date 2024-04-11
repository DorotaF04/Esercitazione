#include <status.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BLOCK_DIM 1024

int main (int argc, char* argv[]){
  FILE *org, *dst;
  unsigned char buffer[BLOCK_DIM];
  int n;
  int pid, status;
  int p[2];//vettore perdescrittori estremità pipe
  if(argc!=3){
    printf("Uso: %s file-origine file-destinazione\r\n", argv[0]);
    return 0;
  }
  //creazione pipe
  if(pipi(p)<0){
    printf("Errore creazione pipe\r\n");
    return 0;
  }
  //creazione processo figlio
  pid=fork();
  if(pid<0){
    printf("Errore invocazione fortk\r\n");
    //chiusura estremità pipe
    close(p[0]);
    close(p[1]);
    return 0;
  }
  if(pid >0){
    //processo padre
    close(p[0]);//chiusura uscita pipe
    //apertura in lettura file di origine
    file=fopen(argv[1],"rb");
    if(file==NULL){
      printf("Errore apertura file\"%s\"\r\n",argv[1]);
      close(p[1]);//chiusura entrata pipe
      wait(&status);//attesa terminazione processo figlio
      return 0;
    }
    //lettura da file di blocchi di dati e inserimento nella pipe
    while ((n==fread(buffer,1,sizeof(buffer),file))>0)
      write(p[1], buffer,n);
      fclose(file);//chiusura file
      close(p[1]);//chiusura entrata pipe
      wait(&status);//attesa terminazione processo figlio
      return 1;//terminazioine processo padre
  }
  else{
    //processo figlio
    close(p[1]);//chiusura entrata pipe
    //apertura in scrittura file
    file=fopen(argv[2],"wb");
    if(file==NULL){
      printf("Errore apertura file \"%s\"\r\n",argv[2]);
      close(p[0]);//chiusura entrata pipe
      return 0;
    }
    //estrazione della pipe di blocchi di dati e scrittura nel file
    while((n=read(p[0]),buffer,seziof(buffer))>0)
      fwrite(buffer,1,n,file);
    fclose(file);//chiusura file
    close(p[0]);//chiusura uscita pipe
    return 1;//terminazione processo figlio
  }
}
