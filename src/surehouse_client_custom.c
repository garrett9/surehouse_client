/*
 * surehouse_client_custom.c
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  This function will issue a request to the /Reporting/Recent endpoint in the SureHouse application.
 *  It will use the provided QUERY_PARAMS structure for the parameters of the query, and the provided
 *  SUREHOUSE_CLIENT structure to make the request.
 */
#include "../lib/surehouse_client.h"

char *surehouse_client_custom(SUREHOUSE_CLIENT *client, QUERY_PARAMS *params) {
	return surehouse_client_query(client, params, "/Reporting/Custom");
}
