/*#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <uuid/uuid.h>
*/
#include "client_for_msg_queue.h"
#define PORT_FRONTEND 5050
//#define SERVER_IP_ADDR "127.0.0.1"
#define BUF_SIZE 256
#define UUID_SIZE_FOR_STR 32
#define FILE_BUF_SIZE 1024
#define DATA_DIR "data/"

void write_to_file_from_client(int confd, char* buff, char* filename);
void server_recive_json_file_and_send_uuid();   //Recive json file from the client and create uuid

