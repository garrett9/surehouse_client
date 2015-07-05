/*
 * surehouse_client.h
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 */

#ifndef SUREHOUSE_CLIENT_H_
#define SUREHOUSE_CLIENT_H_

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <curl/curl.h>
#include <string.h>
#include <json/json.h>

/*
 |-------------------------------------------------------------------------
 | HTTPResponse
 |-------------------------------------------------------------------------
 |
 | A struct for handling the HTTP response body after making a
 | CURL request.
 |
 */
struct HTTPResponse {
	char *body;
	size_t size;
};

typedef struct HTTPResponse HTTP_RESPONSE;

/*
|--------------------------------------------------------------------------
| QueryParams struct
|--------------------------------------------------------------------------
|
| A struct for representing the query parameters that will be used when
| making a CURL request.
|
*/
struct QueryParams {
	/* The array of sensors names to query */
	char **sensors;
	/* The time to query from */
	char *fromTime;
	/* The date to query from */
	char *fromDate;
	/* The time to query to. */
	char *toTime;
	/* The date to query to. */
	char *toDate;
	/* The time interval in minutes in which rows should be aggregated together. */
	unsigned int aggregate;
	/* Whether or not to return absolute values. */
	unsigned int absolute;
	/* How many rows to return */
	unsigned int rows;
	/* How the results should be ordered (0 = ascending; 1 = descending) */
	unsigned int order_by;
	/* Query sensor logs for this past number of minutes. */
	unsigned int minutes;
	/* How many minutes from the current time you want to start read previous data records for. */
	unsigned int skip;
};

typedef struct QueryParams QUERY_PARAMS;

/*
|--------------------------------------------------------------------------
| SureHouseClient struct
|--------------------------------------------------------------------------
|
| A struct for representing a SUREHOUSE_CLIENT instance.
| It holds the query parameters that should be used for the CURL request,
| as well as the CURL struct to make the request.
| It also holds the DOMAIN of the SureHouse web app.
|
*/
struct SureHouseClient {
	/* Print out all data being sent to the server to the command line when not 0. */
	unsigned int debug_send;
	/* Print out all data being recieved from the server to the command line when not 0 */
	unsigned int debug_recieve;
	/* The Base domain of each request. */
	char *domain;
	/* The username for logging into the system. */
	char *username;
	/* The password for logging into the system. */
	char *password;
	/* The authentication token stored for each request. */
	char *token;
	/* The CURL structure used for making requests. */
	CURL *curl;
	/* The current headers attached to each CURL request. */
	struct curl_slist *headers;
};

typedef struct SureHouseClient SUREHOUSE_CLIENT;

/*
|--------------------------------------------------------------------------
| QUERY_PARAMS functions
|--------------------------------------------------------------------------
|
| Below are functions used to interact with the QUERY_PARAMS structure.
|
*/
#define xquery_params_init(...) query_params_init(__VA_ARGS__, NULL)

QUERY_PARAMS *query_params_init(char *, ...);
void query_params_set_from(QUERY_PARAMS *, char *, char *);
void query_params_set_to(QUERY_PARAMS *, char *, char *);
char *query_params_to_json(QUERY_PARAMS *);
void query_params_clean();

/*
|--------------------------------------------------------------------------
| SUREHOUSE_CLIENT functions
|--------------------------------------------------------------------------
|
| Below are functions used to interact with the SUREHOUSE_CLIENT structure.
|
*/

SUREHOUSE_CLIENT *surehouse_client_init(char *, char *, char *);
void surehouse_client_init_headers(SUREHOUSE_CLIENT *);
int surehouse_client_authenticate(SUREHOUSE_CLIENT *);
char *surehouse_client_custom(SUREHOUSE_CLIENT *, QUERY_PARAMS *);
char *surehouse_client_recent(SUREHOUSE_CLIENT *, QUERY_PARAMS *);
char *surehouse_client_query(SUREHOUSE_CLIENT *, QUERY_PARAMS *, char *);
void surehouse_client_ignore_ssl_warning(SUREHOUSE_CLIENT *);
void surehouse_client_debug_send(SUREHOUSE_CLIENT *);
void surehouse_client_debug_receive(SUREHOUSE_CLIENT *);
void surehouse_client_close(SUREHOUSE_CLIENT *);

/*
|--------------------------------------------------------------------------
| Utility functions
|--------------------------------------------------------------------------
|
| Below are functions used for utility purposes.
|
*/

void error(const char *, ...);
char *rtrim(char *, char);
void free_pointer(void *);
void *xmemcpy(void *, size_t size);
size_t write_memory_callback(void *, size_t, size_t, void *);

#endif /* SUREHOUSE_CLIENT_H_ */
