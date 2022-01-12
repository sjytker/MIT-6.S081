#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"



char* fmtname(char *path){
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--);
  p++;
  memmove(buf, p, strlen(p)+1);
  return buf;
}


void
find(char *path, char *pattern) {
	struct stat st;
	struct dirent de;
	int fd;

	// printf("path : %s\n", path);
	// printf("pattern: %s\n", pattern);

	if ((fd = open(path, O_RDONLY)) < 0) {
		return;
	}
	
	if (fstat(fd, &st) < 0) {
		printf("stating path %s error\n", path);
		close(fd);
		return;
	}
	
	char buf[512], *p;
	switch(st.type){
		case T_FILE:
			// printf("fmtname :%s,%d\n", fmtname(path), strcmp(fmtname(path), pattern) == 0);
			if (strcmp(fmtname(path), pattern) == 0) {
				printf("%s\n", path);
			}	
			break;
		case T_DIR:	
			if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
				printf("ls: path too long\n");
				break;
			}	
			strcpy(buf, path);
			p = buf+strlen(buf);
			*p++ = '/';
			while(read(fd, &de, sizeof(de)) == sizeof(de)){
			  if(de.inum == 0 || de.inum == 1 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
				continue;
			  memmove(p, de.name, strlen(de.name));
			  p[strlen(de.name)] = 0;
			  find(buf, pattern);
			}
			break;
	}
	close(fd);
}


int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("usage : find <dir> <pattern>\n");
		exit(0);
	}
	
	char *path = argv[1];
	char *pattern = argv[2];
	find(path, pattern);
	exit(0);
}
