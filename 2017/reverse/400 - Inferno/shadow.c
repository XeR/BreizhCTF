#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>

#include "flag.h"

/* .data */
extern void *data_start;
extern void *_edata;

/* Obfuscating/cleaning functions: */
static void strOpFactor(char *str, size_t size, size_t index)
{
	if(size) {
		*str ^= str[size - index];
		return strOpFactor(str + 1, size - 1, index);
	}
}

inline static void xorStr(char *str, size_t size)
{
	strOpFactor(str, size, 1);
}

inline static void memClear(char *str, size_t size)
{
	strOpFactor(str, size, size);
}

/** This function clears register before segfaulting */
/** Might need adjustments if you're an ATT fan */
#define CLEAR_REG(reg) __asm__("xor " reg ", " reg)
void fuckup(void);
__asm__("fuckup:");
	CLEAR_REG("rax");
	CLEAR_REG("rbx");
	CLEAR_REG("rcx");
	CLEAR_REG("rdx");
	CLEAR_REG("rdi");
	CLEAR_REG("rsi");
	CLEAR_REG("rbp");
	CLEAR_REG("rsp");
	CLEAR_REG("r8");
	CLEAR_REG("r9");
	CLEAR_REG("r10");
	CLEAR_REG("r11");
	CLEAR_REG("r12");
	CLEAR_REG("r13");
	CLEAR_REG("r14");
	CLEAR_REG("r15");
	__asm__("jmp rax");

/** Redefining syscall so we don't get it in the imports */
/** (Dumped from libc) */
__asm__("syscall:");
	__asm__("mov    rax,rdi");
	__asm__("mov    rdi,rsi");
	__asm__("mov    rsi,rdx");
	__asm__("mov    rdx,rcx");
	__asm__("mov    r10,r8");
	__asm__("mov    r8,r9");
	__asm__("mov    r9,QWORD PTR [rsp+0x8]");
	__asm__("syscall");
	__asm__("ret");


/** This functions checks if our father is gdb, ptrace or radare2 */
#ifdef NDEBUG
int endsWith(char *str, size_t size, char *end, size_t size_e)
{
	if(size == 0 || size_e == 0)
		return 1;

	if(str[size] != end[size_e])
		return 0;

	return endsWith(str, size - 1, end, size_e - 1);
}
__attribute__((constructor))
static void checkFather(void)
{
	#define STRKEY 0x8F
	unsigned char str_procexe[] = {
		'/' ^ STRKEY, 'p' ^ STRKEY, 'r' ^ STRKEY, 'o' ^ STRKEY,
		'c' ^ STRKEY, '/' ^ STRKEY, '/' ^ STRKEY, '/' ^ STRKEY,
		'/' ^ STRKEY, '/' ^ STRKEY, '/' ^ STRKEY, '/' ^ STRKEY,
		'e' ^ STRKEY, 'x' ^ STRKEY, 'e' ^ STRKEY,
		STRKEY,
	};
	#undef STRKEY

	#define STRKEY 0x9F
	unsigned char str_gdb[] = {
		'g' ^ STRKEY, 'd' ^ STRKEY, 'b' ^ STRKEY,
		STRKEY,
	};
	#undef STRKEY

	#define STRKEY 0xAA
	unsigned char str_trace[] = {
		't' ^ STRKEY, 'r' ^ STRKEY, 'a' ^ STRKEY, 'c' ^ STRKEY,
		'e' ^ STRKEY,
		STRKEY,
	};
	#undef STRKEY

	#define STRKEY 0xFC
	unsigned char str_radare[] = {
		'r' ^ STRKEY, 'a' ^ STRKEY, 'd' ^ STRKEY, 'a' ^ STRKEY,
		'r' ^ STRKEY, 'e' ^ STRKEY, '2' ^ STRKEY,
		STRKEY,
	};
	#undef STRKEY

	pid_t   father;
	char    path[PATH_MAX];
	ssize_t size;
	int     panic;
	
	/* Determine father executable */
	father = syscall(SYS_getppid);
	xorStr(str_procexe, sizeof(str_procexe));

	/* Write pid in string */
	size = 10;
	while(father) {
		str_procexe[size--] = '0' + (father % 10);
		father /= 10;
	}

	size = syscall(SYS_readlink, str_procexe, path, sizeof(path));
	memClear(str_procexe, sizeof(str_procexe));

	if(size <= 0)
		fuckup();

	panic = 0;

	/* Check if father is *trace, gdb or radare */
	xorStr(str_trace,  sizeof(str_trace));
	xorStr(str_gdb,    sizeof(str_gdb));
	xorStr(str_radare, sizeof(str_radare));

	panic |= endsWith(path, size, str_trace,  sizeof(str_trace)  - 1);
	panic |= endsWith(path, size, str_gdb,    sizeof(str_gdb)    - 1);
	panic |= endsWith(path, size, str_radare, sizeof(str_radare) - 1);

	memClear(str_trace,  sizeof(str_trace));
	memClear(str_gdb,    sizeof(str_gdb));
	memClear(str_radare, sizeof(str_radare));
	memClear(path,       sizeof(path));

	if(panic)
		fuckup();
}
#endif

/** This function kills unknown debuggers (ptrace) */
#ifdef NDEBUG
__attribute__((constructor))
static void noptrace(void)
{
	if(syscall(SYS_ptrace, PTRACE_TRACEME))
		fuckup();
}
#endif

/** This function transforms the flag to piss off the static guys */
/** This function used to use /proc/self/mem, but it fails on my grsec */
__attribute__((constructor))
static void changeFlag(void)
{
	int      fd;
	unsigned char  buffer[sizeof(flag)];
	unsigned char* seek;

	/* Find flag without giving away the xref */
	seek = (char*)&_edata;

	/* Last char of motd */
	while(*seek != 'm')  seek--;

	/* Before flag */
	while(*seek != 0x00) seek--;

	/* First char of the flag */
	seek++;

	/* Change it */
	for(int i = 0; i < sizeof(buffer); i++)
		seek[i] = seek[i] ^ (i * 17 + 5);
}
