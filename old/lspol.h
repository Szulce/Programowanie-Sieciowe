#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<pwd.h>

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

						if(S_ISDIR(buf2.st_mode)==1)dir(f,gdzie);									
						status(f,gdzie);
						free(f);					
					}
		}
	closedir(ds);
}

//wyzerowac wiadomosc wczesniej
void status(char *nazwa,char *gdzie)
{
	
	struct stat buf;
	time_t czasz,czasc,czasd;
	int size;
	int katalog;
	uid_t userid;
	char *login;
	char *nazwapliku;
	
	nazwapliku=(char*)calloc(strlen(nazwa)+1,sizeof(char));
	strcpy(nazwapliku,nazwa);
	
	if(lstat(nazwa,&buf)==0){
	
	strcat(gdzie,nazwapliku);
	strcat(gdzie," ");
	czasz = buf.st_mtime;
	czasc = buf.st_ctime;
        czasd = buf.st_atime;
	size = buf.st_size;
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
	if(S_IXOTH & buf.st_mode)strcat(gdzie,"x");
	if(S_IROTH & buf.st_mode)strcat(gdzie,"r");
	if(S_IWOTH & buf.st_mode)strcat(gdzie,"w");
	
//	strcat(gdzie,"\nrozmiar: ");
//	strcat(gdzie,size);
//	printf(" i-wezel:%d ",buf.st_ino);
	strcat(gdzie,"user: ");
	strcat(gdzie,login);
	strcat(gdzie,"GID: ");
//	strcat(gdzie,buf.st_gid);
//	printf("l_dowiazan:%d \n",buf.st_nlink);
	strcat(gdzie,"ostatni dostep: ");
	strcat(gdzie,ctime(&czasd));
	strcat(gdzie,"zmiana cech pliku: ");
	strcat(gdzie,ctime(&czasc));
	strcat(gdzie,"modyfikacja zawartosci pliku: \n");
	strcat(gdzie,ctime(&czasz));
	
	if(katalog==1)
	{	
	   dir(nazwapliku,gdzie);
	   katalog=0;
	}

	}
	else
	{printf("Nie ma takiego pliku ani katalogu\n");}


}

