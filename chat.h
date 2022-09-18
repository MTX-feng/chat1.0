#ifndef __CHAT_H__
#define __CHAT_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>                  /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>                 /* superset of previous */
#include <signal.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/prctl.h>


#define SERV_PORT 6666
#define SERV_IP_ADDR "127.0.0.1"                                                                              

#define QUIT_STR "quit"
#define N 128

#define L 1
#define C 2
#define Q 3

typedef struct msg{
	char type;   	//消息类型
	char name[32];	//用户名
	char text[N];	//消息
}MSG;

//存放客户端的ip链表
typedef struct node{
	struct sockaddr_in addr;
	struct node *next;
}listnode, *linklist;

/*创建链表头节点*/
linklist linklist_create()
{
	linklist H;
	H = (linklist)malloc(sizeof(listnode));
	H->next = NULL;
	return H;
}

#endif
