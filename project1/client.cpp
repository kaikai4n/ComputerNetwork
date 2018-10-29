#include "header.h"

void *ping(void *this_ping_args);

int main(int argc, char *argv[]){
    struct client_args *args = parse_client_args(argc, argv);
    /*
    printf("number: %d, timeout: %d, server_num: %d\n", args->number, args->timeout, args->server_num);
    for(int i = 0; i < args->server_num; i++){
        printf("host1:port1 %s:%d\n", args->host_port[i]->host, args->host_port[i]->port);
    }
    */
    pthread_t threads[args->server_num];
    struct ping_args this_ping_args[args->server_num];
    for(int server_i = 0; server_i < args->server_num; server_i ++){
        this_ping_args[server_i].number = args->number;
        this_ping_args[server_i].timeout = args->timeout;
        this_ping_args[server_i].host_port = args->host_port[server_i];
        pthread_create(
                &threads[server_i], 
                NULL, 
                ping, 
                (void *)&this_ping_args[server_i]);
    }
    for(int server_i = 0; server_i < args->server_num; server_i ++){
        pthread_join(threads[server_i], NULL);
    }
    return 0;
}

void *ping(void *this_ping_args){
    struct ping_args *ping_args = (struct ping_args *)this_ping_args;
    char *ip = ping_args->host_port->host;
    int port = ping_args->host_port->port;
    int number = ping_args->number;
    int timeout = ping_args->timeout;
    struct sockaddr_in sock_info;
    create_socket_info(&sock_info, CLIENT, ip, port);
    // Connect successfully
    for(int send_i = 0; send_i < number || number == 0
            ; send_i ++){
        //printf("Total send number:%d/%d\n", send_i, args->number);
        int sockfd = create_socket();
        set_timeout(sockfd, timeout);
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
        float delayed_msec = tok(&start_time);
        //printf("Delayed_msec: %d\n", delayed_msec);
        if((int)delayed_msec >= timeout || recv_len <= 0){
            // timeout
            printf("timeout when connect to %s\n", ip);
        }else{
            printf("recv from %s, RTT = %.3f msec\n",
                    ip, delayed_msec);
        }
        close(sockfd);
    }
    pthread_exit(NULL);
}
