#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>

#ifdef __linux
	#include <sys/syscall.h>
	#include <sys/ptrace.h>
#endif

#define SERIAL_LENGTH 0x100
#define BUFFER_LENGTH 0x100

long g_ptrace;
char g_serial[SERIAL_LENGTH];

char g_print[BUFFER_LENGTH];
int g_print_index = 0;

void
real_print(unsigned char index)
{
	static char charset[0x100];
	int i;

	if(!charset[0])
		for(i = 0; i < sizeof(charset); i++)
			charset[i ^ 0xFF] = i ^ 0x17BD6955;
	
	g_print[g_print_index++] = charset[index];
}

void
real_print_flush(void)
{
#ifdef __linux
	syscall(SYS_write, 2, g_print, g_print_index);
#else
	write(2, g_print, g_print_index);
#endif
	
	while(g_print_index)
		g_print[g_print_index--] = 0;
}

void
real_badBoy(void)
{
	real_print('Y' ^ 0xAA);
	real_print('o' ^ 0xAA);
	real_print('u' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('p' ^ 0xAA);
	real_print('r' ^ 0xAA);
	real_print('o' ^ 0xAA);
	real_print('b' ^ 0xAA);
	real_print('a' ^ 0xAA);
	real_print('b' ^ 0xAA);
	real_print('l' ^ 0xAA);
	real_print('y' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('m' ^ 0xAA);
	real_print('i' ^ 0xAA);
	real_print('s' ^ 0xAA);
	real_print('u' ^ 0xAA);
	real_print('n' ^ 0xAA);
	real_print('d' ^ 0xAA);
	real_print('e' ^ 0xAA);
	real_print('r' ^ 0xAA);
	real_print('s' ^ 0xAA);
	real_print('t' ^ 0xAA);
	real_print('o' ^ 0xAA);
	real_print('o' ^ 0xAA);
	real_print('d' ^ 0xAA);
	real_print(':' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('I' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('a' ^ 0xAA);
	real_print('m' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('n' ^ 0xAA);
	real_print('o' ^ 0xAA);
	real_print('t' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('l' ^ 0xAA);
	real_print('o' ^ 0xAA);
	real_print('o' ^ 0xAA);
	real_print('k' ^ 0xAA);
	real_print('i' ^ 0xAA);
	real_print('n' ^ 0xAA);
	real_print('g' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('f' ^ 0xAA);
	real_print('o' ^ 0xAA);
	real_print('r' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('a' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('w' ^ 0xAA);
	real_print('r' ^ 0xAA);
	real_print('o' ^ 0xAA);
	real_print('n' ^ 0xAA);
	real_print('g' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('s' ^ 0xAA);
	real_print('e' ^ 0xAA);
	real_print('r' ^ 0xAA);
	real_print('i' ^ 0xAA);
	real_print('a' ^ 0xAA);
	real_print('l' ^ 0xAA);
	real_print('.' ^ 0xAA);
	real_print('.' ^ 0xAA);
	real_print('.' ^ 0xAA);
	real_print('\n' ^ 0xAA);

	real_print('T' ^ 0xAA);
	real_print('r' ^ 0xAA);
	real_print('y' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('a' ^ 0xAA);
	real_print('g' ^ 0xAA);
	real_print('a' ^ 0xAA);
	real_print('i' ^ 0xAA);
	real_print('n' ^ 0xAA);
	real_print(',' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('n' ^ 0xAA);
	real_print('o' ^ 0xAA);
	real_print('o' ^ 0xAA);
	real_print('b' ^ 0xAA);
	real_print('!' ^ 0xAA);
	real_print('\n' ^ 0xAA);
	real_print_flush();
}

void
real_goodBoy(void)
{
	real_print('C' ^ 0xAA);
	real_print('o' ^ 0xAA);
	real_print('n' ^ 0xAA);
	real_print('g' ^ 0xAA);
	real_print('r' ^ 0xAA);
	real_print('a' ^ 0xAA);
	real_print('t' ^ 0xAA);
	real_print('u' ^ 0xAA);
	real_print('l' ^ 0xAA);
	real_print('a' ^ 0xAA);
	real_print('t' ^ 0xAA);
	real_print('i' ^ 0xAA);
	real_print('o' ^ 0xAA);
	real_print('n' ^ 0xAA);
	real_print('s' ^ 0xAA);
	real_print('!' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('T' ^ 0xAA);
	real_print('h' ^ 0xAA);
	real_print('e' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('f' ^ 0xAA);
	real_print('l' ^ 0xAA);
	real_print('a' ^ 0xAA);
	real_print('g' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('i' ^ 0xAA);
	real_print('s' ^ 0xAA);

	real_print(' ' ^ 0xAA);

	real_print('"' ^ 0xAA);
	real_print('B' ^ 0xAA);
	real_print('R' ^ 0xAA);
	real_print('E' ^ 0xAA);
	real_print('I' ^ 0xAA);
	real_print('Z' ^ 0xAA);
	real_print('H' ^ 0xAA);
	real_print('C' ^ 0xAA);
	real_print('T' ^ 0xAA);
	real_print('F' ^ 0xAA);
	real_print('{' ^ 0xAA);
	real_print('"' ^ 0xAA);

	real_print(' ' ^ 0xAA);
	real_print('+' ^ 0xAA);
	real_print(' ' ^ 0xAA);

	// real_print('M' ^ 0xAA);
	// real_print('D' ^ 0xAA);
	// real_print('5' ^ 0xAA);
	// real_print('(' ^ 0xAA);
	real_print('s' ^ 0xAA);
	real_print('e' ^ 0xAA);
	real_print('r' ^ 0xAA);
	real_print('i' ^ 0xAA);
	real_print('a' ^ 0xAA);
	real_print('l' ^ 0xAA);
	// real_print(')' ^ 0xAA);

	real_print(' ' ^ 0xAA);
	real_print('+' ^ 0xAA);
	real_print(' ' ^ 0xAA);

	real_print('"' ^ 0xAA);
	real_print('}' ^ 0xAA);
	real_print('"' ^ 0xAA);

	real_print('\n' ^ 0xAA);
	real_print_flush();
}

void
real_check(void)
{
	size_t size;
	int i;

	int a = 0;
	int b = 0;
	int c = 0;
	int x = 0;

	for(size = 0; g_serial[size]; size++);

	if(size != 19) goto fail;

	// 1234-1234-1234-1234
	if(g_serial[4]  != '-') goto fail;
	if(g_serial[9]  != '-') goto fail;
	if(g_serial[14] != '-') goto fail;

	for(i = 0; i < 4; i++)
		if((unsigned)(g_serial[i] - '0') > 9)
			goto fail;
		else
			a = a * 10 + g_serial[i] - '0';

	for(i = 0; i < 4; i++)
		if((unsigned)(g_serial[i + 5] - '0') > 9)
			goto fail;
		else
			b = b * 10 + g_serial[i + 5] - '0';

	for(i = 0; i < 4; i++)
		if((unsigned)(g_serial[i + 10] - '0') > 9)
			goto fail;
		else
			c = c * 10 + g_serial[i + 10] - '0';

	for(i = 0; i < 4; i++)
		if((unsigned)(g_serial[i + 15] - '0') > 9)
			goto fail;
		else
			x = x * 10 + g_serial[i + 15] - '0';

	/*
	b² - 4ac == 0 => résultat = -b / 2a

	d = -b / 2a <=> b = -2a * d

	a =
	b = -2 * a * d
	c = ?

	b² - 4ac = 0 <=> (-2 * a * d)² - 4ac = 0
				 <=> 4 * a² * d² - 4ac = 0
				 <=> 4a ( a * d² - c) = 0
				 <=> 4a == 0 (pas drôle) ou...
				 <=> a * d² - c = 0
				 <=> a * d² = c

	d = ?
	a = ?
	b = -2 * a * d
	c = a * d²
	*/

	/*
	k = 3
	a = 1017
	b = 2 * a * k
	c = a * k**2

	Serial: 1017-6102-9153-0003
	Flag: BREIZHCTF{1017-6102-9153-0003}
	*/

	// 0x3F93 == 1017 + 6102 + 9153 + 3
	if(a + b + c + x != 0x3F93 ^ g_ptrace)
		goto fail;

	// ax^2 - bx + c = 0
	if(c - (b - (x * a)) * x == 0)
		real_goodBoy();
	else fail:
		real_badBoy();

}

void
fake_goodBoy(void)
{
	printf("Congratulations!");
	printf("Flag is: BREIZHCTF{STRiNGS_iS_FoR_NooBS}");
}


void
fake_badBoy(void)
{
	printf("You probably misunderstood: I am not looking for a wrong serial...\n");
	printf("Try again, noob!");
}

void
fake_check(void)
{
	if(strcmp(g_serial, "hunter2"))
		fake_badBoy();
	else
		fake_goodBoy();
}

int
main(int argc, char* argv[], char* envp[])
{
	printf("Serial: ");
	g_serial[read(2, g_serial, sizeof(g_serial)) - 1] = 0;
	fake_check();

	return EXIT_SUCCESS;
}

/**
 * This function is used to trick reversers.
 * It is run *BEFORE* main, and it will close stdout.
 * Therefore, reversers will think printf work, while it does not.
 */
void __attribute__((constructor))
constructor(void)
{
#ifdef __linux
	g_ptrace = syscall(SYS_ptrace, PTRACE_TRACEME);
#else
	g_ptrace = 0;
#endif

	real_print('S' ^ 0xAA);
	real_print('e' ^ 0xAA);
	real_print('r' ^ 0xAA);
	real_print('i' ^ 0xAA);
	real_print('a' ^ 0xAA);
	real_print('l' ^ 0xAA);
	real_print(':' ^ 0xAA);
	real_print(' ' ^ 0xAA);
	real_print_flush();
	
	close(1);
	atexit(real_check);
}
