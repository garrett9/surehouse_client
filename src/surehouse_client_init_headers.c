/*
 * surehouse_client_init_headers.c
 *
 *  Created on: July 1, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  This function should be used for initializing the default headers to send on each CURL request using the SUREHOUSE_CLIENT.
 *
 */
#include "../lib/surehouse_client.h"

void surehouse_client_init_headers(SUREHOUSE_CLIENT *client) {
	curl_slist_free_all(client->headers);
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charsets: utf-8");
	curl_easy_setopt(client->curl, CURLOPT_HTTPHEADER, headers);
	client->headers = headers;
}
