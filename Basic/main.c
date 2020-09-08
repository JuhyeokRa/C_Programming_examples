#include <stdio.h>
#include <string.h>

int main(void)
{
    int data_length;
    char data[10] = {'h', 'a', 'p', 'p', 'y', 0,};

    data_length = strlen(data);

    printf("data lenght = %d\n", data_length);

    return 0;
}