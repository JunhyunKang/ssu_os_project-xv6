#include "types.h"
#include "stat.h"
#include "user.h"


int main(int argc, char **argv)
{
	int i, mask;
	char *argv_2[100];

	if(argc<3){
		printf(1,"usage error ssu_trace\n");
		exit();
	}

	mask=atoi(argv[1]);


	for(i=0 ; i<argc; i++){
		argv_2[i]=malloc(100);
		argv_2[i]=argv[i+2];
	}

	trace(mask);
	exec(argv[2], argv_2);

	exit();
}
