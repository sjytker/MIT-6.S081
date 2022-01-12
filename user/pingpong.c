#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
// #include "kernel/proc.h"


char buf[5];

void ping(int *parent, int *child) {
    if (write(child[1], "ping", 4) < 0) {
        printf("write error\n");
    }    
    if (read(parent[0], &buf, 4) < 0) {
        printf("read error\n");
    } 
    printf("receive %s\n", buf);
}

void pong(int *parent, int *child) {
    if (read(child[0], &buf, 4) < 0) {
        printf("read error\n");
    } 
    printf("receive %s\n", buf);
    if (write(parent[1], "pong", 4) < 0) {
        printf("write error\n");
    }    
}

int
main(int argc, char *argv[]) {
   int parent[2], child[2];
   if (pipe(parent) == -1 || pipe(child) == -1) {
        printf("pipe error\n");
   }

   int pid;
   if ((pid = fork()) < 0) {
       printf("fork error\n");
   }
   
   if (pid > 0) {
    ping(parent, child);
   } else {
    pong(parent, child);
   }
   exit(0);
}
