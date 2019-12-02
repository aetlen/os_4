#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef __linux
#include <sys/io.h>
#else
#include <sys/uio.h>
#endif

#include <string.h>
#include <stdio.h>
#include "server.h"

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

int main(int argc, char **argv)
/*”named pipe” client.c*/
{
    struct message msg;
    int fdpub, fdpriv;
    char line[MAX_LINE_LENGTH];
    /* синтезировать имя личного программного канала и создать этот
    канал с правами чтения и записи для всех процессов*/
    sprintf(msg.privfifo, "Fifo%d", getpid());
    if (mkfifo(msg.privfifo, S_IFIFO | 0666) == -1)
    {
        perror (msg.privfifo);
        exit(1);
    }
    /* общедоступный именованный канал (создается перед запуском
    сервера %mkfifo public) открывается на запись, и в него записываются
    имена личного канала и требуемого файла */
    if ((fdpub=open(PUBLIC, O_WRONLY)) == -1)
    {
        perror(PUBLIC);
        exit(2);
    }
    char string[MAX_STRING_LENGTH];
    while(!feof(stdin))
    {
        fgets(string, MAX_STRING_LENGTH, stdin);
        if (!feof(stdin))
        {
            strcpy(msg.string, delete_repeats(string));
            //strcpy(msg.string, argv[1]);
            write(fdpub, (char*)&msg, sizeof(msg));
            // личный именованный канал открывается для чтения
            if ((fdpriv=open(msg.privfifo, O_RDONLY)) == -1)
            {
                perror(msg.privfifo);
                exit(3);
            }
            // распечатать данные, полученные из личного канала
            int n;
            while((n = read(fdpriv, line, MAX_LINE_LENGTH)) > 0)
                write(1, line, n);
            // личный именованный канал закрывается и удаляется из текущего каталога
            close (fdpriv);
        }
    }
    unlink (msg.privfifo);

    return 0;
}