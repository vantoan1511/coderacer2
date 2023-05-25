#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LENGTH 1000U
#define DEFAULT_VALUE 0xAAU
#define HEADER_SIZE sizeof(int)
#define GET_DATA_BUFFER(_buffer) (&_buffer[HEADER_SIZE])

int get_array_element(const char *array, int array_length, int index)
{
    if (index > array_length)
    {
        return 0;
        printf("Out of bound access");
    }

    return array[index];
}

/** Allocate a buffer on heap, the first integer is used to store buffer size
 * @param[in]   size    Size of to be allocated buffer in bytes
 * @return Pointer to allocated buffer
 */
char *allocate_buffer(int size)
{
    char *buffer = (char *)malloc(size + HEADER_SIZE);
    unsigned char i;

    int *buffer_size = (int *)buffer;
    *buffer_size = size;
    for (i = HEADER_SIZE; i < size + HEADER_SIZE; i++)
    {
        buffer[(unsigned int)i] = DEFAULT_VALUE;
    }

    return buffer + HEADER_SIZE;
}

/** Deallocate a buffer previously allocated by @ref allocate_buffer
 * @param[in]   size    Size of to be allocated buffer in bytes
 * @return Pointer to allocated buffer
 */
void deallocate_buffer(char *buffer)
{
    free(buffer - HEADER_SIZE);

    /* Always clear allocated buffer to prevent sensitive data leakage */
    int buffer_size = *((int *)buffer);
    char i;
    for (i = 0; i < buffer_size; i++)
    {
        buffer[(unsigned int)i] = DEFAULT_VALUE;
    }
}

void check_overflow(void)
{
    volatile int important_data = 0;
    char user_input[10];

    gets(user_input);

    if (important_data != 0)
    {
        printf("Warning !!!, the 'important_data' was changed\n");
    }
    else
    {
        printf("the 'important_data' was not changed\n");
    }
}

void access_array(void)
{
    char buffer[10];
    int r;
    r = get_array_element(buffer, 10, 100);
    printf("r = %d\n", r);
}

// just a function with multiple paths that can be discoverd by a fuzzer
void exploreMe(int a, int b, string c)
{
    printf("a: %d; b: %d; c: %s\n", a, b, c.c_str());

    if (a >= 20000)
    {
        if (b >= 2000000)
        {
            if (b - a < 100000)
            {
                // Trigger the undefined behavior sanitizer
                int n = 23;
                n <<= 32;
                if (c == "FUZZING")
                {
                    // Trigger a heap buffer overflow
                    char *s = (char *)malloc(1);
                    strcpy(s, "too long");
                }
            }
        }
    }
}

int main(void)
{
    char *array = allocate_buffer(ARRAY_LENGTH);
    int result = get_array_element(array, ARRAY_LENGTH, ARRAY_LENGTH);

    deallocate_buffer(array);

    check_overflow();

    access_array();

    return result;
}
