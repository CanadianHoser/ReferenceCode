/*
 * match_braces.c
 *
 *  Created on: May 27, 2021
 *  Author: Warren Hein
 *
 * Ensure all braces have an associated closing brace.  Braces need to have a matching close brace before
 * experiencing a different closing brace. (No interleaving of different brace types is allowed.)
 *
 * i.e.  {[}] would return false since the closing ']' is found before the necessary '}'
 *
 */

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

bool isCandidateOpen(char c) {
	switch (c) {
	case '{':
	case '[':
		return true;
	default:
		return false;
	}
}

bool isMatchingCandidate(char matchTo, char candidate) {
	switch (matchTo) {
	case '{':
		if (candidate == '}')
			return true;
		break;
	case '[':
		if (candidate == ']')
			return true;
	}
	return false;
}

const char* findOpenCandidate(const char *srcStr) {
	size_t length = 0;
	if (!srcStr)
		return (const char*) NULL;

	length = strlen(srcStr);
	while (--length > 0 && *srcStr != '\0') {
		if (isCandidateOpen(*srcStr)) {
			return srcStr;
		}
		srcStr++;
	}
	return (const char*) NULL;
}

/* input {[]] */
/* input {[]} */
/* input jfkd{ */
bool bracesComplete(const char *string) {
	bool matchFound = true;
	const char *currentCandidate = string;
    // Corner case, treating this as a match since there are no braces to compare.
    if (string == NULL)
    	return true;
	size_t length = strlen(string);
	const char *endOfString = &string[length];
	while ((currentCandidate = findOpenCandidate(currentCandidate)) != NULL) {
		matchFound = false;
		if (currentCandidate == endOfString)
			break;
		while (endOfString > currentCandidate) {
			matchFound = isMatchingCandidate(*currentCandidate, *endOfString);
			if (matchFound) {
				endOfString--;
				currentCandidate++;
				break;
			} else {
				endOfString--;
			}
		}
		if (matchFound == false)
			break;
	}
	return (matchFound);
}
