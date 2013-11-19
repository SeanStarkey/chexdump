/*
 * hexdump.c
 *
 * Created by Sean Starkey <sean@seanstarkey.com>
 *
 * NO COPYRIGHT - THIS IS 100% IN THE PUBLIC DOMAIN
 *
 * The original version is available at:
 *    https://github.com/SeanStarkey/chexdump
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR(S) OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include "hexdump.h"


static FILE* outputfile;
static char* outputbuffer;
static unsigned long outputbufferoffset;

void output(const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    if (outputfile != NULL) {
        vfprintf(outputfile, format, arguments);
    }
    if (outputbuffer != NULL) {
        outputbufferoffset +=
            vsprintf(outputbuffer+outputbufferoffset, format, arguments);
    }
    va_end(arguments);
}


void line(unsigned char* pointer, unsigned start, unsigned extra)
{
    unsigned cnt;
    unsigned char* pointerToPrint;
    unsigned char ch;

    for (cnt=(0x10-start); cnt<0x10; cnt++)
    {
        output("   ");
    }
    for (cnt=0; cnt<extra-start; cnt++)
    {
        pointerToPrint = (unsigned char*)(pointer+cnt);
        output("%02X ", *pointerToPrint);
    }
    for (cnt=extra; cnt<0x10; cnt++)
    {
        output("   ");
    }

    output("  ");
    for (cnt=(0x10-start); cnt<0x10; cnt++)
    {
        output(" ");
    }
    for (cnt=0; cnt<extra-start; cnt++)
    {
        pointerToPrint = (unsigned char*)(pointer+cnt);
        ch = *pointerToPrint;
        if (!(isgraph(ch) || (ch == ' ')))
        {
            ch = '.';
        }
        output("%c", ch);
    }
    for (cnt=extra; cnt<0x10; cnt++)
    {
        output(" ");
    }
}

void hexdump(unsigned char* pointer, unsigned long displayStart,
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

    output("%08lX  ", displayStart);
    line(pointer, spacesAtStart, (unsigned)extra);
    output("\n");

    for (position = extra; position<length; position+=0x10)
    {
        extra = length - position;
        if (extra > 0x10)
        {
            extra = 0x10;
        }

        display = displayStart + position - spacesAtStart;

        output("%08lX  ", display);
        line(pointer+position, 0, (unsigned)extra);
        output("\n");
    }
}

void ptrhexdumpToFile(FILE* output,
                      unsigned char* input, unsigned long inputLength)
{
    outputfile = output;
    outputbuffer = NULL;
    hexdump(input, 0, inputLength);
}

void ptrhexdumpToStr(char* output, unsigned long outputLength,
                     unsigned char* input, unsigned long inputLength)
{
    outputfile = NULL;
    outputbuffer = output;
    outputbufferoffset = 0;
    hexdump(input, 0, inputLength);
}

void filehexdumpToFile(FILE* output, FILE* input, long offset, int whence,
                 unsigned long inputLength)
{
    long currentPosition;
    unsigned char buffer[1024];
    unsigned long position;
    unsigned long displayOffset; /* Offset to start display at */
    unsigned bytesToRead;
    size_t bytesRead;

    outputfile = output;
    outputbuffer = NULL;

    currentPosition = ftell(input);
    fseek(input, offset, whence);

    position = 0;
    while (position < inputLength)
    {
        displayOffset = ftell(input);
        bytesToRead = inputLength - position;
        if (bytesToRead > 1024)
        {
            bytesToRead = 1024;
        }
        bytesRead = fread(buffer, 1, bytesToRead, input);
        hexdump(buffer, displayOffset, bytesRead);

        position += bytesRead;

        if (bytesRead < bytesToRead)
        {
            break;
        }
    }

    fseek(input, currentPosition, SEEK_SET);
}

void filehexdumpToStr(char* output, unsigned long outputLength,
                      FILE* input, long offset, int whence,
                      unsigned long inputLength)
{
    long currentPosition;
    unsigned char buffer[1024];
    unsigned long position;
    unsigned long displayOffset; /* Offset to start display at */
    unsigned bytesToRead;
    size_t bytesRead;

    outputfile = NULL;
    outputbuffer = output;
    outputbufferoffset = 0;

    currentPosition = ftell(input);
    fseek(input, offset, whence);

    position = 0;
    while (position < inputLength)
    {
        displayOffset = ftell(input);
        bytesToRead = inputLength - position;
        if (bytesToRead > 1024)
        {
            bytesToRead = 1024;
        }
        bytesRead = fread(buffer, 1, bytesToRead, input);
        hexdump(buffer, displayOffset, bytesRead);

        position += bytesRead;

        if (bytesRead < bytesToRead)
        {
            break;
        }
    }

    fseek(input, currentPosition, SEEK_SET);
}
