
int create_socket();
void create_socket_info(
    struct sockaddr_in *info, 
    int type, 
    char *ip, 
    int port
);


int create_socket(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("Cannot create socket.");
        exit(-1);
    }
    return sockfd; 
}

void create_socket_info(
        struct sockaddr_in *info, 
        int type, 
        char *ip, 
        int port){
    bzero(info, sizeof(*info));
    if(type == CLIENT){
        info->sin_family = PF_INET;
        info->sin_addr.s_addr = inet_addr(ip);
        info->sin_port = htons(port);
    }else if(type == SERVER){
        info->sin_family = PF_INET;
        info->sin_addr.s_addr = INADDR_ANY;
        info->sin_port = htons(port);
    }else{
        // an empty sockaddr_in structure
    }
}
