/*
 * query_params_set_from.c
 *
 *  Created on: July 2, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  This function will allow for setting the fromDate and fromTime parameters on the query. While the fromDate field
 *  is required, you can specify NULL for the fromTime field to set it to a time of 00:00:00.
 */
#include "../lib/surehouse_client.h"

void query_params_set_from(QUERY_PARAMS *params, char *date, char *time) {
	if(date != NULL) {
		if(params->fromDate != NULL)
			free(params->fromDate);
		if((params->fromDate = malloc(sizeof(char) * (strlen(date) + 1))) != NULL)
			strcpy(params->fromDate, date);
	}

	if(time != NULL) {
		if(params->fromTime != NULL)
			free(params->fromTime);
		if((params->fromTime = malloc(sizeof(char *) * (strlen(date) + 1))) != NULL)
			strcpy(params->fromTime, time);
	}
}
