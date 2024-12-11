#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];
int line;

void
cat(int fd)
{
	int n;

	int i, count=0;
	char c;

	while((n = read(fd, buf, sizeof(buf))) > 0) {
	
		for(i=0; i<n; i++){
			if((c=buf[i])=='\n')
				count++;
			
			if(count==line)
				break;
		}
 		if (write(1, buf, i+1) != i+1) {
			printf(1, "cat: write error\n");
			exit();
		}
		if(count==line)
			break;

	}
  
	if(n < 0){
    
		printf(1, "cat: read error\n");
    
		exit();
 
	}
}


int
main(int argc, char *argv[])
{
	int fd;
	if(argc <= 2){   
		cat(0);
		exit();
  
	}
	line=atoi(argv[1]);
	if((fd=open(argv[2], 0))<0){
		printf(1,"cat: cannot open %s\n", argv[2]);
		exit();
	}

	cat(fd);
	close(fd);
	exit();
}
