#ifndef _HEXDUMP_H
#define _HEXDUMP_H

void ptrhexdump(FILE* output, char* pointer, unsigned long length);
void filehexdump(FILE* output, FILE* input, long offset, int whence,
                 unsigned long length);

#endif
