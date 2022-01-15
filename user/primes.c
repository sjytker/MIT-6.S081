#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void calprimes(int *v, int count) {
	if (count == 0) return;
	int p[2];
	pipe(p);
	int prime = *v;
	int t;
	printf("%d\n", prime);
	
	if (fork() == 0) {
		close(p[0]);
		for (int i = 0; i < count; i ++) {
			write(p[1], &v[i], 1);
		}
		close(p[1]);
		exit(0);
	} else {
		close(p[1]);
		count = 0;
		while (read(p[0], &t, 1) != 0) {
			if (t % prime) {
				*v++ = t;
				count ++;
			}
		}
		close(p[0]);
		wait(0);
		calprimes(v - count, count);
	}
}

int
main(int argc, char *argv[]) {
	int v[34];
	for (int i = 0; i < 34; i ++) {
		v[i] = i + 2;
	}
	calprimes(v, 34);
	exit(0);
}
