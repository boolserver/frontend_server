#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
//#include <uuid/uuid.h>

#define PORT_FRONTBACK_END 5500
#define BUF_SIZE 256
#define UUID_SIZE_FOR_STR 32
#define FILE_BUF_SIZE 1024
#define DATA_DIR "data/"

void server_send_json_file_to_backend();

