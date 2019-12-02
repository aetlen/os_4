#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


void main(void)
{
    int pid, pid2;
    int fd[2];
    int status, dead;
    switch (pid = fork())
    {
        case -1:
        {
            /* Cбой при вызове fork() */
            printf("Ошибка при вызове fork() #1 \n");
            exit(1);
        }
        case 0:
            /* ПОТОМОК #1 */
        {
            pipe(fd);
            switch (pid2 = fork())
            {
                case -1:
                    /* Cбой при вызове fork() */
                {
                    printf("Ошибка при вызове fork() #2 \n");
                    exit(2);
                }
                case 0:
                {
                    /* ПОТОМОК #2 */
                    close(fd[1]);
                    dup2(fd[0], 0);
                    execl("./cut_string", "./cut_string", NULL);
                    puts("Ошибка при вызове 'cut_string'\n");
                    exit(2);
                }
                default:
                {
                    close(fd[0]);
                    dup2(fd[1], 1);
                    execl("./delete_repeats", "./delete_repeats", NULL);
                    puts("Ошибка при вызове 'delete_repeats'\n");
                    exit(2);
                }
            }
        }
        default: /* ПРЕДОК ГЛАВНЫЙ */
        {
            dead = wait( & status);
            exit(1);
        }
    }
}
