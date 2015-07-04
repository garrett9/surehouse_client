/*
 * surehouse_client_close.c
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  Cleanup any open resources associated to a SUREHOUSE_CLIENT
 */
#include "../lib/surehouse_client.h"

void surehouse_client_close(SUREHOUSE_CLIENT *client) {
	if(client != NULL) {
		/* Close CURL */
		curl_easy_cleanup(client->curl);
		/* Clean up the headers */
		curl_slist_free_all(client->headers);

		/* Free all pointers associated to the structure */
		free_pointer(client->domain);
		free_pointer(client->password);
		free_pointer(client->token);
		free_pointer(client);
	}
}
