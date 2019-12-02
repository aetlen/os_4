#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void delete_repeats(char* S)
{
    int start_len = strlen(S);
    int end_len = start_len;
    for (int i = 0; i < end_len; i++)
    {
        if (S[i] == '\0' && start_len == end_len)
        {
            return;
        }
        else
        if (S[i] == S[i+1])
        {
            for (int j = i+1; j < end_len; j++)
            {
                S[j] = S[j+1];
            }
            end_len--;
            i--;
        }
    }
    S[end_len]='\0';
}

int main(int argc, char** argv)
{
    const int MAX_STRING_LENGTH = 10000;
    char string[MAX_STRING_LENGTH];

    while (fgets(string, MAX_STRING_LENGTH, stdin) != NULL)
    {
        delete_repeats(string);
        printf("%s", string);
    }

    return 0;
}
