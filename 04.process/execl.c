#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(){
	execl("./add","add","3","4",NULL);
	printf("we can't see the following content?\n");
	return 0;
}

/*add.c:

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
    int a=atoi(argv[1]);
	int b=atoi(argv[2]);
	printf("%d + %d = %d\n",a,b,a+b);

	return 0;
}

gcc add.c -o add*/
