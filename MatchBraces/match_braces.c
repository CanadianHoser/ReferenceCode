/*
 * match_braces.c
 *
 * Ensure all braces have a match.  Braces need to have a matching close brace before
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

bool isMatchingCandidate(char matchTo, char candidate) {
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
    bool matchFound = true;
    const char *currentCandidate = string;
    const char *charToCheck = &string[length-1];
    while ((currentCandidate = findOpenCandidate(currentCandidate)) != NULL)
    {
    	matchFound = false;
    	if (currentCandidate == charToCheck)
    		break;
        while (charToCheck > currentCandidate) {
            matchFound = isMatchingCandidate(*currentCandidate, *charToCheck);
            if (matchFound) {
                charToCheck--;
                currentCandidate++;
                break;
            } else {
                charToCheck--;
            }
        }
        if (matchFound == false)
        	break;
    }
    return(matchFound);
}
