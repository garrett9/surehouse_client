/*
 * rtrim.c
 *
 *  Created on: Jun 28, 2015
 *      Author: garrettshevach@gmail.com
 *
 *  Given a string, and character, this function will remove trailing instances of that character from the string.
 *
 */
#include "../lib/surehouse_client.h"

char* rtrim(char* string, char junk) {
    char* original = string + strlen(string);
    while(*--original == junk);
    *(original + 1) = '\0';
    return string;
}
