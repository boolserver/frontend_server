#include "server_for_msg_queue.h"

int init_socket(){
    int listenfd = socket(AF_INET, SOCK_STREAM, 0); 

    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    if(listen(listenfd, 10) == -1){printf("\nERROR: Failed to listen\n"); return -1;}

    return listenfd;
}

void send_uuid_str_to_msg_queue(char *uuid_str){
    int listenfd = init_socket();
    printf("Server set up\n");
    int connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    printf("Listening for client request....\n");
    //sending uuid_str
    write(connfd, uuid_str, UUID_SIZE_FOR_STR);
    printf("UUID writen\nClosing connection\n");
    sleep(1);
    close(connfd);
    sleep(1);
}

int main(int argc, char** argv){
    uuid_t uuid;
    uuid_generate_random(uuid);

    char* uuid_str = malloc(UUID_SIZE_FOR_STR);
    uuid_to_str(uuid, uuid_str);

    printf("This is the uuid str -> %s\nSending UUID to client\n", uuid_str);

    send_uuid_str_to_msg_queue(uuid_str);
}


