#include "os.h"

void memcpy(void *dest, const void *src, uint8_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        ((char*)dest)[i] = ((char*)src)[i];
    }

}
