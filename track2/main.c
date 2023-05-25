#include <stdio.h>
#include <string.h>

int get_array_element(const unsigned long array[], int array_length, int index, unsigned long* element);
void * allocate_buffer(unsigned long size);
int deallocate_buffer(void * buffer);
int calculate_fingerprint(unsigned long uid, unsigned long counter, const char *text, unsigned int* output);

int main(void)
{
    char* buffer = allocate_buffer(100);

    printf("OK: allocate_buffer\r\n");
    const char* text = "Hello world";
    strcpy(buffer, text);

    unsigned int fingerprint = 0;
    if (0 != calculate_fingerprint(0xAA, 2, buffer, &fingerprint))
    {
        printf("ERROR: calculate_fingerprint");
        return -1;
    }

    if (0xe401a57b != fingerprint)
    {
        printf("ERROR: Incorrect fingerprint ");
        return -1;
    }
    printf("OK: calculate_fingerprint\r\n");

    if (0 != deallocate_buffer(buffer))
    {
        printf("ERROR: deallocate_buffer");
        return -1;
    }
    printf("OK: deallocate_buffer\r\n");

    printf("Fingerprint=%x\r\n", fingerprint);

    return 0;
}
