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
    char frontend_or_msgqueue = 'f';
    write(sockfd, &frontend_or_msgqueue, sizeof(char));
    write(sockfd, uuid_str, UUID_SIZE_FOR_STR);
    printf("UUID writen\nClosing connection\n");
    sleep(1);
    close(sockfd);
    sleep(1);
}
/*
void uuid_to_str(uuid_t uuid, char* str_ptr){
    sprintf(
        str_ptr, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", 
        uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
        uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]
    );  
}

int main(int argc, char** argv){
    uuid_t uuid;
    uuid_generate_random(uuid);

    char* uuid_str = malloc(UUID_SIZE_FOR_STR);
    uuid_to_str(uuid, uuid_str);

    printf("This is the uuid str -> %s\nSending UUID to client\n", uuid_str);

    send_uuid_str_to_msg_queue(uuid_str);
}
*/

