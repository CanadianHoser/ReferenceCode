/**
 *******************************************************************************
 *
 * @file seq_decode.c
 *
 * @brief Basic alpha decompression algorithm
 *
 *******************************************************************************
 */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define EXPANSION_ERROR -1
#define EXPANSION_OUT_OF_RANGE -2


char const *find_next_decompress_token(char const *comp_str, size_t *location)
{
	char const *token = comp_str;

	*location = 0;

	if (isdigit(*comp_str)) return "\0";
	while((*token != '\0')) {
		if (isdigit(*(token+1)))
		{
			break;
		} else {
			(*location)++;
		}
		*(token++);
	}
	if (*token == '\0')
		return NULL;
	else
		return(token);
}

int expandBy(char const *comp_str)
{
	int expandByVal = 0;
	// first char is alpha
	char token =  *(comp_str++);
	if (!isalpha(token))
		return EXPANSION_ERROR;
	while (isdigit(*comp_str)) {
		expandByVal += *comp_str - '0';
	    if (isdigit(*(comp_str+1))) {
	    	expandByVal*=10;
	    }
	    *(comp_str++);
	}
	if (!(isalpha(token+expandByVal-1)))
			return EXPANSION_OUT_OF_RANGE;
    return(expandByVal);
}

char * expandBuffer(char *buffer, unsigned int bufferSize, char const *comp_str)
{
	int digitsToExpand = expandBy(comp_str);
	int bufIndex = 0;
	char charToExpand;

	if ((digitsToExpand < 0) || (bufferSize == 0))
		return (char *)NULL;

	charToExpand = comp_str[0];
    bufferSize--;
	buffer[bufIndex++] = comp_str[0]; // token key
	// Now fill in the rest of the digits
	while (--digitsToExpand && --bufferSize > 0) buffer[bufIndex++] = ++charToExpand;
	buffer[bufIndex] = '\0';
	return(buffer);
}

/**
 *******************************************************************************
 * @brief Decompress incoming string.
 * ***[TODO] Please define expectations or limitations:
 * CURRENT_LIMITATIONS: This does not yet properly guarantee against
 *                      buffer overrun for all cases
 *
 * Example:      "a3g4j4g4d"
 * Output:       abcghijjklmghijd
 * Visual aid:   a[bc] g[hij] j[klm] g[hij] d
 *
 * @param[in] comp_str incoming string in compressed format
 * @param[in,out] output_buffer buffer for decompressed string
 * @param[in] output_buffer_size size of output buffer.
 *
 * @return pointer to decompressed buffer. NULL on failure
 *******************************************************************************
 */
char *decompress(char const *comp_str, char *output_buffer,
    size_t output_buffer_size)
{
	char const *token = comp_str;
	char const *next_token;
	char expansion_buffer[32]; // Really, only 26 are needed for the worst case
	size_t location_in_token = 0;
	size_t output_buffer_len = output_buffer_size;
	size_t token_decompressed_length = 0;
	memset(output_buffer, 0, output_buffer_size);

    do {
    	if((next_token = find_next_decompress_token(token, &location_in_token)))
    	{
    		// Copy over data that doesn't need to be decompressed from the start of the token
    		if (location_in_token < output_buffer_len) {
	    		strncat(output_buffer, token, location_in_token);
	    		output_buffer_len -= location_in_token;
    		} else {
    			// We should never see this
    			return NULL;
    		}

    		expandBuffer(expansion_buffer, 32, next_token);
    		token_decompressed_length = strlen(expansion_buffer);

    		if(token_decompressed_length > output_buffer_len)
    		{
    			return NULL;
    		}
    		strncat(output_buffer, expansion_buffer, token_decompressed_length);
    		output_buffer_len -= token_decompressed_length;
    		// Fast forward past the token and any trailing digits
    		while(isdigit(*(++next_token)));
    		token = next_token;
    	} else {
    		if (location_in_token < output_buffer_len) {
			    strncat(output_buffer, token, output_buffer_len);
    		} else {
    			return NULL;
    		}
    	}
    } while (next_token != NULL);
    return (output_buffer);
}

int my_main()
{
    char decompress_this[] = "c3j3d8js3bsj2k4bo3k";
    char buffer[128];

    // Decompress the given string.
    decompress(decompress_this, buffer, 128);
    return 0;
}

#if !defined(UNIT_TEST)
int main() {
	return my_main();
}
#endif
