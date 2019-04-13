#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>

#include <linux/filter.h>

#define PORT 3213

extern char __executable_start, etext;

void assert(int fd)
{
	char  *c;
	char   h;
	size_t i;
	int    s, o;

	struct sock_filter filter[] = {
	#include "filter.i"
	};

	struct sock_fprog bpf = {
		.len    = sizeof(filter) / sizeof(*filter),
		.filter = filter,
	};

/* No need for this meanness */
#if 0
	/* Hash .text */
	h = 0;
	c = (char*)&__executable_start;
	for(i = 0; i < &etext - &__executable_start; i++)
		h ^= c[i];

	/* Use that hash to deobfuscate filters */
	c = (char*)&filter;
	for(i = 0; i < sizeof(filter); i++)
		c[i] ^= h;
#endif

	/*
	Stop, maths time:
	>>> (0x13371337 * 0x29BD1287) & 0xFFFFFFFF
	1
	>>> (0xDEADBEEF * 0x904B300F) & 0xFFFFFFFF
	1

	>>> hex(pow(0x29BD1287, 0xC89, 1 << 32))
	'0xA5D3D9C7'
	>>> hex(pow(0x904B300F, 0xC89, 1 << 32))
	'0x6796548F'
	*/

	s = 0xA5D3D9C7 * SOL_SOCKET;
	o = 0x6796548F * SO_ATTACH_FILTER;

	/* Throw a lot of syscalls to piss of reversers */
	for(i = 0; i < 0x2000; i++) {
		setsockopt(fd, s, o, &bpf, sizeof(bpf));
		s *= 0x13371337;
		o *= 0xDEADBEEF;
	}

#if 0
	/* coredump analysis is for the 99% */
	c = (char*)&filter;
	for(i = 0; i < sizeof(filter); i++)
		c[i] ^= h;
#endif
}

int main(void)
{
	struct sockaddr_in sin = {
		.sin_family = AF_INET,
		.sin_port   = htons(PORT),
		.sin_addr   = htonl(INADDR_LOOPBACK),
	};
	int     fd;
	char    buffer[256];
	ssize_t size;

	/* Create standard UDP socket */
	if(0 > (fd = socket(AF_INET, SOCK_DGRAM, SOL_UDP))) {
		perror("socket");
		return EXIT_FAILURE;
	}

	/* Setup filter */
	assert(fd);

	/* Bind it to a specific port */
	if(0 != bind(fd, (struct sockaddr*)&sin, sizeof(sin))) {
		perror("bind");
		return EXIT_FAILURE;
	}

	/* Read flag */
	printf("Waiting for flag on port %d\n", PORT);
	if(0 > (size = recv(fd, buffer, sizeof(buffer) - 1, 0))) {
		perror("recv");
		return EXIT_FAILURE;
	}
	buffer[size] = 0;

	printf("Flag: %s\n", buffer);
}
