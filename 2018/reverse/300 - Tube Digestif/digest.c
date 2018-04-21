/*
strip -N pbkdf2 -N forkMe -N checkFlag -N checkInside -N key -N flag a.out
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

#define  ITERATIONS 1024
#define  FLAG_START "bzhctf{"
#define  FLAG_END   "}"

/**
 * dd if=/dev/urandom bs=20 count=1 | xxd -i
 *1+0 records in
 *1+0 records out
 *20 bytes copied, 5,9811e-05 s, 334 kB/s
 *  0x88, 0xb6, 0x67, 0x73, 0x2c, 0x83, 0x76, 0x76, 0xd7, 0x7f, 0x01, 0xea,
 *  0x53, 0x47, 0xc3, 0x72, 0x37, 0xab, 0x0d, 0x2c
 */

static const char key[] = {
	0x88, 0xb6, 0x67, 0x73, 0x2c, 0x83, 0x76, 0x76,
	0xd7, 0x7f, 0x01, 0xea,	0x53, 0x47, 0xc3, 0x72,
	0x00, 0x00, 0x00, 0x01
};

/**
 * % sort -R /usr/share/dict/rockyou | head
 * kingt2
 * justinconard
 * 9811573
 * 644260
 * grassgrows247
 * 2630540
 * ironponi159
 * h66353
 * specsdonegan
 * bakation
 *
 * « grassgrows247 » lgtm
 */

static const char flag[] = {
	0xa5, 0x55, 0x15, 0xe0, 0xf3, 0x54, 0xdb, 0x4d,
	0x72, 0x5c, 0x74, 0xc1, 0x6a, 0x52, 0x78, 0xef,
	0x40, 0x15, 0x2a, 0xa9,
};

pid_t forkMe(int fd[2])
{
	pid_t pid;
	int   rx[2];
	int   tx[2];
	int   b;

#ifdef NDEBUG
	pipe(rx);
	pipe(tx);
	pid = fork();
#else
	if(0 != pipe(rx)) {
		perror("pipe(rx)");
		return -1;
	}

	if(0 != pipe(tx)) {
		perror("pipe(tx)");
		return -2;
	}


	if(0 > (pid = fork())) {
		perror("fork");
		return -3;
	}
#endif

#ifdef NDEBUG
	b = pid == 0;

	close(rx[!b]);
	close(tx[b]);

	fd[!b] = tx[!b];
	fd[b]  = rx[b];
#else
	if(0 < pid) {
		close(rx[1]);
		close(tx[0]);

		fd[0] = rx[0];
		fd[1] = tx[1];
	} else {
		close(rx[0]);
		close(tx[1]);

		fd[0] = tx[0];
		fd[1] = rx[1];
	}
#endif

	return pid;
}

int fdMove(int from, int to)
{
	if(from == to)
		return 0;

	if(0 > dup2(from, to))
		return -1;

	if(0 != close(from))
		return -2;
}

int pbkdf2(char *key, size_t keyLength, size_t depth)
{
	char   salt[SHA_DIGEST_LENGTH];
	char   output[SHA_DIGEST_LENGTH];
	int    len;
	size_t i;

	bzero(output, sizeof(output));
	len = sizeof(output);

	/* Retrieve salt */
	if(SHA_DIGEST_LENGTH != read(fileno(stdin), salt, sizeof(salt))) {
		perror("read");
		return -1;
	}

	HMAC(EVP_sha1(), key, keyLength, salt, sizeof(salt), salt, &len);

	/* Call recursively */
	if(depth > 1) {
		int   fd[2];
		pid_t pid;

		if(0 > (pid = forkMe(fd)))
			return -2;

		if(0 == pid) {
			fdMove(fd[0], fileno(stdin));
			fdMove(fd[1], fileno(stdout));

			return pbkdf2(key, keyLength, depth - 1);
		}

		write(fd[1], salt, sizeof(salt));
		read(fd[0], output, sizeof(output));
	}


	for(i = 0; i < sizeof(output); i++)
		output[i] ^= salt[i];

	write(fileno(stdout), output, sizeof(output));

	return 0;
}

int checkInside(char *input, size_t length)
{
	pid_t  pid;
	int    fd[2];

	char   buffer[SHA_DIGEST_LENGTH];
	size_t i;

	if(0 > (pid = forkMe(fd)))
		return -1;

	if(0 == pid) {
		fdMove(fd[0], fileno(stdin));
		fdMove(fd[1], fileno(stdout));
		return pbkdf2(input, length, ITERATIONS);
	}

	write(fd[1], key, sizeof(key));
	read(fd[0], buffer, sizeof(buffer));

#ifndef NDEBUG
	for(i = 0; i < sizeof(buffer); i++)
		printf("%02X ", (unsigned char)buffer[i]);
	puts("");

	for(i = 0; i < sizeof(buffer); i++)
		printf("%02X ", (unsigned char)flag[i]);
	puts("");
#endif

	return memcmp(flag, buffer, sizeof(flag));
}


int checkFlag(char *input, size_t length)
{
	size_t start = sizeof(FLAG_START) - 1;
	size_t end   = length - (sizeof(FLAG_END) - 1);

	if(0 != strncmp(input, FLAG_START, start))
		return -1;

	if(0 != strcmp(input + end, FLAG_END))
		return -2;

	return checkInside(input + start, end - start);
}

int main(int argc, char *argv[])
{
	if(argc == 1) {
		fprintf(stderr, "Usage: %s [password]\n", argv[0]);
		return EXIT_FAILURE;
	}

	if(argc > 2) {
		fprintf(stderr, "Ma parole, mais il est con ce môme !\n");
		fprintf(stderr, "Allez mange ton coredump pour la peine.\n");
		abort();
	}

	if(0 == checkFlag(argv[1], strlen(argv[1]))) {
		printf("Flag is correct!\n");
		return EXIT_SUCCESS;
	} else {
		printf("Flag is wrong...\n");
		return EXIT_FAILURE;
	}
}
