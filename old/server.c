#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "lspol.c"
int main(int argc, char *argv[]) {
 
  pid_t pidp;
  int i,j;
  int gniazdoserwer, gniazdoklient, odpowiedz;
  socklen_t dlugoscserwer,dlugoscklient;
  struct sockaddr_in socketaddres,klientaddres;
  char wiadomosc[2048];
  char sciezka[1024]="/home/szulce241d/Sieci/gniazda";
		j=1;
  gniazdoserwer = socket(AF_INET,SOCK_STREAM,0);
  if( gniazdoserwer == -1 ) {
	  perror("[SERVER] nie udalo sie tworzenie gniazda-serwera\n");
	  return -1;
  }
  
  socketaddres.sin_family = AF_INET;
  socketaddres.sin_port = (int)htons(strtol(argv[1],NULL,10));
  socketaddres.sin_addr.s_addr = htonl(INADDR_ANY);

  dlugoscserwer = sizeof(socketaddres);  
  odpowiedz = bind(gniazdoserwer,(struct sockaddr*)&socketaddres,dlugoscserwer);
  if( odpowiedz == -1 ) {
	perror("[SERVER] blad nadania adresu\n");
	return -1;
  }
  
  odpowiedz = listen(gniazdoserwer,5);//moze obslugiwac 5 na raz
  if( odpowiedz == -1 ) {
  	perror("[SERVER] serwer nie oczekuje na wiadomosc od klienta bo wystapil nieoczekiwany blad\n");
	return -1;
  }
  dlugoscklient = sizeof(klientaddres);
  while(1) {
    printf("[Serwer] oczekuje ...\n");
    gniazdoklient=accept(gniazdoserwer,(struct sockaddr*)&klientaddres,&dlugoscklient);
    if ( gniazdoklient == -1 ) {
	    perror("[SERVER] ACCEPT ERROR\n");
	    return -1;
    }
	if((pidp = fork())<0)
		{
			perror("FORK FAILED\n");
			exit(1);
		}
	if(pidp == 0)
	{
			
   while(( read(gniazdoklient,wiadomosc,1))>0)
    {
	 if(strcmp(wiadomosc,"ls"))
	{
		wiadomosc[0]=0;
		status(sciezka,wiadomosc);
	}
	else 
	{

		strcat(sciezka,"/");
		i=3;
		while(wiadomosc[i]!='\0')
			{sciezka[j]=wiadomosc[i];
				j++;i++;}
	}

	
   write(gniazdoklient,wiadomosc,1);
}
   close(gniazdoklient);
	}
  }

  return 0;
}
