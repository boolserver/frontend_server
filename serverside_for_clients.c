#include "serverside_for_clients.h"

void uuid_to_str(uuid_t uuid, char* str_ptr){
    sprintf(
        str_ptr, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", 
        uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
        uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]
    );
}

void server_recive_json_file_and_send_uuid(){
    int fd =0, confd = 0,b,tot;
    struct sockaddr_in serv_addr;

    char buff[FILE_BUF_SIZE + 1];
    int num;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buff, '0', sizeof(buff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT_FRONTEND);

    bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(fd, 10);

    //INIT for UUID
    uuid_t uuid;
    char* uuid_str = malloc(UUID_SIZE_FOR_STR);
    char* filename = malloc(strlen(uuid_str) + strlen(DATA_DIR) + strlen(".json"));
    size_t filename_size = sizeof(filename);
    //strcpy(filename, DATA_DIR);
    //strcpy(filename, uuid_str);

    while(1){
        confd = accept(fd, (struct sockaddr*)NULL, NULL);
        if (confd==-1) {
            perror("Accept");
            continue;
        }
        //Generating UUID
        uuid_generate_random(uuid);
        uuid_to_str(uuid, uuid_str);
        printf("UUID created -> %s\n", uuid_str); 
        strcpy(filename, DATA_DIR);
        strcat(filename, uuid_str);
        strcat(filename, ".json");
        printf("Filename -> %s and size-> %d\n", filename, sizeof(filename));
        FILE* fp = fopen(filename, "wb");
        tot=0;
        if(fp != NULL){
            while( (b = recv(confd, buff, FILE_BUF_SIZE,0))> 0 ) {
                tot+=b;
                printf("Bytes recived -> %i\n", b);
                fwrite(buff, 1, b, fp);
            }

            printf("Received byte: %i\n",tot);
            if (b<0)
                printf("ERROR in reciving\n");
               //perror("Receiving");

            fclose(fp);
        } else {
            //perror("File");
            printf("ERROR in FILE\n");
        }

        //Send UUID to msg queue
        //send_uuid_str_to_msg_queue(uuid_str); 
        printf("UUID Recived -> %s\n", uuid_str);
         
        //freeing pointers
        printf("Freeing memory");
        //memset(filename, 0, filename_size);
        //memset(uuid_str, 0, UUID_SIZE_FOR_STR);
        close(confd);
    }

    //return 0;
}

int main(int argc, char** argv){
    printf("Starting Server\n");
    server_recive_json_file_and_send_uuid();
}