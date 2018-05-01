char** asm_text;

/* array of strings */
asm_text = malloc( sizeof(char*) * numlines * 80);

if( (asm_text == NULL) ){
	/* Issues with allocation */
	return -1;
}

int i;
for( i = 0; i < numlines; i++){
	*(asm_text + i)  = "\n\0";
}
