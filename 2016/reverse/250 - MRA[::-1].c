#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* flag: BZHCTF{ImSureYouDidNotUseObjdump} */

static const char *key[] = {
	"DontForgetFlagFormat",                         /* 0  */
	"BZHCTF{FlagGoesHere}",                         /* 1  */
	"IWonderHowManyWillTryThisFlag",                /* 2  */
	"WhatDidYouExpect",                             /* 3  */
	"StringsFlagRaspberryPi",                       /* 4  */
	"NobodyUsesStrings",                            /* 5  */
	"RealMenUseObjdump",                            /* 6  */
	"GogoGadgetoGDB",                               /* 7  */
	"TheRadareCommunityIsSuceptible",               /* 8  */
	"ThatIDALicenseCostsMoreThanMyCar",             /* 9  */
	"ThatKindOfDebateIsPointless",                  /* 10 */
	"JustLikeEmacsVersusVi",                        /* 11 */
	"EverybodyKnowThatViIsBetter",                  /* 12 */
	"VEhBTksgWU9VIE1BUklPIQ==",                     /* 13 */
	"QlVUIE9VVCBGTEFHIElTIElOIEFOT1RIRVIgQ0FTVExF", /* 14 */
	"SSBwcmVmZXIgVExvWiBhbnl3YXk=",                 /* 15 */
};

static char flag[] = {
	'I' ^ 'R',
	'm' ^ 'e',
	'S' ^ 'a',
	'u' ^ 'l',
	'r' ^ 'M',
	'e' ^ 'e',
	'Y' ^ 'n',
	'o' ^ 'U',
	'u' ^ 's',
	'D' ^ 'e',
	'i' ^ 'O',
	'd' ^ 'b',
	'N' ^ 'j',
	'o' ^ 'd',
	't' ^ 'u',
	'U' ^ 'm',
	's' ^ 'p',
	'e' ^ 'R',
	'O' ^ 'e',
	'b' ^ 'a',
	'j' ^ 'l',
	'd' ^ 'M',
	'u' ^ 'e',
	'm' ^ 'n',
	'p' ^ 'U',
};

void showUsage(char *arg)
{
	fprintf(stderr, "Usage: %s <pass>\n", arg);
}

int badBoy()
{
	fprintf(stderr, "Access denied!\n");
	return EXIT_FAILURE;
}

int goodBoy()
{
	printf("Access granted! Go claim your points! ;-)\n");
	return EXIT_SUCCESS;
}

int checkInput(char *input)
{
	size_t i, j;
	size_t index;
	size_t length;
	char*  thisKey;
	char   ret;
	
	ret     = 0;
	
	length  = strlen(input);
	
	index   = (input[0] ^ input[length - 1]);
	index  %= sizeof(key) / sizeof(key[0]);
	
	thisKey = (char*)key[index];
	
	/* Slow equal */
	for(i = j = 0; i < length - 2; i++, j++) {
		if(0 == thisKey[j])
			j = thisKey[j];

		ret |= input[i + 1] ^ thisKey[j] ^ flag[i];
	}
	
	ret |= flag[i];
	
	return ret;
}

int main(int argc, char *argv[])
{
	char *input;
	
	if(argc != 2) {
		showUsage(argv[0]);
		return EXIT_FAILURE;
	}
	
	input = argv[1];
	
	if(strncmp(input, "BZHCTF", 6) != 0 || checkInput(&input[6]))
		return badBoy();
	else
		return goodBoy();
}
