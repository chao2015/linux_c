#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

//compliment the function of printing multiple directory.

void printdir(char *dir,int depth){
	DIR *pd=opendir(dir);
	if(NULL==pd){
		fprintf(stderr,"cannot open dir:%s\n",dir);
		return;
	}
	struct dirent *pdInfo;
	struct stat statbuf;
	chdir(dir);
	while((pdInfo=readdir(pd))!=NULL){
		stat(pdInfo->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)){
			if(strcmp(".",pdInfo->d_name)==0||
				(strcmp("..",pdInfo->d_name)==0))
				continue;
			printf("%*s%s/\n",depth," ",pdInfo->d_name);
			printdir(pdInfo->d_name,depth+4);
		}
		else
			printf("%*s%s\n",depth," ",pdInfo->d_name);
	}
	chdir("..");
	closedir(pd);
}

int main(int argc,char* argv[]){
	char *topdir,pwd[2]=".";
	if(argc<2)
		topdir=pwd;
	else
		topdir=argv[1];
	printf("Directory scan of %s\n",topdir);
	printdir(topdir,0);
	printf("\ndone.\n");
	return 0;
}
