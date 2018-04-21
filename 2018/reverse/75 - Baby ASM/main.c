#include <stdio.h>
#include <stdlib.h>

#define F(x) (x - 0x2A)

char flag[] = {
	F('b'), F('z'), F('h'), F('c'), F('t'), F('f'), F('{'), F('S'), F('t'),
	F('a'), F('t'), F('i'), F('c'), F('_'), F('a'), F('n'), F('a'), F('l'),
	F('y'), F('s'), F('i'), F('s'), F('_'), F('F'), F('T'), F('W'), F('}'),
};

int checkFlag(char *input)
{
	size_t i;

	for(i = 0; i < sizeof(flag); i++)
		if(input[i] != 42 + flag[i])
			return 0;

	return 1;
}

int main(int argc, char *argv[])
{
	if(checkFlag(argv[1]))
		puts("ok");
	else
		puts("ko");

