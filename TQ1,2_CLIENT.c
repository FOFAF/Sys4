/* tcp_client.c */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 3490 /* 서버 포트 */
#define MAXDATASIZE 100

int csock, numbytes;

void *t_function(void *data)
{
	char buf[MAXDATASIZE];
	while(1){
	numbytes = -1;
	memset(buf, 0, MAXDATASIZE);
	if ((numbytes = recv(csock, buf, MAXDATASIZE, 0)) == -1) {
		continue;
	}
	printf("Received: %s\n", buf);
}
}

int main(int argc, char *argv[])
{
//int csock, numbytes;
	char buf[MAXDATASIZE];
	struct sockaddr_in serv_addr;
	int len;
    	int thr_id;
 	pthread_t p_thread;
   	int a = 1;
	if (argc != 3) {
		fprintf(stderr,"usage: tcp_client <SERVER IP><ECHO STRING>\n");
		exit(1);
	}
	/* 클라이언트 소켓 생성 */
	if ((csock = socket(AF_INET, SOCK_STREAM, 0))== -1) {
		perror("socket");
		exit(1);
	}
	/* 서버 주소 설정 */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(PORT);
	/* 서버 연결 */
	if (connect(csock, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1) {
		perror ("connect");
		exit (1);
	}
    	thr_id = pthread_create(&p_thread, NULL, t_function, (void *)&a);
    	if (thr_id < 0)
    	{
       		perror("thread create error : ");
        		exit(0);
    	}
	printf("CreateTH\n");
while(1){
	/* 메시지 송수신 */
	memset(buf, 0, MAXDATASIZE);
	gets(buf);
	len = strlen(buf);
	if ( send(csock, buf, len, 0) != len) {
		
	}

}
	close (csock);
}
