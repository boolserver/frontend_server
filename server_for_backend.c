#include "server_for_backend.h"
#include "serverside_for_clients.h"

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
/*
void concatinate_string(char* filename, char* dir, char* uuid, char* ext){
    strcpy(filename, dir);
    strcat(filename, uuid);
    strcat(filename, ext);
}*/

void server_send_json_file_to_backend(){
    
    int fd = init_socket();
    int confd, b=0, tot=0;

    char buffer[BUF_SIZE];
    memset(buffer, '0', sizeof(buffer));

    //INIT for UUID
    char* uuid_str = malloc(UUID_SIZE_FOR_STR);
    char filename[strlen(uuid_str) + strlen(DATA_DIR) + strlen(".json") + 1];
    char output_filename[strlen(uuid_str) + strlen(DATA_DIR) + strlen(".txt") + 1];
    size_t filename_size = strlen(filename);
    size_t output_filename_size = strlen(output_filename);
    
    char mode_json_or_result;

    while(1){
        confd = accept(fd, (struct sockaddr*)NULL, NULL);
        if (confd==-1) {
            perror("Accept");
            continue;
        }
        
        //Getting Mode
        if((b = read(confd, &mode_json_or_result, 1)) != 1)
            printf("\nMode specified worngly\n\n");
        
        //Getting UUID
        if((b = recv(confd, uuid_str, UUID_SIZE_FOR_STR, 0)) < UUID_SIZE_FOR_STR)
            printf("UUID less then required size\n");
         
        printf("UUID recived -> %s\n", uuid_str); 
        
        if(mode_json_or_result == 'j'){
            printf("\nThis is json file giver\n");
            strcpy(filename, DATA_DIR);
            strcat(filename, uuid_str);
            strcat(filename, ".json");
            printf("Filename -> %s and len-> %d\n", filename, strlen(filename));
            
            read_file_and_send_to_client(confd, (char *)&buffer, (char *)&filename);
            printf("UUID Recived -> %s\n", uuid_str);
        }
        else if(mode_json_or_result == 'r'){
            printf("\nThis will be storing files\n");
            strcpy(output_filename, DATA_DIR);
            strcat(output_filename, uuid_str);
            strcat(output_filename, ".txt");
            printf("Output Filenam -> %s\n", output_filename);

            write_to_file_from_client(confd, (char *)&buffer, (char *)&output_filename); 
            printf("Rsult Stored on frontend\n");
        }
        close(confd);
    }

}

int main(int argc, char** argv){
    server_send_json_file_to_backend();
}
