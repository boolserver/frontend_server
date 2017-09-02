#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <uuid/uuid.h>

// For server
#include <netdb.h>
//#include "client_for_msg_queue.h"

// For testing 
//#include "../message_queue/msg_queue.h"

#define SERVER_IP_ADDR "127.0.0.1"
#define PORT 5000
#define UUID_SIZE_FOR_STR 32

int init_sockfd();
void send_uuid_str_to_msg_queue(char *uuid_str);
