/* Buddy Bitmap is a fully populated binary tree that contains 0 or 1 in each node.
Invariant: a non-leaf node contains 1 if and only if both of its child nodes contain 1.

Here's an example of a tree:

Level 0:            0
               ___/   \___ 
Level 1:      0           0
            /   \       /   \
Level 2:   1     0     0     1
          / \   / \   / \   / \
Level 3: 1   1 1   0 0   0 1   1

Offset:  0   1 2   3 4   5 6   7      (at level 3)

Each node is identified by its level and index within the level.

The operations that are provided to you in a library are:
1) NUM_LEVELS                     # constant that has the number of levels (e.g. 4 in the example)
2) ClearBit(level, offset)        # assigns the value of the bit at the `level` and `offset` to 0.

Please implement:
ClearBits(offset, range)  # Clears `range` bits starting at `offset` at the last level 
*/

#include <cstdio>
#include <cstring>

#define NUM_LEVELS 4
bool debug = false;
typedef struct _bit {
  int level;
  int offset;
} bit_t;

bit_t bits_cleared[(1<<NUM_LEVELS)-1];
int bits_updated_count;

  
void ClearBit(int level, int offset) {
  if (debug)
	  printf("Cleared bit %d on level %d\n", offset, level);
  bits_cleared[bits_updated_count].level = level;
  bits_cleared[bits_updated_count].offset = offset;
  bits_updated_count++;
}

void ClearBits(int offset, int range) {
  int level = NUM_LEVELS-1;
  int endbit = offset+range > NUM_LEVELS*2 ? NUM_LEVELS*2 - 1 : offset+range-1;
  int bit;
  
  if (endbit < offset) return; 

  while (level >= 0)
  {
    bit = offset;
    while (bit <= endbit)
    {
       ClearBit(level, bit);
       bit++;
    }
    offset = offset>>1;
    endbit = endbit>>1;
    level--;
  }
} 


// THE FOLLOWING IS ALL UNIT_TEST CODE.  IDEALLY, IT WOULD BE IN A SEPARATE TEST FILE

void init_bits_cleared(void) 
{
  bits_updated_count = 0; 
  memset((void *)bits_cleared, 0xFF, (2^NUM_LEVELS-1) * sizeof(bit_t));
}
int pass_count = 0;
int fail_count = 0;
#define ASSERT_TRUE(x) if(x) pass_count++; else {fail_count++; printf("%s: %s failed\n", __FUNCTION__, #x);}

bool all_expectations_pass(bit_t *expected, int num_elements) 
{
  ASSERT_TRUE(bits_updated_count == num_elements);
  fail_count = 0;;
  for (int result_index = 0; result_index < num_elements; result_index++)
  {
    ASSERT_TRUE((bits_cleared[result_index].level == expected[result_index].level && 
                 bits_cleared[result_index].offset == expected[result_index].offset));
  }
  return(fail_count == 0);
}

int check_zero(void)
{
  bit_t expected_bits[] = {};
  init_bits_cleared();
  ClearBits(7,0);
  ASSERT_TRUE(bits_updated_count == 0);
  ASSERT_TRUE((bits_cleared[0].level == 0xFFFFFFFF &&
			 bits_cleared[0].offset == 0xFFFFFFFF));
  return all_expectations_pass(expected_bits, sizeof(expected_bits)/sizeof(bit_t));
}

int check_one(void)
{
  bit_t expected_bits[] = {{3,7}, {2,3}, {1,1}, {0,0}};
  init_bits_cleared();
  ClearBits(7,1);
  ASSERT_TRUE(bits_updated_count == 4);
  return all_expectations_pass(expected_bits, sizeof(expected_bits)/sizeof(bit_t));
}

int check_first(void)
{
  bit_t expected_bits[] = {{3,0}, {2,0}, {1,0}, {0,0}};
  init_bits_cleared();
  ClearBits(0,1);
  ASSERT_TRUE(bits_updated_count == 4);
  return all_expectations_pass(expected_bits, sizeof(expected_bits)/sizeof(bit_t));
}

int check_last(void)
{
  bit_t expected_bits[] = {{3,7}, {2,3}, {1,1}, {0,0}};
  init_bits_cleared();
  ClearBits(7,1);
  ASSERT_TRUE(bits_updated_count == 4);
  return all_expectations_pass(expected_bits, sizeof(expected_bits)/sizeof(bit_t));
}

int check_initial_index_out_of_bounds(void)
{
  bit_t expected_bits[] = {};
  init_bits_cleared();
  ClearBits(8,1);
  ASSERT_TRUE(bits_updated_count == 0);
  return all_expectations_pass(expected_bits, sizeof(expected_bits)/sizeof(bit_t));
}

int check_many_even(void)
{
  bit_t expected_bits[] = {{3,3}, {3,4}, {3,5}, {3,6},
                           {2,1}, {2,2}, {2,3},
                           {1,0}, {1,1},
                           {0,0}};
  init_bits_cleared();
  ClearBits(3,4);
  ASSERT_TRUE(bits_updated_count == 10);
  return all_expectations_pass(expected_bits, sizeof(expected_bits)/sizeof(bit_t));
}

int check_many_odd(void)
{
  bit_t expected_bits[] = {{3,2}, {3,3}, {3,4}, 
                           {2,1}, {2,2}, 
                           {1,0}, {1,1},
                           {0,0}};
  init_bits_cleared();
  ClearBits(2,3);
  ASSERT_TRUE(bits_updated_count == 8);
  return all_expectations_pass(expected_bits, sizeof(expected_bits)/sizeof(bit_t));
}

int check_all(void)
{
  bit_t expected_bits[] = {{3,0}, {3,1}, {3,2}, {3,3}, {3,4}, {3,5}, {3,6}, {3,7}, 
                           {2,0}, {2,1}, {2,2}, {2,3}, 
                           {1,0}, {1,1},
                           {0,0}};
  init_bits_cleared();
  ClearBits(0,8);
  ASSERT_TRUE(bits_updated_count == 15);
  return all_expectations_pass(expected_bits, sizeof(expected_bits)/sizeof(bit_t));
}

int check_bounds(void)
{
  bit_t expected_bits[] = {{3,0}, {3,1}, {3,2}, {3,3}, {3,4}, {3,5}, {3,6}, {3,7}, 
                           {2,0}, {2,1}, {2,2}, {2,3}, 
                           {1,0}, {1,1},
                           {0,0}};
  init_bits_cleared();
  ClearBits(0,20);
  ASSERT_TRUE(bits_updated_count == 15);
  return all_expectations_pass(expected_bits, sizeof(expected_bits)/sizeof(bit_t));
}

#define CHECK_RESULT(x) {bool result = false; result = x;  printf("%-40s: %s\n", #x, result ? "passed" : "FAILED");}
int main(void) {
  CHECK_RESULT(check_zero());
  CHECK_RESULT(check_one());
  CHECK_RESULT(check_many_even());
  CHECK_RESULT(check_many_odd());
  CHECK_RESULT(check_first());
  CHECK_RESULT(check_last());
  CHECK_RESULT(check_all());
  CHECK_RESULT(check_bounds());
  CHECK_RESULT(check_initial_index_out_of_bounds());
}

