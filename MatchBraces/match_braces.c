/*
 * match_braces.c
 *
 *  Created on: May 27, 2021
 *      Author: whein
 */


#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

bool isCandidateOpen(char c) {
    switch(c) {
        case '{':
        case '[':
            return true;
        default:
            return false;
    }
}

bool isCandidateClose(char matchTo, char candidate) {
    switch(matchTo) {
        case '{':
            if (candidate == '}') return true;
            break;
        case '[':
            if (candidate == ']') return true;
    }
    return false;
}

const char* findOpenCandidate(const char *srcStr)
{
	size_t length = 0;
	if (!srcStr)
		return (const char *)NULL;

	length = strlen(srcStr);
	while(--length > 0 && *srcStr != '\0') {
		if (isCandidateOpen(*srcStr)) {
			return srcStr;
		}
		srcStr++;
	}
	return (const char *)NULL;
}

/* input {[]] */
/* input {[]} */
/* input jfkd{ */
bool bracesComplete(const char *string) {
    size_t length = strlen(string);
    bool matchFound = false;
    const char *start = string;
    const char *end = &string[length-1];
    while ((start = findOpenCandidate(start)) != (const char *)NULL)
    {
    	matchFound = false;
    	if (start == end)
    		break;
        while (end > start) {
            matchFound = isCandidateClose(*start, *end);
            if (matchFound) {
            	// fprintf(stderr, "Match found for 0x%" PRIxPTR " 0x%" PRIxPTR "\n", start, end);
                end--;
                start++;
                break;
            } else {
                end--;
            }
        }
        if (matchFound == false)
        	break;
    }
    return(matchFound);
}
