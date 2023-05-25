#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "crc32.h"
#include <string.h>

#define TIMEOUT 600UL
#define MIN_LENGTH_BYTES 6UL

typedef struct
{
    unsigned long size;
    unsigned long time;
    unsigned long reserved[10];
} buffer_header_t;

/**
 * @brief Get the array element object
 *
 * @param[in]  array         Array
 * @param[in]  array_length  Length of the array
 * @param[in]  index         Index of element to retrieve
 * @param[out] element       Element value output pointer
 * @return int
 *  0       No error
 *  else    Error
 */
int get_array_element(const unsigned long array[], int array_length, int index, unsigned long* element)
{
    if (index > array_length)
    {
        printf("ERROR: Out of bound access");
        return -1;
    }

    *element = array[index];

    return 0;
}

/**
 * @brief Allocate a buffer on heap
 *
 * This function calls malloc to allocate heap buffer, the begining of buffer (header) is used to store metadata
 * Metadata structure is defined by @ref buffer_header_t
 *
 * @param[in]   size    Size of to be allocated buffer in bytes
 * @return Pointer to usable buffer
 */
void * allocate_buffer(unsigned long size)
{
    char* buffer = (char *) malloc(size + sizeof(buffer_header_t));
    unsigned char i;

    buffer_header_t * header = (buffer_header_t *)buffer;
    header->size = size;
    header->time = (unsigned long)time(NULL);

    for (i = sizeof(buffer_header_t); i < size + sizeof(buffer_header_t); i++)
    {
        buffer[(unsigned long)i] = 0;
    }

    return buffer + sizeof(buffer_header_t);
}

/**
 * @brief Deallocate a buffer previously allocated by @ref allocate_buffer
 * Buffer can only be deallocated within 600 seconds after being allocated
 * Deallocated memory is cleared to prevent sensitive data leakage
 * @param[in] buffer    Previously allocated buffer
 * @return int
 *      0       No error
 *     -1       Buffer couldn't be deallocated
 */
int deallocate_buffer(void * buffer)
{
    buffer_header_t * header = (buffer_header_t *)((char*)buffer -  sizeof(buffer_header_t));
    unsigned long current_time = (unsigned long)time(NULL);

    if ((current_time - header->time) > TIMEOUT)
    {
        return -1;
    }

    free(header);

    /* Always clear allocated buffer to prevent sensitive data leakage */
    unsigned char i;
    for (i = 0; i < header->size; i++)
    {
        ((char*)(buffer))[(unsigned long)i] = 0;
    }

    return 0;
}

/**
 * @brief Calculate fingerprint of a given text message
 *
 * Fingerprint = CRC32(UID|COUNTER|DATA0...DATAN)
 * Byte array length must be at least @ref MIN_LENGTH_BYTES
 *
 * @param[in]  uid           Unique ID
 * @param[in]  counter       Message counter
 * @param[in]  text          Pointer to text message
 * @param[out] output        Fingerprint output
 *
 * @return int
 *   0      No error
 *  -1      Null pointer
 *  -2      Byte array is not long enough
 */
int calculate_fingerprint(unsigned long uid, unsigned long counter, const char *text, unsigned int* output)
{
    char* workspace = (char*)malloc(sizeof(uid) + sizeof(counter) + strlen(text) + 1); // plus 1 for null character

    if (text == (void*)0)
    {
        return -1;
    }

    if (strlen(text) < MIN_LENGTH_BYTES)
    {
        return -2;
    }

    (void)memcpy(workspace, &uid, sizeof(uid));
    (void)memcpy(&workspace[sizeof(uid)], &counter, sizeof(counter));
    (void)memcpy(&workspace[sizeof(uid) + sizeof(counter)], text, strlen(text));
    workspace[sizeof(uid) + sizeof(counter) + strlen(text)] = 0;

    *output = crc32((unsigned char*)workspace);

    free(workspace);

    return 0;
}
