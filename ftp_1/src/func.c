#include "factory.h"

void cd_func(char* dir, char* sendfile){	
	FILE* fin;
	fin=fopen(sendfile,"w");
	//
	char buf[100]={0};
	char buf1[100]={0};
	sprintf(buf1,"%s/%s",getcwd(NULL,0),dir);
	int ret=chdir(buf1);	//包含了客户端仅输入"cd"的情况
	if(-1==ret){
		fprintf(fin,"Cannot Open:\"./%s\".Invalued Dir Address.\n",dir);
		fclose(fin);
		return;
	}
	fprintf(fin,"%s\n",getcwd(NULL,0));
	//
	fclose(fin);
}

void ls_func(char* dir, char* sendfile){
	if(dir[0]=='\0'){	//客户端仅输入"ls"的情况
		bzero(dir,sizeof(dir));
		strcpy(dir,".");
	}
	FILE* fin;
	fin=fopen(sendfile,"w");
	//
	DIR *dp=opendir(dir);
	if(NULL==dp){
		fprintf(fin,"Cannot Open:\"./%s\".Invalued Dir Address.\n",dir);
		fclose(fin);
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
		
		fprintf(fin,"%c %s%s%s %2d %s %s %6d %s %s\n",
			type,pow[2],pow[1],pow[0],
			(int)statbuf.st_nlink, 
			getpwuid(statbuf.st_uid)->pw_name, 
			getgrgid(statbuf.st_gid)->gr_name,
			(int)statbuf.st_size,time,
			dpInfo->d_name);
	}
	//
	fclose(fin);
}

void remove_func(char* buf){
	unlink(buf);
	printf("unlink %s\n",buf);
}

void pwd_func(char* sendfile){
	FILE* fin;
	fin=fopen(sendfile,"w");
	fprintf(fin,"%s\n",getcwd(NULL,0));
	fclose(fin);
}

void quit_func(char* buf){
	chdir(buf);
	printf("quit\n");
}

