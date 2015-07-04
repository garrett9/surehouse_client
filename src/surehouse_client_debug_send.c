/*
 * surehouse_client_debug_send.c
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  Turn on debug mode to print all data sent to the server to the command line.
 */
#include "../lib/surehouse_client.h"

void surehouse_client_debug_send(SUREHOUSE_CLIENT *client) {
	client->debug_send = 1;
}
