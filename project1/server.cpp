#include "header.h"

int main(int argc, char *argv[]){
    int server_port = parse_server_args(argc, argv);
    int server_sockfd = create_socket(), client_sockfd;
    struct sockaddr_in server_info, client_info;
    socklen_t client_info_size = sizeof(client_info);
    char empty_ip[] = {""};
    const int MAX_NUM_CONN = 1024;
    create_socket_info(&server_info, SERVER, empty_ip, server_port);
    create_socket_info(&client_info, -1, empty_ip, -1);
    bind(server_sockfd, (struct sockaddr *)&server_info, sizeof(server_info));
    listen(server_sockfd, MAX_NUM_CONN);
    
    fd_set master_set;
    fd_set working_set;
    FD_ZERO(&master_set);
    FD_ZERO(&working_set);
    FD_SET(server_sockfd, &master_set);
    int max_fd = server_sockfd + 1;
    struct timeval select_timeout;
    select_timeout.tv_sec = 0;
    select_timeout.tv_usec = 100000;
    char server_msg[] = "Hi, I am server.";

    while(true){
        memcpy(&working_set, &master_set, sizeof(master_set));
        if(select(max_fd, &working_set, NULL, NULL, &select_timeout) == -1){
            fprintf(stderr, "Select error in server!!\n");
            exit(-1);
        }
        for(int fd = 0; fd < max_fd; fd ++){
            if(FD_ISSET(fd, &working_set) == false)
                continue;
            //printf("selected fd: %d\n", fd);
            if(fd == server_sockfd){
                // Server receives new connections.
                client_sockfd = accept(
                        server_sockfd, 
                        (struct sockaddr *)&client_info, 
                        &client_info_size);
                printf("recv from client %s:%d\n", 
                        inet_ntoa(client_info.sin_addr), 
                        ntohs(client_info.sin_port));
                FD_SET(client_sockfd, &master_set);
                send(client_sockfd, server_msg, sizeof(server_msg), 0);
                if(client_sockfd + 1 > max_fd)
                    max_fd = client_sockfd + 1;
            }else{
                // Client sends more message required dealing.
                char recv_msg[100] = {};
                int recv_len = recv(fd, recv_msg, sizeof(recv_msg), 0);
                if(recv_len <= 0){
                    //fprintf(stderr, "Receiving error from client.\n");
                    close(fd);
                    FD_CLR(fd, &master_set);
                    //exit(-1);
                }else{
                    send(fd, server_msg, sizeof(server_msg), 0); 
                }
            }
        }
    }
    return 0;
}
