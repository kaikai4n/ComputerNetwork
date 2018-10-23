#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "data_header.h"
#include "network_function.h"

int main(int argc, char *argv[]){
    int server_sockfd = create_socket(), client_sockfd;
    struct sockaddr_in server_info, client_info;
    char *server_ip = {"140.112.30.41"};
    int server_port = 7122;
    const int MAX_NUM_CONN = 5;
    create_socket_info(&server_info, SERVER, server_ip, server_port);
    create_socket_info(&client_info, -1, "", -1);
    bind(server_sockfd, (struct sockaddr *)&server_info, sizeof(server_info));
    listen(server_sockfd, MAX_NUM_CONN);
    
    while(true){
        client_sockfd = accept(
                server_sockfd, 
                (struct sockaddr *)&client_info, 
                sizeof(client_info));
        server_msg = "Hi, I am server.";

    }
    return 0;
}
