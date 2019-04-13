#!/usr/bin/php
<?php
const PORT   = 3213;
const FLAG   = "My_CRaCKMeS_RuN_iN_youR_KeRNeL";
const OFFSET = 15;

$flag = FLAG;
if(strlen($flag) % 2)
	$flag .= "}";

$hash = PORT ^ ((8 + strlen("BZHCTF{" . FLAG . "}")) << 10);
 
for($i = 0; $i < strlen($flag); $i += 2) {
	$word  = (ord($flag[$i]) << 8) | ord($flag[$i + 1]);
	$hash ^= $word;

	printf("# %s (%04X)\n", substr($flag, $i, 2), $word);
	printf("ldh [%d]\n", OFFSET + $i);
	printf("xor x\n");
	printf("jne #0x%04X, fail\n", $hash);
	printf("tax\n\n");
}
