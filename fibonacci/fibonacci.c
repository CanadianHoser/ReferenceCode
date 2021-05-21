#include <stdio.h>

unsigned int fib(unsigned int n) 
{
	unsigned int first = 1;
    unsigned int second = 1;
    unsigned int result = 0;

    if (0 < n && n <= 2) return 1;
    for (int iteration = 2; iteration < n; iteration++)
	{
		result = first+second;
		first = second;
		second = result;
	}
	return result;
}

unsigned int pass_count = 0;
unsigned int fail_count = 0;
#define ASSERT_TRUE(x) if(x) pass_count++; else printf("%s failed\n", #x);
#define ASSERT_FALSE(x) if(!x) pass_count++; else printf("%s failed\n", #x);

int main(void) 
{
    ASSERT_TRUE(fib(4) == 2);
    ASSERT_TRUE(fib(4) == 3);
    ASSERT_TRUE(fib(5) == 5);
    for (unsigned int i = 0; i<30; i++)
	{
		printf("result of fib(%d) = %d\n", i, fib(i));
	}
}

    
