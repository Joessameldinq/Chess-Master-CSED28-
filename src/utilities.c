#include <string.h>
#include "../include/utilities.h"
void toLowerString(char *s)
{
    int n = strlen(s);
    for(int i = 0;i  < n;i++)
    {
        if(s[i] >='A' && s[i] <='Z')
            s[i] = s[i] + ('a' - 'A');
    }
}