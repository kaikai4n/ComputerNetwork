#include "header.h"

int main(int argc, char *argv[]){
    int server_port = parse_server_args(argc, argv);
    int server_sockfd = create_socket(), client_sockfd;
    struct sockaddr_in server_info, client_info;
    socklen_t client_info_size = sizeof(client_info);
    char empty_ip[] = {""};
    const int MAX_NUM_CONN = 5;
    create_socket_info(&server_info, SERVER, empty_ip, server_port);
    create_socket_info(&client_info, -1, empty_ip, -1);
    bind(server_sockfd, (struct sockaddr *)&server_info, sizeof(server_info));
    listen(server_sockfd, MAX_NUM_CONN);
    
    while(true){
        printf("Waiting for new connections.\n");
        client_sockfd = accept(
                server_sockfd, 
                (struct sockaddr *)&client_info, 
                &client_info_size);
        char server_msg[] = "Hi, I am server.";
        printf("Server receive connection from client %d\n", 
                client_info.sin_addr.s_addr);
        send(client_sockfd, server_msg, sizeof(server_msg), 0);
        printf("Server send message and exit\n");
        exit(0);
    }
    return 0;
}
