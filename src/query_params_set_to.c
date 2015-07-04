/*
 * query_params_set_to.c
 *
 *  Created on: July 2, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  This function will allow for setting the toDate and toTime parameters on the query. While the toDate field
 *  is required, you can specify NULL for the toTime field to set it to a time of 00:00:00.
 */
#include "../lib/surehouse_client.h"

void query_params_set_to(QUERY_PARAMS *params, char *date, char *time) {
	if(date != NULL) {
		if(params->toDate != NULL)
			free(params->toDate);
		if((params->toDate = malloc(sizeof(char) * (strlen(date) + 1))) != NULL)
			strcpy(params->toDate, date);
	}

	if(time != NULL) {
		if(params->toTime != NULL)
			free(params->toTime);
		if((params->toTime = malloc(sizeof(char *) * (strlen(date) + 1))) != NULL)
			strcpy(params->toTime, time);
	}
}
