/*
 * match_braces.c
 *
 *  Created on: May 27, 2021
 *      Author: whein
 */


#include <stdio.h>
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
	fprintf(stderr, "srcStr: %s, length = %zd\n", srcStr, length);
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
/* input jfkd{ */
bool bracesComplete(const char *string) {
    size_t length = strlen(string);
    int matchFound = false;
    const char *start = string;
    const char *end = &string[length-1];
    while (start != end) {
        matchFound = true;
        if (isCandidateOpen(*start)) {
            matchFound = false;
            while (end != start) {
                matchFound = isCandidateClose(*start, *end);
                if (matchFound) {
                    end--;
                    start++;
                } else {
                    end--;
                }
            }
            start++;
            // Did not find a match, can just exit out.
            if (!matchFound)
                return (false);
        }
        start++;
    }
    return(matchFound);
}
