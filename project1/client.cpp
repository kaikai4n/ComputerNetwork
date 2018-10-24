#include "header.h"

int main(int argc, char *argv[]){
    struct client_args *args = parse_client_args(argc, argv);
    /*
    printf("number: %d, timeout: %d, server_num: %d\n", args->number, args->timeout, args->server_num);
    for(int i = 0; i < args->server_num; i++){
        printf("host1:port1 %s:%d\n", args->host_port[i]->host, args->host_port[i]->port);
    }
    */
    for(int server_i = 0; server_i < args->server_num; server_i ++){
        char *ip = args->host_port[server_i]->host;
        int port = args->host_port[server_i]->port;
        struct sockaddr_in sock_info;
        create_socket_info(&sock_info, CLIENT, ip, port);
        // Connect successfully
        for(int send_i = 0; send_i < args->number || args->number == 0
                ; send_i ++){
            //printf("Total send number:%d/%d\n", send_i, args->number);
            int sockfd = create_socket();
            set_timeout(sockfd, args->timeout);
            int conn_msg = connect(
                    sockfd, 
                    (struct sockaddr *)&sock_info, 
                    sizeof(sock_info));
            if(conn_msg == -1){
                //printf("Client cannot connect to server of %s:%d\n",
                //        ip, port);
                //exit(-2);
                printf("timeout when connect to %s\n", ip);
                close(sockfd);
                continue;
            }
            char send_msg[] = "Hello server!!\n";
            char recv_msg[100] = {};
            struct timeval start_time;
            tik(&start_time);
            send(sockfd, send_msg, sizeof(send_msg), 0);
            int recv_len = recv(sockfd, recv_msg, sizeof(recv_msg), 0);
            int delayed_msec = tok(&start_time);
            //printf("Delayed_msec: %d\n", delayed_msec);
            if(delayed_msec >= args->timeout || recv_len <= 0){
                // timeout
                printf("timeout when connect to %s\n", ip);
            }else{
                printf("recv from %s, RTT = %d msec\n",
                        ip, delayed_msec);
            }
            close(sockfd);
        }
    }
    return 0;
}
