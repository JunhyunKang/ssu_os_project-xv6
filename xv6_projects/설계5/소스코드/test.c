#include "types.h"
#include "param.h"
#include "stat.h"
#include "mmu.h"
#include "proc.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"
#include "fcntl.h"
#include "user.h"

#define BUFMAX 1024

void write_to_norm(char* fname);

int main()
{
	int fd;
	int i, size;
	char buf[BUFMAX];
	char* fname = "test_cs";

	if ((fd = open(fname, O_CREATE | O_CS | O_RDWR)) <= 0) {
		printf(2, "ERROR: open failed in cs file\n");
		exit();
	}

	for (i = 0; i < BUFMAX; i++)
		buf[i] = 'a';

	for (i = 0; i < 130; i++) {
		if ((size = write(fd, buf, BUFMAX)) != BUFMAX) {
			printf(2, "ERROR: write failed in cs file\n");
			exit();
		}

		/*	
		if (i == 50)
			write_to_norm("test_norm");	
		*/
	}

	printinfo(fd, fname);
	
	close(fd);
	exit();
}

void write_to_norm(char* fname)
{
	int fd;
	int i, size;
	char buf[BUFMAX];

	if ((fd = open(fname, O_CREATE | O_RDWR)) <= 0) {
		printf(2, "ERROR: open failed in normal file\n");
		exit();
	}

	for (i = 0; i < BUFMAX; i++)
		buf[i] = 'b';

	for (i = 0; i < 2; i++) {
		if ((size = write(fd, buf, BUFMAX)) != BUFMAX) {
			printf(2, "ERROR: write failed in normal file\n");
			exit();
		}
	}

	printinfo(fd, fname);
	
	close(fd);
}
