#include <stdio.h>
#include <ctype.h>
#include "hexdump.h"


void line(FILE* output, char* pointer, unsigned start, unsigned extra)
{
    unsigned cnt;
    unsigned char* pointerToPrint;
    char ch;

    for (cnt=(0x10-start); cnt<0x10; cnt++)
    {
        fprintf(output, "   ");
    }
    for (cnt=0; cnt<extra-start; cnt++)
    {
        pointerToPrint = (unsigned char*)(pointer+cnt);
        fprintf(output, "%02X ", *pointerToPrint);
    }
    for (cnt=extra; cnt<0x10; cnt++)
    {
        fprintf(output, "   ");
    }

    fprintf(output, "  ");
    for (cnt=(0x10-start); cnt<0x10; cnt++)
    {
        fprintf(output, " ");
    }
    for (cnt=0; cnt<extra-start; cnt++)
    {
        pointerToPrint = (unsigned char*)(pointer+cnt);
        ch = *pointerToPrint;
        if (!(isgraph(ch) || (ch == ' ')))
        {
            ch = '.';
        }
        fprintf(output, "%c", ch);
    }
    for (cnt=extra; cnt<0x10; cnt++)
    {
        fprintf(output, " ");
    }
}

void hexdump(FILE* output, char* pointer, unsigned long displayStart,
             unsigned long length)
{
    unsigned long position;
    unsigned long extra;
    unsigned long display;
    //unsigned start;
    unsigned spacesAtStart;

    spacesAtStart = displayStart & 0x0F;

    extra = length + spacesAtStart;
    if (extra > 0x10)
    {
        extra = 0x10;
    }

    fprintf(output, "%08lX  ", displayStart);
    line(output, pointer, spacesAtStart, (unsigned)extra);
    fprintf(output, "\n");

    for (position = extra; position<length; position+=0x10)
    {
        extra = length - position;
        if (extra > 0x10)
        {
            extra = 0x10;
        }

        display = displayStart + position - spacesAtStart;

        fprintf(output, "%08lX  ", display);
        line(output, pointer+position, 0, (unsigned)extra);
        fprintf(output, "\n");
    }
}

void ptrhexdump(FILE* output, char* pointer, unsigned long length)
{
    hexdump(output, pointer, 0, length);
}

void filehexdump(FILE* output, FILE* input, long offset, int whence,
                 unsigned long length)
{
    long currentPosition;
    char buffer[1024];
    unsigned long position;
    unsigned long displayOffset; /* Offset to start display at */
    unsigned bytesToRead;
    size_t bytesRead;

    currentPosition = ftell(input);
    fseek(input, offset, whence);

    position = 0;
    while (position < length)
    {
        displayOffset = ftell(input);
        bytesToRead = length - position;
        if (bytesToRead > 1024)
        {
            bytesToRead = 1024;
        }
        bytesRead = fread(buffer, 1, bytesToRead, input);
        hexdump(output, buffer, displayOffset, bytesRead);

        position += bytesRead;

        if (bytesRead < bytesToRead)
        {
            break;
        }
    }

    fseek(input, currentPosition, SEEK_SET);
}
