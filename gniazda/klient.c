#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

 char wiadomosc[10000];

int main(int argc, char *argv[]) {
 
 int gniazdoklient,dlugosc,odpowiedz;
  struct sockaddr_in sockaddres;
  struct hostent *host;

 
host = gethostbyname2(argv[1],AF_INET);

  if(!host) { 
	  perror("[KLIENT] nie znaleziono hosta o takiej nazwie\n");
	  return -1; 
  }
  gniazdoklient = socket(AF_INET,SOCK_STREAM,0); 
  if ( gniazdoklient == -1 ) {
	perror("[KLIENT] blad tworzenia gniazda\n");
	return -1;
  }


  sockaddres.sin_family = AF_INET;
  sockaddres.sin_port = (int)htons(strtol(argv[2],NULL,10));
  sockaddres.sin_addr = *(struct in_addr*)*host->h_addr_list;
  dlugosc = sizeof(sockaddres);

odpowiedz = connect(gniazdoklient,(struct sockaddr*)&sockaddres,dlugosc);
if(odpowiedz==-1) {
    perror("[KLIENT] nie udalo sie skomunikowac z serwerem\n");
    return -1;    
  }


  printf("wprowadz polecenie \ncd katalog : jesli chcesz zmienic sciezke \nls: jesli chesz wylistowac zawartosc katalogu \nkn: jesli chesz zakonczyc\n");
 while(1)
{
	 fgets(wiadomosc,10000,stdin);
	if(wiadomosc[0]=='k' && wiadomosc[1]=='n')break;
 	else { 
	write(gniazdoklient,wiadomosc,sizeof(wiadomosc));
	printf("[KLIENT] Czekam na odpowiedz serwera\n");
        read(gniazdoklient,wiadomosc,sizeof(wiadomosc));
  	printf("[ODPOWIEDZ] %s\n",wiadomosc);
	}
}
  close(gniazdoklient);

  return 0;
}
