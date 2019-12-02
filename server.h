#ifndef SERVER
#define SERVER


#define MAX_STRING_LENGTH 10000
#define MAX_LINE_LENGTH  100


struct message
{
    char privfifo[15];
    // имя частного именованного канала
    char string[MAX_STRING_LENGTH];
    // имя запрошенного файла
};
#define PUBLIC "public"
// имя общедоступного именованного программного канала
#define LINESIZE 512

#endif