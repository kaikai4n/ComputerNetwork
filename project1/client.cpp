#include "header.h"

int main(int argc, char *argv[]){
    struct client_args *args = parse_client_args(argc, argv);
    printf("number: %d, timeout: %d, server_num: %d\n", args->number, args->timeout, args->server_num);
    for(int i = 0; i < args->server_num; i++){
        printf("host1:port1 %s:%d\n", args->host_port[i]->host, args->host_port[i]->port);
    }
    exit(0);
    int sockfd = create_socket();
    char ip[] = {"140.112.30.41"};
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
    // Connect successfully
    char send_msg[] = "Hello server!!\n";
    char recv_msg[100] = {};
    send(sockfd, send_msg, sizeof(send_msg), 0);
    printf("Client send message.\n");
    recv(sockfd, recv_msg, sizeof(recv_msg), 0);
    printf("Client receives message: %s.\n", recv_msg);
    return 0;
}
