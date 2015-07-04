/*
 * surehouse_client_ignore_ssl_warning.c
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  In the event that the SureHouse web application does not have a verified SSL certificate yet, then you can use this function temporaily to ignore
 *  SSL warnings.
 */
#include "../lib/surehouse_client.h"

void surehouse_client_ignore_ssl_warning(SUREHOUSE_CLIENT *client) {
	if(client != NULL && client->curl != NULL) {
		curl_easy_setopt(client->curl, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(client->curl, CURLOPT_SSL_VERIFYPEER, 0);
	}
}
