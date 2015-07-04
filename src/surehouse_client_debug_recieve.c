/*
 * surehouse_client_debug_recieve.c
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  Turn on debug mode to print all data recieved from the server to the command line.
 */
#include "../lib/surehouse_client.h"

void surehouse_client_debug_recieve(SUREHOUSE_CLIENT *client) {
	client->debug_recieve = 1;
}
