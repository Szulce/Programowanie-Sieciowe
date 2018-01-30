#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
  int gs, gk, res;
  socklen_t dls,dlk;
  struct sockaddr_in saddr,kaddr;
  char zn;

  /*tworzymy gniazdo serwera*/
  gs = socket(AF_INET,SOCK_STREAM,0);
  if( gs == -1 ) {
	  perror("SERVER: blad socket");
	  return -1;
  }
  
  /*nazywamy gniazdo serwera*/
  saddr.sin_family = AF_INET;
  
  //atoi - deprecated non thread safe
  //saddr.sin_port = htons(atoi(argv[1]));
  saddr.sin_port = (int)htons(strtol(argv[1],NULL,10));
  //INADDR_ANY - gniazdo dotyczy wszystkich interfejsow sieciowych
  //hosta
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  //wybranie konkretnego interfejsu sieciowego ultra60
  //inet_pton(AF_INET,"158.75.2.120",&(saddr.sin_addr));
  dls = sizeof(saddr);  
  res = bind(gs,(struct sockaddr*)&saddr,dls);
  if( res == -1 ) {
	perror("SERVER: blad bind");
	return -1;
  }
  
  //tworzymy kolejke polaczen i czekamy na klientow
  res = listen(gs,5);
  if( res == -1 ) {
  	perror("SERVER: blad listen");
	return -1;
  }
  dlk = sizeof(kaddr);
  while(1) {
    printf("Serwer czeka\n");
    //akceptujemy polaczenie, tworzymy gniazdo do obslugi klienta
    gk=accept(gs,(struct sockaddr*)&kaddr,&dlk);
    if ( gk == -1 ) {
	    perror("SERVER: blad accept");
	    return -1;
    }
    read(gk,&zn,1);
    zn++;
    write(gk,&zn,1);
    close(gk);
  }
  return 0;
}
