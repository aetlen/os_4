#include <stdlib.h>
#include <stdio.h>

void cut_string(char* S)
{
    for (int i = 0; i < 30; i++)
    {
        if (S[i] == '\0')
        {
            return;
        }
        else
        if (i == 29 && S[i] != '\0')
        {
            S[30] = '\0';
            return;
        }
    }
}


int main(int argc, char** argv)
{
    const int MAX_STRING_LENGTH = 10000;
    char string[MAX_STRING_LENGTH];

    while (fgets(string, MAX_STRING_LENGTH, stdin) != NULL)
    {
        cut_string(string);
        printf("%s", string);
    }

    return 0;
}
