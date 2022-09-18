#include "chat.h"

int main(int argc, const char *argv[])
{
    int fd = -1; 
    int port = SERV_PORT;
    pid_t pid;
    MSG msg;

    port = atoi(argv[2]);
    if(port < 0 || (port > 0 && port <= 5000)) {
        printf("%s serv_ip serv_addr\n", argv[0]);
        exit(1);
    }   

    struct sockaddr_in sin;
    if(argc != 3){ 
        printf("%s serv_ip serv_addr\n", argv[0]);
        exit(1);
    }   

    /*1.创建socket fd*/
    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){ 
        perror("socket");
        exit(1);
    }   

    /*sockaddr_in 结构体填充*/
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERV_PORT);
    if(inet_pton(AF_INET, argv[1], (void *)&sin.sin_addr) != 1){ 
        perror("inet_pton");
        exit(1);
    } 

    /*用户名输入*/
    printf("输入用户名：");
    fgets(msg.name, N, stdin);
    msg.name[strlen(msg.name)-1] = '\0';

    /*发送登陆信号及用户名*/
    msg.type = 1;
    sendto(fd, &msg, sizeof(msg), 0, (struct sockaddr *)&sin, sizeof(sin));

    pid = fork();
    /*子进程发送消息*/
    if(pid == 0)
    {
        /*设置当父进程退出时，子进程也退出*/
        prctl(PR_SET_PDEATHSIG, SIGHUP);
        while(1)
        {
            memset(msg.text, 0, N);
            fgets(msg.text, N, stdin);
            msg.text[strlen(msg.text)-1] = '\0';
            /*输入quit表示退出*/
            if(strncmp(msg.text, QUIT_STR, 4) == 0){
                msg.type = 3;
                sendto(fd, &msg, sizeof(msg), 0, (struct sockaddr *)&sin, sizeof(sin));
                /*杀死父进程*/
                kill(getppid(), SIGINT);         
            }
            else
            {   /*输入的不是quit就发送出去*/
                msg.type = 2;
                sendto(fd, &msg, sizeof(msg), 0, (struct sockaddr *)&sin, sizeof(sin));
            }
        }
    }
    else /*父进程接收消息*/
    {
        while(1){
            recvfrom(fd, &msg, sizeof(msg), 0, (struct sockaddr *)&sin, (unsigned int *)sizeof(sin));
            printf("%s\n", msg.text);
        } 
    }
    return 0;
}