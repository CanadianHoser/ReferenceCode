#ifdef DESCRIPTION
Method read4: 

The API read4 reads 4 consecutive characters from the file, then writes those characters into the buffer array buf.

The return value is the number of actual characters read.

Note that read4() has its own file pointer, much like FILE *fp in C.

Definition of read4:

    Parameter:  char[] buf
    Returns:    int

Note: buf[] is destination not source, the results from read4 will be copied to buf[]
Below is a high level example of how read4 works:

File file("abcdefghijk"); // File is "abcdefghijk", initially file pointer (fp) points to 'a'
char[] buf = new char[4]; // Create buffer with enough space to store characters
read4(buf); // read4 returns 4. Now buf = "abcd", fp points to 'e'
read4(buf); // read4 returns 4. Now buf = "efgh", fp points to 'i'
read4(buf); // read4 returns 3. Now buf = "ijk", fp points to end of file
 

Method read:

By using the read4 method, implement the method read that reads n characters from the file and store it in the buffer array buf. Consider that you cannot manipulate the file directly.

The return value is the number of actual characters read.

Definition of buf_read:

    Parameters:  char[] buf, int n
    Returns:  int

Note: buf[] is destination not source, you will need to write the results to buf[]
 

Example 1:

File file("abc");
Solution sol;
// Assume buf is allocated and guaranteed to have enough space for storing all characters from the file.
buf_read(buf, 1); // After calling your read method, buf should contain "a". We read a total of 1 character from the file, so return 1.
buf_read(buf, 2); // Now buf should contain "bc". We read a total of 2 characters from the file, so return 2.
buf_read(buf, 1); // We have reached the end of file, no more characters can be read. So return 0.
Example 2:

File file("abc");
Solution sol;
buf_read(buf, 4); // After calling your read method, buf should contain "abc". We read a total of 3 characters from the file, so return 3.
buf_read(buf, 1); // We have reached the end of file, no more characters can be read. So return 0.
/* Calls from the Applciation 
  buf_read(A, 10);
  buf_read(B, 255);
  buf_read(C, 1);
  buf_read(D, 5);
*/

#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>

static char last_read[4];
static unsigned int last_read_chars_remaining = 0;
static unsigned int read_count = 0;
extern unsigned int read4(char *);

void reset_read_buffer(void)
{
  last_read_chars_remaining = 0;
  read_count = 0;
}

unsigned int buf_read(char* buff, unsigned int n)
{
  unsigned int total_count = 0;
  // Account for any remaining characters from previous read
  if (last_read_chars_remaining) {
    total_count = (n >= last_read_chars_remaining) ? last_read_chars_remaining : n;
    memcpy(buff, &last_read[read_count-last_read_chars_remaining], total_count);;
    last_read_chars_remaining-=n;
    n -= total_count;
  }
  while (n > 0) {
    read_count = read4(last_read);
    last_read_chars_remaining = read_count;
    if (n < read_count) {
      memcpy(&buff[total_count], last_read, n);
      last_read_chars_remaining -= n;
      total_count+=n;
      n = 0;
    } else {
      memcpy(&buff[total_count], last_read, read_count);
      last_read_chars_remaining = 0;
      total_count+=read_count;
      n-=read_count;
      if (read_count<4)
        break;
    }
  }
  buff[total_count] = '\0';
  return (total_count);
}
