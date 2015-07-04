/*
 * query_params_init.c
 *
 *  Created on: Jun 29, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  This function will create a QUERY_PARAMS structure provided with an array of sensor names to initialize it with.
 */
#include "../lib/surehouse_client.h"

QUERY_PARAMS *query_params_init(char *sensor, ...) {
	int i;
	int size;
	int res = 0;
	int allocated = 0;
	char *temp;
	char **sensors;
	va_list arguments;
	QUERY_PARAMS *params;

	/* Allocate memory for the QUERY_PARAMS */
	if((params = malloc(sizeof(struct QueryParams))) == NULL)
		return NULL;

	va_start(arguments, sensor);
	/* Count how many arguments were given. */
	for(i = 0; sensor != NULL; sensor = va_arg(arguments, char *), i++) {
		/*
		 * The amount of space required to store the sensor list is n + 1,
		 * where n is the number of sensors being stored. The +1 is used to
		 * store a NULL terminating string as the last sensor.
		 */
		size = sizeof(char *) * (i + 2);

		/*
		 * We first check to see if we allocated a double pointer to store our sensors.
		 * If not, then we use malloc. If we did, then we use realloc to reallocate the
		 * required memory to store our new sensors.
		 */
		if(!allocated) {
			if((sensors = (char **)malloc(size)) == NULL) {
				res = 0;
				break;
			}
			allocated = 1;
		}
		else
			if((sensors = (char **)realloc(sensors, size)) == NULL) {
				res = 0;
				break;
			}

		/*
		 * Lastly, we create a copy of the provided sensor, and place it in the double pointer
		 * for later use. We also put a NULL terminating string as the last element in the list
		 * to let us know we reached the end.
		 */
		temp = (char *)xmemcpy(sensor, strlen(sensor) + 1);
		sensors[i] = temp;
		res = 1;
	}

	sensors[i] = NULL;

	va_end(arguments);

	/* If the sensors could not be set to the params, then free the allocated params, and return NULL */
	if(!res) {
		query_params_clean(params);
		return NULL;
	}

	params->sensors = sensors;
	params->fromTime = NULL;
	params->fromDate = NULL;
	params->toTime = NULL;
	params->toDate = NULL;
	params->aggregate = 0;
	params->absolute = 0;
	params->rows = 0;
	params->order_by = 0;
	params->minutes = 0;
	params->skip = 0;

	return params;
}
