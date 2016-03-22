#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>

//implement a command of linux:"ls-l". 

void printstat(char *dir){
	DIR *dp=opendir(dir);
	if(NULL==dp){
		fprintf(stderr,"cannot open dir:%s\n",dir);
		return;
	}
	struct stat statbuf;
	struct dirent *dpInfo;
	char buf[512], type, time[20]={0};
	char *pow[3];
	int ret, i, t;
	while((dpInfo=readdir(dp))!=NULL){
		bzero(buf,sizeof(buf));	
		sprintf(buf,"%s/%s",dir,dpInfo->d_name);
		ret=stat(buf,&statbuf);
		
		int mode=statbuf.st_mode;
		if(mode>>12==8)
			type='-';
		else if(mode>>12==4)
			type='d';
		else
			type='o';
			
		t=7;
		for(i=0;i<3;++i){
			switch(mode&t){
				case 0:pow[i]="---";break;
				case 2:pow[i]="-w-";break;
				case 1:pow[i]="--x";break;
				case 3:pow[i]="-wx";break;
				case 4:pow[i]="r--";break;
				case 5:pow[i]="r-x";break;
				case 6:pow[i]="rw-";break;
				case 7:pow[i]="rwx";break;
				default:break;
			}
			mode=mode>>3;
		}

		strncpy(time,ctime(&statbuf.st_mtime)+4,12);
		printf("%c %s%s%s %2d %s %s %6d %s %s\n",
			type,pow[2],pow[1],pow[0],
			(int)statbuf.st_nlink, 
			getpwuid(statbuf.st_uid)->pw_name, 
			getgrgid(statbuf.st_gid)->gr_name,
			(int)statbuf.st_size,time,
			dpInfo->d_name);
	}
}

int main(int argc,char* argv[]){
	char *dir;
	if(argc<2)
		dir=".";
	else
		dir=argv[1];
	printstat(dir);
	printf("\n");
	return 0;
}
