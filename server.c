#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "server.h"

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


int main(int argc, char **argv)
{
    int fd, fdpub, fdpriv;
    struct message msg;
    /*структура сообщения, которое читается из
    общедоступного именованного канала*/
    while(1) /* бесконечный цикл */
    {
        /* Общедоступный канал открывается для чтения. Системный вызов
        open() блокируется, если никакой процесс-клиент не открывает другой
        конец канала*/
        if ((fdpub=open(PUBLIC, O_RDONLY)) == -1)
        {
            perror(PUBLIC);
            exit(1);
        }
        /* Из общедоступного канала читается сообщение, содержащее имена
        личного канала и */
        /* требуемого файла. Требуемый файл открывается для чтения. */
        while(read(fdpub, (char*)&msg, sizeof(msg)) > 0)
        {
            int lines_num = count_lines(msg.string, MAX_LINE_LENGTH);
            char** lines = format_string(msg.string, MAX_LINE_LENGTH);
            /* Личный канал открывается для записи. Файловый сервер спит, пока
            клиент не откроет свой конец личного канала. Если возникли
            проблемы, и клиент не может открыть канал, сервер повиснет внутри
            этого open(). !см. соответствующие ключи, предотвращающие
            блокировки. */
            if ((fdpriv=open(msg.privfifo, O_WRONLY)) == -1)
            {
                perror(msg.privfifo);
                break;
            }
            /* данные из файла копируются в личный именованный канал */
            cut_string(msg.string);
            write(fdpriv, msg.string, MAX_STRING_LENGTH);
            /* Когда копирование закончено, требуемый файл и личный
            именованный канал закрывается */
            close(fd);
            close(fdpriv);
        }
        /* end of first while() */
        /* Закрыть общедоступный канал. Сервер переходит к следующему
        витку цикла. Причина для закрытия и повторного открытия состоит в
        том, что хочется, чтобы файловый сервер спал на open(), ожидая
        запросов. */
        close (fdpub);
    }
    return 0;
}
/* !Программа никогда не завершается */
