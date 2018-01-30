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
void status(char *nazwa);

void dir(char *nazwa)
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

						if(S_ISDIR(buf2.st_mode)==1)dir(f);									
						status(f);
						free(f);					
					}
		}
	closedir(ds);
}
void status(char *nazwa)
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
	
	printf("%s\n",nazwapliku);
	czasz = buf.st_mtime;
	czasc = buf.st_ctime;
        czasd = buf.st_atime;
	size = buf.st_size;
	userid=buf.st_uid;
	login=getpwuid(userid)->pw_name;
	katalog=0;
	
	if(S_ISDIR(buf.st_mode)==1){printf("katalog \n");katalog=1;}
	else if(S_ISREG(buf.st_mode)==1)printf("plik zwykly  ");
	else if(S_ISLNK(buf.st_mode)==1)printf("dowizanie symboliczne ");
	else if(S_ISBLK(buf.st_mode)==1)printf("blokowy ");
	else if(S_ISCHR(buf.st_mode)==1)printf("znakowy ");
	else if(S_ISFIFO(buf.st_mode)==1)printf("potok ");
	else if(S_ISSOCK(buf.st_mode)==1)printf("gniazdo ");
	
	if(S_IXUSR & buf.st_mode)printf("x");
	if(S_IRUSR & buf.st_mode)printf("r");
	if(S_IWUSR & buf.st_mode)printf("w");
	printf("-");
	if(S_IXGRP & buf.st_mode)printf("x");
	if(S_IRGRP & buf.st_mode)printf("r");
	if(S_IWGRP & buf.st_mode)printf("w");
	printf("-");
	if(S_IXOTH & buf.st_mode)printf("x");
	if(S_IROTH & buf.st_mode)printf("r");
	if(S_IWOTH & buf.st_mode)printf("w");
	
	printf("\nrozmiar:%dB ",size);
	printf(" i-wezel:%d ",buf.st_ino);
	printf("user:%s ",login);
	printf("GID:%d ",buf.st_gid);
	printf("l_dowiazan:%d \n",buf.st_nlink);
	printf("ostatni dostep: %s",ctime(&czasd));
	printf("zmiana cech pliku: %s",ctime(&czasc));
	printf("modyfikacja zawartosci pliku: %s\n",ctime(&czasz));
	
	if(katalog==1)
	{	
	   dir(nazwapliku);
	   katalog=0;
	}

	}
	else
	{printf("Nie ma takiego pliku ani katalogu\n");}
	free(nazwapliku);

}

int main(int arcg ,char * argv[])
{ status(argv[1]);
	return 0;}
