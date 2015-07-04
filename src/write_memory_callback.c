/*
 * write_body_callback.c
 *
 *  SOURCE: http://curl.haxx.se/libcurl/c/getinbody.html
 *
 *  This function will be used to store the response's body after making a CURL request.
 */
#include "../lib/surehouse_client.h"

size_t write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct HTTPResponse *resonse = (struct HTTPResponse *)userp;

  resonse->body = realloc(resonse->body, resonse->size + realsize + 1);
  if(resonse->body == NULL)
    return 0;

  memcpy(&(resonse->body[resonse->size]), contents, realsize);
  resonse->size += realsize;
  resonse->body[resonse->size] = 0;

  return realsize;
}
