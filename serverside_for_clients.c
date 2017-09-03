#include "serverside_for_clients.h"

void uuid_to_str(uuid_t uuid, char* str_ptr){
    sprintf(
        str_ptr, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", 
        uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
        uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]
    );
}

void write_to_file_from_client(int confd, char* buff, char* filename){ 
    FILE* fp = fopen(filename, "wb");
    int tot=0, b;
    if(fp != NULL){
        while( (b = recv(confd, buff, BUF_SIZE,0))> 0 ) {
            tot+=b;
            printf("Bytes recived -> %i\n", b);
            fwrite(buff, 1, b, fp);
        }
        printf("Fine till here\n");
        printf("Received byte: %i\n",tot);
        if (b<0)
            printf("ERROR in reciving\n");
            //perror("Receiving");
        printf("Closing file\n");
        if (fclose(fp)) { printf("error closing file."); exit(-1); }
        printf("No error in closing file\n");
    } else {
        //perror("File");
        printf("ERROR in FILE\n");
    }
}

void read_file_and_send_to_client(int confd, char* buff, char* filename){
    FILE* fp = fopen(filename, "rb");
    printf("File to be opend -> %s\n", filename);
    int tot=0, b;
    if(fp != NULL){
        while((b = fread(buff, 1, sizeof(buff), fp)) > 0){ 
            send(confd, buff, b, 0); 
        }
        if (fclose(fp)) { printf("error closing file."); exit(-1); }
        printf("No error in closing file or sending file\n");
    } else {
        //perror("File");
        printf("ERROR in FILE, fp == NULL\n");
    }
}

void server_recive_json_file_and_send_uuid(){
    int fd =0, confd = 0,b,tot;
    struct sockaddr_in serv_addr;

    char buff[FILE_BUF_SIZE + 1];
    int num, rm;

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
    char filename[strlen(uuid_str) + strlen(DATA_DIR) + strlen(".json") + 1];
    size_t filename_size = strlen(filename);
    char output_filename[strlen(uuid_str) + strlen(DATA_DIR) + strlen(".txt") + 1];
    ////strcpy(filename, DATA_DIR);
    ////strcpy(filename, uuid_str);
    //printf("Filename size -> %i\n", filename_size);

    char mode_file_or_uuid;
    char file_exist_or_not;
    
    while(1){
        confd = accept(fd, (struct sockaddr*)NULL, NULL);
        if (confd==-1) {
            perror("Accept");
            continue;
        }

        if((b = read(confd, &mode_file_or_uuid, 1)) != 1)
            printf("\nMode specified worngly\n\n");

        if(mode_file_or_uuid == 'f'){
            //Generating UUID 
            uuid_generate_random(uuid);
            uuid_to_str(uuid, uuid_str);
            printf("UUID created -> %s\n", uuid_str); 
             
            printf("SENDING UUID to client\n");
            send(confd, uuid_str, UUID_SIZE_FOR_STR, 0);
            
            strcpy(filename, DATA_DIR);
            strcat(filename, uuid_str);
            strcat(filename, ".json");
            printf("Filename -> %s and len-> %d\n", filename, strlen(filename));
            
            write_to_file_from_client(confd, (char *)&buff, (char *)&filename);
    
            //Sending UUID str to Client

            //Send UUID to msg queue
            send_uuid_str_to_msg_queue(uuid_str); 
            printf("UUID Recived -> %s\n", uuid_str);
        }
        else if(mode_file_or_uuid == 'u'){
            if((b = recv(confd, uuid_str, UUID_SIZE_FOR_STR, 0)) < 32) {printf("EROR: Reciving UUID");}
            strcpy(output_filename, DATA_DIR);
            strcat(output_filename, uuid_str);
            strcat(output_filename, ".txt");
            printf("Output filename -> %s\n", output_filename);

            if(access(output_filename, F_OK) != 1){
                printf("File exists\n");
                file_exist_or_not = 't';     // 't' means file exist, means 'there'
                write(confd, &file_exist_or_not, sizeof(char));
                
                read_file_and_send_to_client(confd, (char *)&buff, (char *)&output_filename);
                
                printf("Deleting output file after transfering to Client\n");
                if((rm = remove(output_filename)) != 0){printf("ERROR: deleting result file");}
            }
            else{
                file_exist_or_not = 'n';
                write(confd, &file_exist_or_not, sizeof(char));
            }
        }

        close(confd);
    }

    //return 0;
}

/*int main(int argc, char** argv){
    printf("Starting Server\n");
    server_recive_json_file_and_send_uuid();
}*/
