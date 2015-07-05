# SureHouse Client

- [About](#about)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Usage](#usage)
- [Parameters](#parameters)
	- [sensors](#sensors)
	- [fromDate](#fromdate)
	- [fromTime](#fromtime)
	- [toDate](#todate)
	- [toTime](#totime)
	- [aggregate](#aggregate)
	- [absolute](#absolute)
	- [order_by](#order_by)
	- [minutes](#minutes)
	- [skip](#skip)
- [Debugging](#debugging)
- [Authentication](#authentication)
- [Results](#results)
- [Tools](#tools)

# About

This library is a C client library used to interact with the SureHouse REST API. It relies on libcurl to handle HTTP requests, and JSON-C to create POST data and parse responses. This library also handles authentication with the API, along with the ability to easily construct queries to be sent to the API.

# Dependencies

- [libcurl](http://curl.haxx.se/libcurl/) is used to handle HTTP requests to the REST API. The following command would install the library on an Ubuntu based system.
	```
	pkg-config --libs libcurl
	```
	Be sure to also include the header file for the library.
	```C
	#include <curl/curl.h>
	```

- [JSON-C](https://linuxprograms.wordpress.com/2010/05/20/json-c-libjson-tutorial/) is used to build POST data to send to the API, and parse responses from the API. The following command would install the library on an Ubuntu based system.
	```
	sudo apt-get install libjson0 libjson0-dev
	```
	Be sure to also include the header file for the library.
	```C
	#include <json/json.h>
	```

# Installation

To install the library, you can simply run the following command to pull the project from Github.

```
git clone https://github.com/garrett9/surehouse_client
```

Upon pulling the project, you will see in the **lib** directory the library file (libsurehouse_client.a). You can compile your own programs that utilize this library like so...

```
gcc -lcurl -ljson my_program.c -o my_program ../lib/libsurehouse_client.a
```
- **Note**: You might want to consider recompiling the code on your system before continuing. To do this simply run the following command in the **src** directory. In doing so, it will create another library file for the project, and place it in the **lib** directory, thus overwriting the old one.

	```make lib``` 

- **Note**: Be sure to include the libraries **-lcurl** and **-ljson** in your compile command since the client library relies heavily on both.

You will also see the header file for the library in the **lib** directory as well (surehouse_client.h). Be sure to include this in your project's source code wherever you utilze the client's functions.

```C
#include "../lib/surehouse_client.h"
```

# Usage

The client provides two structures that assist in interacting with the SureHouse REST API; **SUREHOUSE_CLIENT**, and **QUERY_PARAMS**. The SUREHOUSE_CLIENT is responsible for handling all cURL requests to the API. Therefore, only one instance is necessary in your program. You can initialize a SUREHOUSE_CLIENT through the following commands.
```C
/* Initialize the client */
SUREHOUSE_CLIENT *client = surehouse_client_init("API DOMAIN", "USERNAME", "PASSWORD");

/* 
 * In the event that the API does not yet have an SSL certificate signed by a trusted third party, you will have to call the following function in 
 * in order to successfully make HTTP requests to the API.
 */
surehouse_client_ignore_ssl_warning(client);

/* Make sure you then authenticate yourself with the API */
if(!surehouse_client_authenticate(client)) {
	/* Authentication failed, so close the client, and close the program through the "error" function. */
	surehouse_client_close(client);
	error("Failed to authenticate the client with the given credentials!");
}
```
Be sure to authenticate yourself with the API BEFORE making any HTTP requests. If you would like to, you can learn more about how authentication works  [here](#authentication). 

For querying data, there's two type of functions, or routes, to do so.
- **https://[domain]/Reporting/Custom**: This route offers the most functionality, and allows you to pinpoint specifc time frames on when to query data. Below is an example on how you can send parameters to this route via POST.

	```
	{
	  "sensors": [
	    "CT_SERVICE_L1",
	    "CT_SERVICE_L2"
	  ],
	  "aggregate": 1,
	  "absolute": false,
	  "fromTime": "13:00",
	  "fromDate": "2015-07-02",
	  "toTime": "13:00",
	  "toDate": "2015-07-03",
	  "order_by": "asc"
	}
	```
	
	The above parameters issue a query for the datapoints CT_SERVICE_L1 and CT_SERVICE_L2. It specifies to group the results by every minute from 2015-07-02 1:00PM to 2015-07-03 1:00PM, which would be 24 hours. Lastly, it specifies to sort the results by the time stamp in an ascending order. The other option for sorting would be "desc", which returns results sorted by the time stamp in a descending order. If you specify no aggregate, meaning you want to aggregate all of the results within the time frame, then "order_by" would have no effect. You can issue the same query via a GET request through the following URL, but keep in mind that the client library uses the POST method.
	
	```
	https://[domain]/Reporting/Custom?sensors[]=CT_SERVICE_l1&sensors[]=CT_SERVICE_L2&aggregate=1&absolute=false&fromTime=13:00&fromDate=2015-07-02&toTime=13:00&toDate=2015-07-03&order_by=asc
	```
	
	Here is an example on how to make the request by using the client, assuming you have already initialized it the way described near the beginning of this section.
	
	```C
	/* Initialize a QUERY_PARAMS structure. */
	QUERY_PARAMS *params;
	if((params = xquery_params_init("CT_SERVICE_L1", "CT_SERVICE_L2")) == NULL) {
		/*
		 * This would rarely happen, but the QUERY_PARAMS structure could not be initialized.
		 * Therefore, clean up any open resources, and then exit the program using the "error" function.
		 */
		surehouse_client_close(client);
		error("Failed to instantiate the Query Parameters structure!");
	}
	
	/*
	 * Set your parameters here.
	 * I'm not setting the "order_by" or "absolute" fields here because they default to
	 * "asc" and "false" respectively.
	 */
	query_params_set_to(params, "2015-07-03", "13:00");
	query_params_set_from(params, "2015-07-02", "13:00");
	params->aggregate = 1;
	
	/*
	 * Make the HTTP request by using the SUREHOUSE_CLIENT, as well as your contructed QUERY_PARAMS.
	 * The result would be a char pointer on success, or NULL on failure.
	 */
	char *payload = surehouse_client_custom(client, params);
	```

- **https://[domain]/Reporting/Recent**: This route offers slightly less functionality, but allows you to repeatedly query most recent data without having to manipulate dates on the client side. Below is an example on how you can send parameters to this route via POST.

	```
	{
	  "sensors": [
	    "CT_SERVICE_L1",
	    "CT_SERVICE_L2"
	  ],
	  "aggregate": 1,
	  "absolute": false,
	  "minutes": 1440,
	  "skip": 1440,
	  "order_by": "asc"
	}
	```

	The above parameters issue a query for the datapoints CT_SERVICE_L1 and CT_SERVICE_L2. What's different about this query from the previous one is that there are two new parameters exclusive to this route; **minutes** and **skip**. Think of **skip** as a way to dynamically set the **toDate** and **toTime** fields without having to manipulate dates on the client side. It's the number of minutes to subtract, or "skip" from the current time before reading the previous records before it. Therefore, if the field is not specified, then the **toDate** will be set to the current time. You can then think of **minutes** as a way to dynamically set the **fromDate** and **fromTime** fields. In otherwords, it's the number of minutes before the current time stamp minus **skip**. Below is perhaps a better representation of what these fields do.
	
	- from_timestamp = (current_timestamp - **skip**) - **minutes**
	- to_timestamp = (current_timestamp - **skip**)

	For the above query, since 1440 minutes equals a total 24 hours, it's specifying to query all data for the previous day, or 48 hours before the current time to 24 hours before the current time. You can issue the same query via GET through the following URL, but keep in mind that the client library uses the POST method.

	```
	https://[domain]/Reporting/Custom?sensors[]=CT_SERVICE_l1&sensors[]=CT_SERVICE_L2&aggregate=1&absolute=false&minutes=1440&skip=1440&order_by=asc
	```

	Here is an example on how to make the request by using the client. It's very similar to the previous route.

	```C
	/* Initialize a QUERY_PARAMS structure. */
	QUERY_PARAMS *params;
	if((params = xquery_params_init("CT_SERVICE_L1", "CT_SERVICE_L2")) == NULL) {
		/*
		 * This would rarely happen, but the QUERY_PARAMS structure could not be initialized.
		 * Therefore, clean up any open resources, and then exit the program using the "error" function.
		 */
		surehouse_client_close(client);
		error("Failed to instantiate the Query Parameters structure!");
	}
	
	/*
	 * Set your parameters here.
	 * I'm not setting the "order_by" or "absolute" fields here because they default to
	 * "asc" and "false" respectively.
	 */
	params->skip = 60 * 24; //24 hours
	params->minutes = 60 * 24; //24 hours
	params->aggregate = 1;
	
	/*
	 * Make the HTTP request by using the SUREHOUSE_CLIENT, as well as your contructed QUERY_PARAMS.
	 * The result would be a char pointer on success, or NULL on failure.
	 */
	char *payload = surehouse_client_recent(client, params);
	```

For a more in depth explanation of all of the available parameters for both routes, click [here](#parameters).

As a final remark, when you're done, be sure to close all of your resources to free up memory.

```C
/* Clean up the SUREHOUSE_CLIENT */
surehouse_client_close(client);

/* Clean up the QUERY_PARAMS */
query_params_clean(params);
```

# Parameters

The client offers a number of different parameters for making complex queries. Below is a detailed list of all of the supported parameters, the routes they are supported on, what their default values are, and what they actually provide.

#### sensors
- **Description**: This field is an array of strings. Each element in the array must match exactly the sensor name that was configured in the web application under the **Sensors** tab in the menu.
- **Supported Route(s)**
	- /Reporting/Custom
	- /Reporting/Recent
- **Default**: None (This is a required field)
- **Usage**

	```C
	params = xquery_params_init("CT_SERVICE_L1", "CT_SERVICE_L2");
	```
	
#### fromDate
- **Description**: All records must have a timestamp greater than or equal to this date, which must be in the format of YYYY-MM-DD. See the [fromTime](#fromtime) parameter to add a time to this date.
- **Supported Route(s)**
	- /Reporting/Custom
- **Default**: None (If left blank, it will not be applied)
- **Usage**

	```C
	query_params_set_from(params, "2015-07-02", NULL);
	```

#### fromTime
- **Description**: This time is represented in a 24 hour clock format, where 14:00 is equal to 2:00pm. This time will be appended to the **fromDate** argument if one is provided. If **fromDate** is not provided, and **fromTime** is, then **fromTime** will have no effect. In other words, a **fromDate** is required in order to use a **fromTime**.
- **Supported Route(s)**
	- /Reporting/Custom
- **Default**: None (If left blank, it will not be applied)
- **Usage**

	```C
	query_params_set_from(params, "2015-07-02", NULL);
	```

#### toDate
- **Description**: All records must have a timestamp less than or equal to this date, which must be in the format of YYYY-MM-DD. See the [toTime](#totime) parameter to add a time to this date.
- **Supported Route(s)**
	- /Reporting/Custom
- **Default**: None (If left blank, it will not be applied)
- **Usage**

	```C
	query_params_set_to(params, "2015-07-03", NULL);
	```
	
#### toTime
- **Description**: This time is represented in a 24 hour clock format, where 14:00 is equal to 2:00pm. This time will be appended to the **toDate** argument if one is provided. If **toDate** is not provided, and **toTime** is, then **toTime** will have no effect. In other words, a **toDate** is required in order to use a **toTime**.
- **Supported Route(s)**
	- /Reporting/Custom
- **Default**: None (If left blank, it will not be applied)
- **Usage**

	```C
	query_params_set_to(params, "2015-07-03", "13:00");
	```
	
#### aggregate
- **Description**: This positive integer specifies the number of minutes the data being queried will be aggregated by. For example, if the integer 5 is supplied, then all values in the records of a 5 minute interval (example: 13:30:00 - > 13:35:00) will be averaged together, and returned in a single row.
- **Supported Route(s)**
	- /Reporting/Custom
	- /Reporting/Recent
- **Default**: 0 (This parameter is optional. When 0 or not included, the entire dataset of the requested sensors will be averaged together to return one single row.)
- **Usage**

	```C
	params->aggregate = 1; //Aggregate by 1 minute intervals
	params->aggregate = 0; //Aggregate all rows into one single value
	```
	
#### absolute
- **Description**: If true, then the absolute value of every value retrieved will be returned. If false, or not included, then the retrieved value will be returned as is.
- **Supported Route(s)**
	- /Reporting/Custom
	- /Reporting/Recent
- **Default**: false (This parameter is optional)
- **Usage**

	```C
	params->absolute = 1; //Return absolute values
	params->absolute = 0; //Return values as is.
	```
	
#### rows
- **Description**: Specify how many rows you want to return. This parameter has no real affect on the query itself besides limiting how many rows are returned.
- **Supported Route(s)**
	- /Reporting/Custom
	- /Reporting/Recent
- **Default**: -1 (This parameter is optional. When not given, or provided with a negative number, all rows will be returned.)
- **Usage**

	```C
	params->rows = 100; //Return a maximum of 100 rows
	params->rows = -1; //Return all of the rows
	```
	
#### order_by
- **Description**: Sort the results by the timestamps in either ascending (asc) or descending (desc) order.
- **Supported Route(s)**
	- /Reporting/Custom
	- /Reporting/Recent
- **Default**: asc (This parameter is optional. When not given, the results will be sorted in ascending order)
- **Usage**

	```C
	params->order_by = 0; //Sort in ascending order
	params->order_by = 1; //Sort in descending order
	```
	
#### minutes
- **Description**: The number of minutes prior to the current timestamp (or current timestamp minus **skip** minutes) to start recording data for. Think of it as a way to dynamically set the **fromDate** and **fromTime** values without having to edit dates on the client side.
- **Supported Route(s)**
	- /Reporting/Recent
- **Default**: 0 (This parameter is optional. When less than or equal to 0, or when not provided, then all rows prior to the current timestamp (or current timestamp minus **skip** minutes) will be returned.
- **Usage**

	```C
	params->minutes = 24 * 60; //Return all rows recorded in the past 24 hours.
	params->minutes = 24 * 60 * 5; //Return all rows recorded in the past 5 days.
	```
	
#### skip
- **Description**: The number of minutes to substract from the current timestamp so that all rows between the current timestamp and the current timestamp minus **skip** minutes will not be returned.
- **Supported Route(s)**
	- /Reporting/Recent
- **Default**: 0 (This parameter is optional. When less than or equal to 0, or when not provided, then no rows will be skipped prior to the current timestamp)
- **Usage**

	```C
	params->skip = 24 * 60; //Return all rows recorded prior to 24 hours before the current timestamp.
	params->skip = 24 * 60 * 5; //Return all rows recorded prior to 5 days before the current timestamp.
	```
	
# Debugging

There could be instances where you would like to know what data you are sending to the server, and what you're receiving. In order to allow for this, you can issue the following function calls after initialize the SUREHOUSE_CLIENT.

```C
SUREHOUSE_CLIENT *client = surehouse_client_init("API DOMAIN", "USERNAME", "PASSWORD");
/* Print out all data sent to the server to the command line. */
surehouse_client_debug_send(client);
/* Print out all data sent from the server to the command line. */
surehouse_client_debug_receive(client);
```

# Authentication

Authentication to the system is handled through JSON (JWT) web tokens. You can think of a JWT as an authentication token signed by the server, and issued to the user. Upon a user sending their credentials to the server, the user will be issued a new JWT to use for each subsequent request. If the token is valid on the server side, then the request will be processed successfully. If the token fails, then one of two different HTTP status codes could be returned.

- **401**: A 401 HTTP response, otherwise known as **Unauthorized** indicates that either a JWT was not provided, or the one sent was not valid.
- **419**: A 419 HTTP response, otherwise know as **Authentication Timeout**, indicates that a JWT was provided, but it has expired.

A 401 response can also be received when a user attempts to send their credentials to the server to get a JWT, and the credentials are incorrect. In order to authenticate yourself with the SureHouse server, you must run the following commands before making any queries for the data.

```C
SUREHOUSE_CLIENT *client = surehouse_client_init("API DOMAIN", "USERNAME", "PASSWORD");
if(!surehouse_client_authenticate(client))
	printf("Authentication failed!");
else
	printf("Authentication succeeded!");
	
```

To go more in details, authentication works through the following steps.

1. The user sends their credentials in the following form via POST to the API route of **/Users/Login**. (Don't include [ ])
	
	```json
	{
		"name": "[My Username]",
		"password": "[My Password]"
	}
	```
	
2. If the credentials were successfully authenticated, then the user will be given the following JSON response. (Don't include [ ])
	
	```json
	{
	    "result": "success",
	    "status": 200,
	    "message": "Successfully authenticated!",
	    "payload": {
	        "auth_token": "[Authentication Token]"
	    }
	}
	```
	
3. The user can now make an authenticated request to a subsequent API call by placing the token in the **Authorization** header like the following. (Don't include [ ])
	
	```
	Authorization: Bearer [Authentication Token]
	```
		
- **Question**: If it's possible for my JWT to expire, wouldn't that mean I would eventually be unable to make any future calls to the API?
- **Answer**: This is 100% correct. However, in using the functions provided in the client for querying data, you don't have to worry about this problem. If the client receives a 419 HTTP response on any request sent to the server, besides the one to authenticate, it will attempt to re-authenticate the client by using your previously entered credentials. If successfull, it will re-issue the previous request, and return the results you were initially after.

# Results

All results from the SureHouse REST API follow the same structure.

```json
{
	"results":"success/error", 
	"status": "HTTP status code", 
	"message":"A message from the server.",
	"payload": ["The Response Data"]
}
```
To go further in detail, an actual successfull response could look like the following.

```json
{
	"results":"success", 
	"status": 200, 
	"message": "OK!", 
	"payload": [
		{
			"Time":"2015-06-28 01:29:00",
			"CT_SERVICE_L1":"86.00",
			"CT_SERVICE_L2":"13.00"
		},
		{
			"Time":"2015-06-28 01:30:00",
			"CT_SERVICE_L1":"10.00",
			"CT_SERVICE_L2":"81.00"
		},
		{
			"Time":"2015-06-28 01:31:00",
			"CT_SERVICE_L1":"20.00",
			"CT_SERVICE_L2":"27.00"
		}
	]
}
```
This particular response contains data for two different sensors (CT_SERVICE_L1 and CT_SERVICE_L2) across three different timestamps. However, the above response was for a request where the **aggregate** parameter was 1, meaning results were aggregated together for every minute. Below is a response where no **aggregate** was specified.

```json
{
    "result": "success",
    "status": 200,
    "message": "OK!",
    "payload": [
        {
            "CT_SERVICE_L1": "49.43",
            "CT_SERVICE_L2": "49.45"
        }
    ]
}
```
The only difference is that there is no **Time** field in the response since the entire data set will be aggregated together if no **aggregate** value was provided. If no results were retrieved from the query, then the "payload" field would be absent. Below is a response of a possible error.

```json
{
    "result": "error",
    "status": 400,
    "message": "The \"sensors\" field is a required array of sensors to query!"
}
```
For the above example, no sensors were provided to query, thus triggering the 400 HTTP response, otherwise known as a Bad Request. You can experience a similar error if you were to query a sensor that did not exist in the system. However, if a response greater than or equal to 400 is received by the client from the server, then a NULL string will be returned as specified earlier under the [Usage](#usage) section.

# Tools

If you are looking for a better way to construct queries before writing the code to do so, I strongly recommend the Chrome plugin [Postman](https://www.getpostman.com/). It supports all basic REST operations, and offers the ability to make HTTP requests, send form data, and preview the response in a cleanly formatted version, or as rendered HTML.

Finally, I strongly recommend the JSON-C library for parsing JSON responses. I felt that it was best to let the user parse the JSON results since the JSON-C library offers far more functionality than I would be able to deliver.

