#include "server_for_backend.h"

int init_socket(){
    int fd =0;
    struct sockaddr_in serv_addr;

    //char buff[FILE_BUF_SIZE + 1];
    //int num;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created\n");

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT_FRONTBACK_END);

    bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(fd, 10);
   
    return fd;
}

void server_send_json_file_to_backend(){
    
    int fd = init_socket();
    int confd, b=0, tot=0;

    char sendbuffer[BUF_SIZE];
    memset(sendbuffer, '0', sizeof(sendbuffer));

    //INIT for UUID
    char* uuid_str = malloc(UUID_SIZE_FOR_STR);
    char filename[strlen(uuid_str) + strlen(DATA_DIR) + strlen(".json") + 1];
    size_t filename_size = strlen(filename);
    
    while(1){
        confd = accept(fd, (struct sockaddr*)NULL, NULL);
        if (confd==-1) {
            perror("Accept");
            continue;
        }
        //Getting UUID
        if((b = recv(confd, uuid_str, UUID_SIZE_FOR_STR, 0)) < UUID_SIZE_FOR_STR)
            printf("UUID less then required size\n");
         
        printf("UUID recived -> %s\n", uuid_str); 
        strcpy(filename, DATA_DIR);
        strcat(filename, uuid_str);
        strcat(filename, ".json");
        printf("Filename -> %s and len-> %d\n", filename, strlen(filename));
        
        FILE* fp = fopen(filename, "rb");
        tot=0;
        if(fp != NULL){
            while((b = fread(sendbuffer, 1, sizeof(sendbuffer), fp)) > 0){
                send(confd, sendbuffer, b, 0);
            }
            if (fclose(fp)) { printf("error closing file."); exit(-1); }
            printf("No error in closing file or sending file\n");
        } else {
            //perror("File");
            printf("ERROR in FILE\n");
        }

        printf("UUID Recived -> %s\n", uuid_str);
         
        close(confd);
    }

}

int main(int argc, char** argv){
    server_send_json_file_to_backend();
}
