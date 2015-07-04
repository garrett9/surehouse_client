/*
 * query_params_to_json.c
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  This function will convert a QUERY_PARAMS structure to a JSON string representation, and then return it.
 */
#include "../lib/surehouse_client.h"

char *query_params_to_json(QUERY_PARAMS *params) {
	if(params == NULL || params->sensors == NULL)
		return NULL;

	int i;

	/* Initialize a JSON object */
	json_object *json = json_object_new_object();

	/* Initialize a JSON array for the sensors */
	json_object *sensors = json_object_new_array();

	/* For each sensor, add it to the JSON array. */
	for(i = 0; params->sensors[i] != NULL; i++)
		json_object_array_add(sensors, json_object_new_string(params->sensors[i]));

	/* Add the JSON array to the JSON object. */
	json_object_object_add(json, "sensors", sensors);

	/* How should the results be aggregated */
	json_object_object_add(json, "aggregate", json_object_new_int(params->aggregate));

	/* If only absolute values should be returned */
	json_object_object_add(json, "absolute", json_object_new_boolean(params->absolute));

	/* The number of rows to return */
	if(params->rows > 0)
		json_object_object_add(json, "rows", json_object_new_int(params->rows));

	/* The number of past minutes from the current time to query data for. */
	if(params->minutes > 0)
		json_object_object_add(json, "minutes", json_object_new_int(params->minutes));

	/* How many minutes from the current time to start reading previous data records for. */
	if(params->skip > 0)
		json_object_object_add(json, "skip", json_object_new_int(params->minutes));

	/* The time to query from */
	if(params->fromTime != NULL)
		json_object_object_add(json, "fromTime", json_object_new_string(params->fromTime));

	/* The date to query from */
	if(params->fromDate != NULL)
		json_object_object_add(json, "fromDate", json_object_new_string(params->fromDate));

	/* The time to query to */
	if(params->toTime != NULL)
		json_object_object_add(json, "toTime", json_object_new_string(params->toTime));

	/* The date to query to */
	if(params->toDate != NULL)
		json_object_object_add(json, "toDate", json_object_new_string(params->toDate));

	/* How the results should be ordered (1 = descending; 0 = ascending). */
	if(params->order_by)
		json_object_object_add(json, "order_by", json_object_new_string("desc"));
	else
		json_object_object_add(json, "order_by", json_object_new_string("asc"));

	return (char *)json_object_to_json_string(json);
}
