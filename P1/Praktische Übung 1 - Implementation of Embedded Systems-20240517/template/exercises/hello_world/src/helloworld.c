#include <stdio.h>
#include <stdint.h>

#include <educore-v.h>
#include <utils/delay.h>

int main(void)
{
    printf("Hello World!\n");
    mmio->led=0x0000;
    while (1)
    {
        mmio->led++;
        _delay_ms(1);
    }
}

