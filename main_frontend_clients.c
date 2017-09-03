#include "serverside_for_clients.h"

int main(int argc, char** argv){
    printf("Starting Server to handle clients");
    server_recive_json_file_and_send_uuid();
}
