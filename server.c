#include "chat.h"

void do_login(int fd, MSG msg, linklist H, struct sockaddr_in cin);
void do_chat(int fd, MSG msg, linklist H, struct sockaddr_in cin);
void do_quit(int fd, MSG msg, linklist H, struct sockaddr_in cin);

int main(){
    int fd = -1;
    struct sockaddr_in sin;
    MSG msg;
    pid_t pid;

    /*1.创建socket fd*/
    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){                                                                
        perror("socket");
        exit(1);
    }

    //允许绑定地址快速重用
    int b_reuse = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof(int));

    /*2.绑定*/
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERV_PORT);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0){
        perror("bind");
        exit(1);
    }

    char buf[BUFSIZ];
    struct sockaddr_in cin;
    socklen_t addrlen = sizeof(cin);
    printf("UDP server started!\n");

    pid = fork();
    if(pid == 0)
    {   
        while(1)
        {

        }

    }else{ //父进程接收数据
        linklist H = linklist_create();
        
        while(1){        
            recvfrom(fd, &msg, sizeof(msg), 0, (struct sockaddr *)&cin, &addrlen);   
            /*判断接收到的消息类型*/
            switch(msg.type)
            {
                case 1:
                    do_login(fd, msg, H, cin);
                    break;
                case 2:
                    do_chat(fd, msg, H, cin);
                    break;
                case 3:
                    do_quit(fd, msg, H, cin);
                    break;
            }
        }
    }
    return 0;
}

void do_login(int fd, MSG msg, linklist H, struct sockaddr_in cin)
{
    linklist p = H->next;
    sprintf(msg.text, "%s login", msg.name);;
    int len = sizeof(cin);
    while(p){
        /*发送客户端登陆信息给其他客户端*/
        sendto(fd, &msg, sizeof(msg), 0, (struct sockaddr *)&p->addr, len);
        p = p->next;
        //printf("%s\n", p->addr);
    }

    /*采用头插法将当前客户端添加之链表*/
    p = (linklist)malloc(sizeof(listnode));
    p->addr = cin;
    p->next = H->next;
    H->next = p;

    return;
}

void do_chat(int fd, MSG msg, linklist H, struct sockaddr_in cin)
{
    linklist p = H->next;
    char s[N] = {0};
    /*字符串拼接 哪一个用户发送了什么消息*/
    sprintf(s, "%s said %s", msg.name, msg.text);
    strcpy(msg.text, s);

    int len = sizeof(cin);
    while (p)
    {   /*将消息转发之除了发送客户端的其他客户端*/
        if (memcmp(&cin, &p->addr, len) != 0)
        {
            sendto(fd, &msg, sizeof(msg), 0, (struct sockaddr *)&p->addr, len);
        }
            p = p->next;
    }

    return;
}

void do_quit(int fd, MSG msg, linklist H, struct sockaddr_in cin)
{
    linklist p = H;
    linklist  q;
    sprintf(msg.text, "%s offline", msg.name);
    while (p->next)
    {   /*将接收到的客户端信息与链表中的进行对比，相同则释放此节点*/
        if (memcmp(&cin, &p->next->addr, sizeof(cin)) == 0)
        {
            q = p->next;
            p->next = q->next;
            free(q);
            q = NULL;
        }
        else
        {   /*发送退出客户端信息至未退出客户端*/
            sendto(fd, &msg, sizeof(msg), 0, (struct sockaddr *)&p->next->addr, sizeof(p->next->addr));
            p = p->next;
        }
    }
    return;
}
