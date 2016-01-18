#ifndef sppu_h

#define sppu_h

#include <stdlib.h>

#include <stdint.h>

#include <stdio.h>

#include <string.h>

#define OPERATION_PARAMETERS int argc, char *argv[]

struct _operation {
	
	/* ~ Arguments with this key are forwarded to the handler. ~ */
	
	char *key;
	
	/* ~ The help message for the operation. ~ */
	
	char *help;
	
	/* ~ The handler of the operation. Accepts the remaining argument count and list. ~ */
	
	void (* handler)(OPERATION_PARAMETERS);
	
};

void sppu_help(OPERATION_PARAMETERS);

#endif