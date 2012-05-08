/**
 * In order to find which call is used in an ELF file you can
 * use nm(1); for example to look inside cat(1) and find out
 * which one of the open's family is used you can do this
 *
 *  $ nm -D `which cat` | grep open
 *       U fdopen
 *       U iconv_open
 *       U open64
 */
#define _GNU_SOURCE// in order to use RTLD_NEXT

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<dlfcn.h>
#include<stdarg.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/ip.h>

// http://stackoverflow.com/questions/880263/how-can-i-detect-file-accesses-in-linux

#if 0
int gethostname(char *name, size_t len) {
	fprintf(stderr, "[gethostname(%d)]\n", len);
	char* fakename = strdup("pippo");

	strcpy(name, fakename);

	return 0;
}
#endif
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	int (*connect_handler)(int, const struct sockaddr*, socklen_t);

	struct sockaddr_in* saddr = (struct sockaddr_in*)addr;

	int size = 156;
	char dest[size];
	unsigned short port = ntohs(saddr->sin_port);

	inet_ntop(AF_INET, &(saddr)->sin_addr.s_addr, dest, size);

	fprintf(stderr, "[connect to %s:%u]\n", dest, port);

	connect_handler = dlsym(RTLD_NEXT, "connect");

	return connect_handler(sockfd, addr, addrlen);
}

FILE *fopen64(const char *path, const char *mode) {
	fprintf(stderr, "[opened file: '%s']\n", path);

	FILE* (*fopen_handler)(const char* path, const char* mode);

	fopen_handler = dlsym(RTLD_NEXT, "fopen64");

	return fopen_handler(path, mode);
}

FILE *fopen(const char *path, const char *mode) {
	fprintf(stderr, "[opened file: '%s']\n", path);

	FILE* (*fopen_handler)(const char* path, const char* mode);

	fopen_handler = dlsym(RTLD_NEXT, "fopen");

	return fopen_handler(path, mode);
}

// https://bbs.archlinux.org/viewtopic.php?id=96790
int open(const char *pathname, int flags, ...) {
	static int (*open_handler)(const char*, int, ...);

	fprintf(stderr, "[opened file: '%s']\n", pathname);

	// load the real open call
	open_handler = dlsym(RTLD_NEXT, "open");

	mode_t mode = 0;

	va_list arg;
	if ( flags & O_CREAT ) {
		va_start(arg, flags);
		mode = va_arg(arg, mode_t);
		va_end(arg);
	}


	return open_handler(pathname, flags, mode);
}

int open64(const char *pathname, int flags, ...) {
	static int (*open_handler)(const char*, int, ...);

	fprintf(stderr, "[opened file: '%s']\n", pathname);

	// load the real open call
	open_handler = dlsym(RTLD_NEXT, "open64");

	mode_t mode = 0;

	va_list arg;
	if ( flags & O_CREAT ) {
		va_start(arg, flags);
		mode = va_arg(arg, mode_t);
		va_end(arg);
	}


	return open_handler(pathname, flags, mode);
}
