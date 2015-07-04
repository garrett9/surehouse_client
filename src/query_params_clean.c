/*
 * query_params_clean.c
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  This function will free all resouces allocated for a QUERY_PARAMS struct.
 */
#include "../lib/surehouse_client.h"

void query_params_clean(QUERY_PARAMS *params) {
	if(params != NULL) {

		/* Free the sensors from the params */
		if(params->sensors != NULL) {
			int i;

			/* Free all individual sensors in the double pointer. */
			for(i = 0; params->sensors[i] != NULL; i++)
				free_pointer(params->sensors[i]);

			/* Free the double pointer */
			free(params->sensors);
			params->sensors = NULL;
		}

		/* Free all resources associated to the structure */
		free_pointer(params->fromTime);
		free_pointer(params->toTime);
		free_pointer(params);
	}
}
