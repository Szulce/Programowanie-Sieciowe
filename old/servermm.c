#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include<dirent.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<pwd.h>

char wiadomosc[10000];

void status(char *nazwa,char *gdzie);

void dir(char *nazwa,char *gdzie)
{

        DIR *ds;
        struct dirent *k;
        char *KOM;
        char *f;
        struct stat buf2;
        if((ds=opendir(nazwa))==NULL){printf("BLAD OTWARCIA\n");return;}
        while((k=readdir(ds))!=NULL)
        {
                KOM = k->d_name;

                if(strcmp(KOM,".")!=0 && strcmp(KOM,"..")!=0)
                                        {
					
                                                lstat(KOM,&buf2);
                                                f=(char*)calloc(strlen(nazwa)+1,sizeof(char));
                                                strcpy(f,nazwa);
                                                strcat(f,"/");
                                                strcat(f,KOM);

                                        //        if(S_ISDIR(buf2.st_mode)==1)dir(f,gdzie);                                                       
                                                status(f,gdzie);}
                                                free(f);
                }
        closedir(ds);
}

void status(char *nazwa,char *gdzie)
{

        struct stat buf;
        //time_t czasz,czasc;
	time_t czasd;
       // int size;
        int katalog;
        uid_t userid;
        char *login;
       char nazwapliku[10000];

        //nazwapliku=(char*)calloc(strlen(nazwa)+1,sizeof(char));
        strcpy(nazwapliku,nazwa);
        if(lstat(nazwa,&buf)==0){
        
	strcat(gdzie,nazwapliku);
        strcat(gdzie," ");
        //czasz = buf.st_mtime;
        //czasc = buf.st_ctime;
        czasd = buf.st_atime;
        //size = buf.st_size;
        userid=buf.st_uid;
        login=getpwuid(userid)->pw_name;
        katalog=0;

        if(S_ISDIR(buf.st_mode)==1){strcat(gdzie,"katalog \n");katalog=1;}
        else if(S_ISREG(buf.st_mode)==1)strcat(gdzie,"plik zwykly  ");
        else if(S_ISLNK(buf.st_mode)==1)strcat(gdzie,"dowizanie symboliczne ");
        else if(S_ISBLK(buf.st_mode)==1)strcat(gdzie,"blokowy ");
        else if(S_ISCHR(buf.st_mode)==1)strcat(gdzie,"znakowy ");
        else if(S_ISFIFO(buf.st_mode)==1)strcat(gdzie,"potok ");
        else if(S_ISSOCK(buf.st_mode)==1)strcat(gdzie,"gniazdo ");

        if(S_IXUSR & buf.st_mode)strcat(gdzie,"x");
        if(S_IRUSR & buf.st_mode)strcat(gdzie,"r");
        if(S_IWUSR & buf.st_mode)strcat(gdzie,"w");
         strcat(gdzie,"-");
	if(S_IXGRP & buf.st_mode)strcat(gdzie,"x");
        if(S_IRGRP & buf.st_mode)strcat(gdzie,"r");
        if(S_IWGRP & buf.st_mode)strcat(gdzie,"w");
        strcat(gdzie,"-");
//      strcat(gdzie,"\nrozmiar: ");
//      strcat(gdzie,size);
//      printf(" i-wezel:%d ",buf.st_ino);
        strcat(gdzie,"user: ");
        strcat(gdzie,login);
//      strcat(gdzie,"GID: ");
//      strcat(gdzie,buf.st_gid);
//      strcat("l_dowiazan:%d \n",buf.st_nlink);
        strcat(gdzie,"ostatni dostep: ");
        strcat(gdzie,ctime(&czasd));
//        strcat(gdzie,"zmiana cech pliku: ");
//        strcat(gdzie,ctime(&czasc));
//        strcat(gdzie,"modyfikacja zawartosci pliku: ");
//        strcat(gdzie,ctime(&czasz));
//tousun	strcat(gdzie,"\n");

//printf("gdziesize:%d\n",strlen(gdzie));
       if(katalog==1)
        {
 	  dir(nazwapliku,gdzie);
          katalog=0;
        }
        }
        else
        {printf("Nie ma takiego pliku ani katalogu\n");}
//free(nazwapliku);
}


int main(int argc, char *argv[]) {
 
  pid_t pidp;
  int i,j;
  int gniazdoserwer, gniazdoklient, odpowiedz;
  socklen_t dlugoscserwer,dlugoscklient;
  struct sockaddr_in socketaddres,klientaddres;
  char sciezka[2048]=".";
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

   while(( read(gniazdoklient,&wiadomosc,sizeof(wiadomosc)))>0)
    {
	printf("[SERVER] Wiadomosc : %s\n",wiadomosc);
	
	 if(wiadomosc[0]=='l'&& wiadomosc[1]=='s') 
	{
		printf("\nsciezka %s\n",sciezka);	
		wiadomosc[0]='\0';
		status(sciezka,wiadomosc);
		printf("wiad size %d",strlen(wiadomosc));
	}
	else 
	{
		printf("[SERVER]sciezka przed:%s\n",sciezka);
		strcat(sciezka,"/");
		i=3;
		while(wiadomosc[i]!='\0' && wiadomosc[i]!=' ' && wiadomosc[i]!='\t') 
			{sciezka[j]=wiadomosc[i];
				j++;i++;}
		printf("[SERVER]sciezka po %s\n",sciezka);
		wiadomosc[0]='\0';
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
