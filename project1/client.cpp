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
    int sockfd = create_socket();
    char ip[] = {"140.112.90.200"};
    int port = 7122;
    struct sockaddr_in sock_info;
    create_socket_info(&sock_info, CLIENT, ip, port);
    int conn_msg = connect(
            sockfd, 
            (struct sockaddr *)&sock_info, 
            sizeof(sock_info));
    if(conn_msg == -1){
        printf("Client cannot connect to server of %s:%d\n",
                ip, port);
        exit(-2);
    }
    return 0;
}
