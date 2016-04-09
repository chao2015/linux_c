#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
void scandir1(char* pathname,int width)
{
	DIR* dir;
	dir=opendir(pathname);
	struct dirent* df;
	char path[512];
	while( (df=readdir(dir))!=NULL)
	{
		if( !strcmp(df->d_name,".") || !strcmp(df->d_name,".."))	//not "." or ".."
		{
		}else{
			printf("%*s%s\n",width," ",df->d_name);
			if(df->d_type == 4)				//d_type==4 is dir, details: man 2 stat
			{
				bzero(path,sizeof(path));	
				sprintf(path,"%s%s%s",pathname,"/",df->d_name);
				scandir1(path,width+4);		//recursion
			}else{
			}
		}
	}
}

int main(int argc,char* argv[])
{
	if(argc !=2)
	{
		printf("error args\n");
		return -1;
	}
	printf("%s\n",argv[1]);
	scandir1(argv[1],0);
	return 0;
}	
