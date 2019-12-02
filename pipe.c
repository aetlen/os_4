#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define R 0
#define W 1

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

int main(void) {
    cut_string()
    int p[2], q[2];
    FILE *fp;
    pipe(p);
    pipe(q);
    switch (fork()) {
        case -1: {
            perror("Ошибка при вызове fork() \n");
            exit(1);
        }
        case 0: {
            close(p[W]);
            if (dup2(p[R], R) == -1) {
                perror("Error with pipe 'p' in 'center_filter' for read");
            }
            close(p[R]);

            close(q[R]);
            if (dup2(q[W], W) == -1) {
                perror("Error with pipe 'q' in 'center_filter' for write");
            }
            close(q[W]);

            execl("output/format_string", "output/format_string", NULL);
            perror("center_filter: call error");
            exit(1);
        }
        default: {
            close(p[R]);
            close(q[W]);

            fp = fdopen(p[W], "w");

            const size_t MAX_STRING_LENGTH = 10000;
            char string[MAX_STRING_LENGTH];

            while (fgets(string, MAX_STRING_LENGTH, stdin) != NULL)
            {
                cut_string(string);
                fprintf(fp, "%s", string);
            }
            fclose(fp);

            fp = fdopen(q[R], "r");
            char letter;
            while ((letter = fgetc(fp)) != EOF)
                printf("%c", letter);
            fclose(fp);
            exit(0);
        }
    }
}
