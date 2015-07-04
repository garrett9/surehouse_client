/*
 * xmemcpy.c
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  Given a pointer to memory, this function will copy the number given bytes to a newly allocated pointer, and return it.
 */
#include "../lib/surehouse_client.h"

void *xmemcpy(void *mem, size_t bytes) {
	void *new_mem;
	if((new_mem = malloc(sizeof(mem) * bytes)) == NULL)
		return NULL;

	if(memcpy(new_mem, mem, bytes) == NULL)
		return NULL;
	return new_mem;
}
