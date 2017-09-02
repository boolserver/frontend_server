#include "client_for_msg_queue.h"

int init_sockfd(){ 
    int sockfd = 0;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
        printf("\nERROR: Could not create socket\n");
        return -1; 
    }   
                
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ 
        printf("\nERROR : Connect Failed\n\n");
        return -1; 
    }   

    return sockfd;
}

void send_uuid_str_to_msg_queue(char *uuid_str){
    int sockfd = init_sockfd();
    printf("Server set up\n");
    write(sockfd, uuid_str, UUID_SIZE_FOR_STR);
    printf("UUID writen\nClosing connection\n");
    sleep(1);
    close(sockfd);
    sleep(1);
}
/*
int main(int argc, char** argv){
    uuid_t uuid;
    uuid_generate_random(uuid);

    char* uuid_str = malloc(UUID_SIZE_FOR_STR);
    uuid_to_str(uuid, uuid_str);

    printf("This is the uuid str -> %s\nSending UUID to client\n", uuid_str);

    send_uuid_str_to_msg_queue(uuid_str);
}
*/

