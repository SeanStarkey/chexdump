
#include <stdio.h>
#include <ctype.h>
#include "hexdump.h"


void line(void* pointer)
{
    int cnt;
    unsigned char* pointerToPrint;
    char ch;

    for (cnt=0; cnt<0x10; cnt++)
    {
        pointerToPrint = (unsigned char*)(pointer+cnt);
        printf("%02X ", *pointerToPrint);
    }

    printf("  ");
    for (cnt=0; cnt<0x10; cnt++)
    {
        pointerToPrint = (unsigned char*)(pointer+cnt);
        ch = *pointerToPrint;
        if (!(isgraph(ch) || (ch == ' ')))
        {
            ch = '.';
        }
        printf("%c", ch);
    }
}

void hexdump(void* pointer, long length)
{
    unsigned long position = 0;

    for (position = 0; position<100; position+=0x10)
    {
        printf("%08lX  ", position);
        line(pointer+position);
        printf("\n");
    }
}


