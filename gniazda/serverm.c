#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pwd.h>

char wiadomosc[10000];

void status(char *nazwa);

void dir(char *nazwa)
{

        DIR *ds;
        struct dirent *k;
        char *KOM;
	char f[10000];
        struct stat buf2;
        if((ds=opendir(nazwa))==NULL){printf("BLAD OTWARCIA\n");return;}
        while((k=readdir(ds))!=NULL)
        {
                KOM = k->d_name;

                if(strcmp(KOM,".")!=0 && strcmp(KOM,"..")!=0)
                                        {
					
                                                lstat(KOM,&buf2);
                                                strcpy(f,nazwa);
                                                strcat(f,"/");
                                                strcat(f,KOM);

                                                status(f);
					}
                }
        closedir(ds);
}

void status(char *nazwa)
{

        struct stat buf;
        //time_t czasz,czasc;
	time_t czasd;
        int size;
        int katalog;
        uid_t userid;
        char *login;
	char roz[100];
        if(lstat(nazwa,&buf)==0){
        
	strcat(wiadomosc,nazwa);
        strcat(wiadomosc," ");
        //czasz = buf.st_mtime;
        //czasc = buf.st_ctime;
        czasd = buf.st_atime;
        size = buf.st_size;
        userid=buf.st_uid;
        login=getpwuid(userid)->pw_name;
	sprintf(roz,"%d ",size);

        katalog=0;

        if(S_ISDIR(buf.st_mode)==1){strcat(wiadomosc,"katalog \n");katalog=1;}
        else if(S_ISREG(buf.st_mode)==1)strcat(wiadomosc,"plik zwykly\n");
        else if(S_ISLNK(buf.st_mode)==1)strcat(wiadomosc,"dowizanie symboliczne\n");
        else if(S_ISBLK(buf.st_mode)==1)strcat(wiadomosc,"blokowy\n");
        else if(S_ISCHR(buf.st_mode)==1)strcat(wiadomosc,"znakowy\n");
        else if(S_ISFIFO(buf.st_mode)==1)strcat(wiadomosc,"potok\n");
        else if(S_ISSOCK(buf.st_mode)==1)strcat(wiadomosc,"gniazdo\n");

        if(S_IXUSR & buf.st_mode)strcat(wiadomosc,"x");
        if(S_IRUSR & buf.st_mode)strcat(wiadomosc,"r");
        if(S_IWUSR & buf.st_mode)strcat(wiadomosc,"w");
         strcat(wiadomosc,"-");
	if(S_IXGRP & buf.st_mode)strcat(wiadomosc,"x");
	if(S_IRGRP & buf.st_mode)strcat(wiadomosc,"r");
        if(S_IWGRP & buf.st_mode)strcat(wiadomosc,"w");
        strcat(wiadomosc,"-");
	if(S_IXOTH & buf.st_mode)strcat(wiadomosc,"x");
	if(S_IXOTH & buf.st_mode)strcat(wiadomosc,"x");
	if(S_IXOTH & buf.st_mode)strcat(wiadomosc,"x");
        
        strcat(wiadomosc," rozmiar: ");
        strcat(wiadomosc,roz);
//      strcat(" i-wezel:%d ",buf.st_ino);
        strcat(wiadomosc," user: ");
        strcat(wiadomosc,login);
//      strcat(gdzie,"GID: ");
//      strcat(gdzie,buf.st_gid);
//      strcat("l_dowiazan:%d \n",buf.st_nlink);
        strcat(wiadomosc,"\nostatni dostep: ");
        strcat(wiadomosc,ctime(&czasd));
//        strcat(gdzie,"zmiana cech pliku: ");
//        strcat(gdzie,ctime(&czasc));
//        strcat(gdzie,"modyfikacja zawartosci pliku: ");
//        strcat(gdzie,ctime(&czasz));
	strcat(wiadomosc,"\n");
       
if(katalog==1)
        {
 	  dir(nazwa);
          katalog=0;
        }
        }
        else
        {printf("Nie ma takiego pliku ani katalogu\n");}
}


int main(int argc, char *argv[]) {
 
  pid_t pidp;
  int i,j,k;
  int gniazdoserwer, gniazdoklient, odpowiedz;
  socklen_t dlugoscserwer,dlugoscklient;
  struct sockaddr_in socketaddres,klientaddres;
  char sciezka[10000]=".";
		j=strlen(sciezka)+1;
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
  
  odpowiedz = listen(gniazdoserwer,5);
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

   while(( read(gniazdoklient,&wiadomosc,sizeof(wiadomosc)))>0)
    {
	printf("[SERVER] Wiadomosc : %s\n",wiadomosc);
	
	 if(wiadomosc[0]=='l'&& wiadomosc[1]=='s') 
	{
		wiadomosc[0]=0;
		status(sciezka);
	}
	else 
	{
		strcat(sciezka,"/");
		i=3;
		k=strlen(wiadomosc)-1;
	
	while(i!=k) 
		{
			sciezka[j]=wiadomosc[i];
			j++;i++;
				
		}
		j++;
		//printf("[SERVER]sciezka po %s\n",sciezka);
		wiadomosc[0]=0;
		strcat(wiadomosc,"znajdujesz sie w ");
		strcat(wiadomosc,sciezka);
	}

   	write(gniazdoklient,&wiadomosc,sizeof(wiadomosc));


	printf("[SERWER] Wykonano instrukcje i odeslano odpowiedz\n");
}
   close(gniazdoklient);
	}
  }

  return 0;
}
