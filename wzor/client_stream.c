#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
  int gk,dl,res;
  struct sockaddr_in saddr;
  struct hostent *host;
  char zn;

  host = gethostbyname(argv[1]);
  if(!host) { 
	  perror("KLIENT: blad gethostbyname");
	  return -1; 
  }
  gk = socket(AF_INET,SOCK_STREAM,0); 
  if ( gk == -1 ) {
	perror("KLIENT: blad socket");
	return -1;
  }
  saddr.sin_family = AF_INET;
  //atoi - deprecated non thread safe
  //saddr.sin_port = htons(atoi(argv[2]));
  saddr.sin_port = (int)htons(strtol(argv[2],NULL,10));
  saddr.sin_addr = *(struct in_addr*)*host->h_addr_list;
  dl = sizeof(saddr);
  
  res = connect(gk,(struct sockaddr*)&saddr,dl);
  if(res==-1) {
    perror("KLIENT: blad connect !!!");
    return -1;    
  }
  
  printf("Podaj jakis znak z klawiatury: ");
  scanf("%c",&zn);
  write(gk,&zn,1);
  read(gk,&zn,1);
  printf("Odebralem od serwera = %c\n",zn);
  close(gk);
  return 0;
}
