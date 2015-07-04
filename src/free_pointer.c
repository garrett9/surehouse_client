/*
 * free_pointer.h
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  Free some pointer from memory, and set it to NULL.
 */
#include "../lib/surehouse_client.h"

void free_pointer(void *pointer) {
	if(pointer != NULL) {
		free(pointer);
		pointer = NULL;
	}
}
