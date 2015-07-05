/*
 * curl_get.c
 *
 *  Created on: Jul 2, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  This function will issue a request to the SureHouse web application provided with a route, and a JSON string to post.
 */
#include "../lib/surehouse_client.h"

char *surehouse_client_query(SUREHOUSE_CLIENT *client, QUERY_PARAMS *params, char *route) {
	if(client == NULL || client->curl == NULL || params == NULL || route == NULL)
		return NULL;

	long http_code = 0;
	CURLcode res;
	HTTP_RESPONSE response;
	int reauth = 0;
	char *url;
	char *payload;
	char *ret = NULL;

	if((url = malloc(sizeof(char) * (strlen(client->domain) + strlen(route) + 1))) == NULL)
		return NULL;

	/* Builder the URL to request */
	strcpy(url, client->domain);
	strcat(url, route);

	response.body = malloc(1);
	response.size = 0;

	if((payload = query_params_to_json(params)) == NULL)
		return NULL;

	/* Set the CURL options */
	curl_easy_setopt(client->curl, CURLOPT_URL, url);
	curl_easy_setopt(client->curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(client->curl, CURLOPT_POSTFIELDS, payload);
	curl_easy_setopt(client->curl, CURLOPT_WRITEFUNCTION, write_memory_callback);
	curl_easy_setopt(client->curl, CURLOPT_WRITEDATA, (void *)&response);

	if(client->debug_send != 0)
		printf("surehouse_client_query SEND: %s\n", payload);

	res = curl_easy_perform(client->curl);
	if(res != CURLE_OK)
		ret = NULL;
	else {
		curl_easy_getinfo(client->curl, CURLINFO_RESPONSE_CODE, &http_code);

		if(client->debug_recieve != 0)
			printf("surehouse_client_query RECIEVE: %s\n", response.body);

		/*
		 * If the Status code of the response if a 419, then that means our authenticate has expired.
		 * Therefore, we specify later that we want to reauthenticate ourselves before trying again.
		 */
		if(http_code == 419)
			reauth = 1;
		/*
		 * A 200 response was provided back, so we provide the entire response string back to the
		 * caller of the function.
		 */
		else if(http_code >= 200 && http_code < 300)
			ret = response.body;
		/*
		 * An error greater than 400 was returned, meaning we're no longer able to make queries.
		 * Therefore, we return NULL.
		 */
		else
			return NULL;
	}

	free_pointer(payload);
	free_pointer(url);

	if(reauth) {
		/*
		 * If we made it here, that means a 419 response was recieved. This means that our
		 * authentication token has expired, meaning we have to re-authenticate ourselves
		 * before trying again. Therefore, we free the response we got since it's meaningless,
		 * and we then attempt to authenticate ourselves again. If successfully, we recursively
		 * call this function again to try to make a successfull request. If we could not
		 * authenticate, then we're done, so return NULL.
		 */
		free(response.body);
		if(!surehouse_client_authenticate(client))
			return NULL;
		return surehouse_client_custom(client, params);
	}

	return ret;
}
