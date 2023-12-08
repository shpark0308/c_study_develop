#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sys/epoll.h>

#define MAX_CLIENT (5)
#define BUF_SIZE   (1024)
#define EVENT_MAX  (1)
#define EPOLL_SIZE (20)

int user_fds[1024];

struct udata
{
	int fd;
	char name[80];
};

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void send_to_clients(struct epoll_event ev, char* msg)
{
	char buf[BUF_SIZE + 24]; // 24를 하는 이유는?

	struct udata* user_data = (struct udata*)ev.data.ptr; // 여기 ptr 에 담는가보다
	for (int i=0; i<1024; i++) // client 가 존재하는지 아닌지 여부를 이곳으로 확인하나보다...
	{
		memset(buf, 0, BUF_SIZE + 24);
		snprintf(buf, BUF_SIZE+24, "%s : %s", user_data->name, msg);
		if ((user_fds[i] == 1))
		{
			send(user_fds[i], buf, BUF_SIZE+24, 0);
		}
	}	
}

int main()
{
	int serv_sock, clnt_sock;


	// 배열 초기화
	memset(user_fds, -1, sizeof(int)*1024);
	
	// (1). 소켓 생성
	serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("serv sock create failed");

	// (2). bind 생성하기
	const char* serv_port = "8088";

	struct sockaddr_in serv_addr, clnt_addr;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("bind error");

	// (3). listen
	bRet = listen(serv_sock, MAX_CLIENT);
	if (bRet <0) err_handle("listen failed");
#if 0
	// (4). accept ( 이벤트 등록 )
	struct epoll_event ev;
	ev.events = EPOLLIN; // 이벤트가 들어올 시, 알림
	// struct epoll_event {
	// 	__unint32_t events;
	//	epoll_data_t data;
	// }
	// typedef union epoll_data
	// {
	//	void* ptr;	// 이벤트가 발생한 파일디스크립터의 구조체
	//	int fd;		// 이벤트가 발생하게 될 파일 디스크립터
	//	_unint32_t u32;
	//	_unint64_t u64;
	// }
	ev.data.fd = serv_sock;
	//////////////////////////////////////////////////////////////
#endif

	// accept 를 위한 이벤트 생성
	// struct epoll_event {
	// 	__unint32_t events; //
	// }
	//
	// accept 를 위한 [ 이벤트 ] 를 생성 (( epoll_event )) -> (( epoll_data_t )) 가 있음
	// struct epoll_event {
	// 	__uint32_t events;
	// 	epoll_data_t data;
	// }
	// typedef union epoll_data
	// {
	// 	void* ptr; 	// 이벤트가 발생한 파일디스크립터의 (( 구조체 ))
	//	int fd;
	//	_unint32_t u32;
	//	_unint64_t u64;
	// } epoll_data_t;
	//
	struct epoll_event ev; // ev listen 소켓의 사건
	ev.events = EPOLLIN;
	ev.data.fd = serv_sock;

	


#if 0
	// [1]. 파일 디스크립터 저장소 생성
	int epollfd = epoll_create(EVENT_MAX);
	if (epollfd < 0) err_handle("epoll_create failed");

#endif
	// [1]. 파일 디스크립터 저장소 생성
	int epollfd = epoll_create(1);
	if (epollfd < 0) err_handle("epoll_crate failed");
#if 0
	// [2]. 관심있는 파일 디스크립터를 epoll 관찰 대상으로 등록
	// int epoll_ctrl(int epfd, int op, struct epoll, struct epoll_event *events);
	epoll_ctl(epollfd, EPOLL_CTL_ADD, serv_sock, &ev); // serv_sock 의 상태 변화를 epollfd 를 통해 관찰
#endif
	// [2]. 관심 있는 파일 디스크립터를 epoll 관찰 대상으로 등록
	// int epoll_ctl(int epfd, int op, 관찰 대상 fd, struct epoll_event* event);
	epoll_ctl(epollfd, EPOLL_CTL_ADD, serv_sock, &ev);


	struct udata *user_data; // 계속 새로 할당
	char send_msg[BUF_SIZE];

	int event_count;
	struct epoll_event* events = (struct epoll_event*)malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
	printf("EPOLL_SIZE : (%d)\n", EPOLL_SIZE);

	while(true) // ==> thread 로 따로 빼야한다.
	{
		// 사건 발생 시까지 무한 대기
		// int epoll_wait(int epfd, struct epoll_event * event, int maxevents, int timeout);
		//////// 
		// epollfd 의 사건 발생 시, events 에 fd 를 채운다.
		// eventn 은 serv_socket 에 connection 된 fd 세움
		// 사건 발생 시까지 무한 대기
		// int epoll_wait(int epfd, struct epoll_event* event, int maxevents, int timeout);
		// event 받아올 event
		event_count = epoll_wait(epollfd, events, EPOLL_SIZE, -1);
		if (event_count<0) err_handle("epoll_wait failed");

		for (int i=0; i<event_count; i++) // 이벤트 횟수만큼 감지
		{
			if (events[i].data.fd == serv_sock)
			{
				memset(send_msg, 0, BUF_SIZE);
				
				int clnt_addr_size = sizeof(clnt_addr);
                        	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);
				user_fds[clnt_sock]  = 1;

				user_fds[clnt_sock] = 1;
				user_data = (struct udata*) malloc(sizeof(user_data));
				memset(user_data, 0, sizeof(user_data));

				user_data->fd = clnt_sock;

				char* temp = " First insert your nickname : ";
				int send_len = send(user_data->fd, temp, strlen(temp),0);

				sleep(1);
				int recv_len = recv(user_data->fd, user_data->name, sizeof(user_data->name),0);

				printf("Welcome [%s] \n", user_data->name);

				sleep(1);
				snprintf(send_msg, sizeof(send_msg), "Okay your nickname : %s\n", user_data->name);

				send_len = send(user_data->fd, send_msg, strlen(send_msg), 0);

				ev.events = EPOLLIN;
				ev.data.fd = clnt_sock;
				ev.data.ptr = user_data;

				// 이게 무슨뜻인지
				epoll_ctl(epollfd, EPOLL_CTL_ADD, clnt_sock, &ev);

			}
			else // 연결된 소켓에서 이벤트가 발생함
			{
				user_data = (struct udata*)events[i].data.ptr; // 해당 데이터를 가지고 오기
				memset(send_msg, 0, BUF_SIZE);
				int recv_len = recv(user_data->fd, send_msg, BUF_SIZE,0);
				if (recv_len <=0)
				{
					// 소켓 종료
					epoll_ctl(epollfd, EPOLL_CTL_DEL, user_data->fd, events);
					close(user_data->fd);
					user_fds[user_data->fd] = -1;
					free(user_data); // 메모리 해지까지 완벽하게 해주어야 한다.
				}
				else
				{
					send_to_clients(events[i], send_msg);
				}
			}

		}
	}

	return 0;
}
