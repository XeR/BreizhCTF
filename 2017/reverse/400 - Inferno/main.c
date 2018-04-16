#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flag.h"

#define  FLAG_START "bzhctf{"
#define  FLAG_END   "}"


unsigned char motd[] = "\e[31m"
	" ██▓ ███▄    █   █████▒▓█████  ██▀███   ███▄    █  ▒█████  \n"
	"▓██▒ ██ ▀█   █ ▓██   ▒ ▓█   ▀ ▓██ ▒ ██▒ ██ ▀█   █ ▒██▒  ██▒\n"
	"▒██▒▓██  ▀█ ██▒▒████ ░ ▒███   ▓██ ░▄█ ▒▓██  ▀█ ██▒▒██░  ██▒\n"
	"░██░▓██▒  ▐▌██▒░▓█▒  ░ ▒▓█  ▄ ▒██▀▀█▄  ▓██▒  ▐▌██▒▒██   ██░\n"
	"░██░▒██░   ▓██░░▒█░    ░▒████▒░██▓ ▒██▒▒██░   ▓██░░ ████▓▒░\n"
	"░▓  ░ ▒░   ▒ ▒  ▒ ░    ░░ ▒░ ░░ ▒▓ ░▒▓░░ ▒░   ▒ ▒ ░ ▒░▒░▒░ \n"
	" ▒ ░░ ░░   ░ ▒░ ░       ░ ░  ░  ░▒ ░ ▒░░ ░░   ░ ▒░  ░ ▒ ▒░ \n"
	" ▒ ░   ░   ░ ░  ░ ░       ░     ░░   ░    ░   ░ ░ ░ ░ ░ ▒  \n"
	" ░           ░            ░  ░   ░              ░     ░ ░  \n"
	"                                                           \n"
	"           Abandon hope all ye who enters here.            \n"
	"                                                           \n"
	"\e[0m";

/* Not the actual flag, sorry */
unsigned char flag[32] = {
	0xE2, 0x94, 0xF4, 0x9A, 0x91, 0x68, 0x3B, 0x1D,
	0xCC, 0xA7, 0x01, 0x93, 0x84, 0xF3, 0x4A, 0xEE,
	0x14, 0xBB, 0x2F, 0xEF, 0xBA, 0x0C, 0xFD, 0x4B,
	0x27, 0xc9, 0x53, 0x02, 0xD4, 0x4A, 0xE1, 0xB3,
};

int checkPass(char *test)
{
	static const char lut[] = "0123456789abcdef";
	char   ret = 0;

#ifdef FLAG_START
	/* Check start */
	if(strncmp(test, FLAG_START, sizeof(FLAG_START) - 1))
		return -1;
	test += sizeof(FLAG_START) - 1;
#endif

	/* Check actual flag */
	for(size_t i = 0; i < sizeof(flag); i++) {
		char a = flag[i] >> 4;
		char b = flag[i] & 0x0F;

		ret |= lut[a] ^ test[2 * i + 0];
		ret |= lut[b] ^ test[2 * i + 1];
	}
	test += 2 * sizeof(flag);

#ifdef FLAG_END
	/* Check end */
	if(strncmp(test, FLAG_END, sizeof(FLAG_END)))
		return -2;
#endif

	return ret;
}

int main(int argc, char *argv[])
{
	puts(motd);

	if(argc < 2) {
		fprintf(stderr, "Usage: %s <flag>\n", argv[0]);
		return EXIT_FAILURE;
	}


#ifndef NDEBUG
	printf("Flag: " FLAG_START);

	for(int i = 0; i < sizeof(flag); i++)
		printf("%02x", (unsigned)flag[i]);

	puts(FLAG_END);
#endif

	if(checkPass(argv[1])) {
		fprintf(stderr, "Error: wrong password\n");
		return EXIT_FAILURE;
	}

	puts("Congratulations!");
	printf("Flag: %s\n", argv[1]);
}
