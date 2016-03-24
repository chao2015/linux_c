#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*create child and parent process,
create a defunct and solve it,
get exitcode.
fork(),sleep(),wait(),WIFEXITED,WEXITSTATUS*/

int main(){
	//pid_t pid;
	//pid=fork();
	//if(pid==0){
	if(!fork()){
		exit(10);
	}else{
		int ExitCode;
		wait(&ExitCode);
		if(WIFEXITED(ExitCode)){
			printf("child process exited with code %d\n",
				WEXITSTATUS(ExitCode));
		}
		sleep(10);
	}
	return 0;
}
