/*
 * error.c
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  This function is used for closing cleaning up any open resources, as well as printing an error message and terminating the program.
 *
 */

#include "../lib/surehouse_client.h"

void error(const char *format, ...) {
	va_list args;
	printf("\nERROR: ");
	va_start(args, format);
	printf(format, args);
	va_end(args);
	printf("\n\n");
	exit(0);
}
