#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/param.h"


int main(int argc, char *argv[]) {

	int count = 0, k = 0;
	int m = 0;
	char* lines[MAXARG];
	char *p;
	char block[32], buf[32];

	for (int i = 1; i < argc; i ++) {
		lines[count++] = argv[i];
	}

	p = buf;
	while ((k = read(0, block, sizeof(block))) > 0) {
		for (int i = 0; i < k; i ++) {
			if (block[i] == '\n') {
				lines[count++] = p;
				lines[count] = 0;
				p = buf;
				m = 0;
				count = argc - 1;
				if (fork() == 0) {
					exec(argv[1], lines);
				}	
			} else if (block[i] == ' ') {
				buf[m++] = 0;
				lines[count++] = p;
				p = buf + m;
			} else {
				buf[m++] = block[i];	
			}
		}
	}
	exit(0);
}
