/*
 * surehouse_client_init.c
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  This function should be used for initializing a SUREHOUSE_CLIENT context.
 *
 */

#include "../lib/surehouse_client.h"

SUREHOUSE_CLIENT *surehouse_client_init(char * domain, char *username, char *password) {
	char *client_domain;
	char *client_username;
	char *client_password;
	CURL *curl;
	SUREHOUSE_CLIENT *client;
	struct curl_slist *headers = NULL;

	/* Initialize the CURL structure */
	if(!(curl = curl_easy_init()))
		return NULL;

	/* Allocate memory for the SUREHOUSE_CLIENT */
	if((client = (SUREHOUSE_CLIENT *)malloc(sizeof(struct SureHouseClient))) == NULL) {
		curl_easy_cleanup(curl);
		return NULL;
	}

	client_domain = (char *)xmemcpy(domain, strlen(domain) + 1);
	client_username = (char *)xmemcpy(username, strlen(username) + 1);
	client_password = (char *)xmemcpy(password, strlen(password) + 1);

	client->curl = curl;
	client->headers = headers;
	client->domain = rtrim(client_domain, '/');
	client->username = client_username;
	client->password = client_password;
	client->debug_send = 0;
	client->debug_recieve = 0;

	surehouse_client_init_headers(client);
	return client;
}


