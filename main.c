
#include <stdio.h>
#include "hexdump.h"

int main()
{
    char test[200];
    int cnt;

    for (cnt=0; cnt<100; cnt++)
    {
        test[cnt] = cnt;
    }

    hexdump(test, 100);
}
