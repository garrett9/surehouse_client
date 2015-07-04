/*
 * surehouse_client_authenticate.c
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  This function will issue a request to the /Users/Login endpoint in order to authenticate.
 */
#include "../lib/surehouse_client.h"

int surehouse_client_authenticate(SUREHOUSE_CLIENT *client) {
	if(client == NULL || client->curl == NULL)
		return 0;

	int ret = 0;
	json_object *body;
	json_object *payload;
	CURLcode res;
	HTTP_RESPONSE response;
	char endpoint[] = "/Users/Login";
	char *url;
	char *token;
	if((url = malloc(sizeof(char) * (strlen(client->domain) + strlen(endpoint) + 1))) == NULL)
		return 0;

	response.body = malloc(1);
	response.size = 0;

	/* Build the URL to request */
	strcpy(url, client->domain);
	strcat(url, endpoint);

	/* Build the JSON payload to send */
	json_object *post = json_object_new_object();
	json_object_object_add(post, "name", json_object_new_string(client->username));
	json_object_object_add(post, "password", json_object_new_string(client->password));

	/* Set the CURL options */
	curl_easy_setopt(client->curl, CURLOPT_URL, url);
	curl_easy_setopt(client->curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(client->curl, CURLOPT_POSTFIELDS, json_object_to_json_string(post));
	curl_easy_setopt(client->curl, CURLOPT_WRITEFUNCTION, write_memory_callback);
	curl_easy_setopt(client->curl, CURLOPT_WRITEDATA, (void *)&response);

	if(client->debug_send != 0)
		printf("surehouse_client_authenticate SEND: %s\n", json_object_to_json_string(post));

	res = curl_easy_perform(client->curl);
	if(res != CURLE_OK)
		ret = 0;
	else {
		long http_code;
		curl_easy_getinfo(client->curl, CURLINFO_RESPONSE_CODE, &http_code);

		if(client->debug_recieve != 0)
			printf("surehouse_client_authenticate RECIEVE: %s\n", response.body);

		/*
		 * If we made it here, that means we have the response.
		 * Therefore, we can must save the auth_token for future requests.
		 * However, we can only do this if a 200 response was given, so we
		 * need to check for the status code after making the request.
		 */
		if(http_code >= 200 && http_code < 300) {
			body = json_tokener_parse(response.body);
			payload = json_object_object_get(body, "payload");
			/* Make sure a payload was given. */
			if(payload != NULL) {
				/* Make sure the auth_token was given. */
				if((token = (char *)json_object_get_string(json_object_object_get(payload, "auth_token"))) != NULL) {
					ret = 1;
					client->token = token;
					/*
					 * Now that we know we definately know we have the token, let's attach it to
					 * the curl's headers. First, we allocate a buffer to store the header. The
					 * header will always be 357 bytes when putting together "Authorization: Bearer {token}".
					 * We then re-initialize the headers for the CURL client so that we can add in the
					 * Authorization header.
					 */
					char authorization[357];
					strcpy(authorization, "Authorization: Bearer ");
					strcat(authorization, token);
					surehouse_client_init_headers(client);
					client->headers = curl_slist_append(client->headers, authorization);
				}
			}
		}
	}

	/* Do some cleanup */
	free(post);
	free(body);
	free(payload);
	free(response.body);
	free(url);
	free(token);

	return ret;
}
