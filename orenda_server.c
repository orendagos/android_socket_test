#include <pthread.h>

#include <orenda_socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void *_server_send_test(void *handle){
    int ret;
	int retry = 0;
	char buf[]="_server_send_test\n";
    printf("orenda, %s entry\n", __func__);
	server_socket_t* m_server_socket_t = (client_socket_t*)handle;
	while(retry<5){
		usleep(1000*1000*20);
		server_send_msg(m_server_socket_t,buf, strlen(buf));
	}
	
	
    return NULL;
}


int main()
{
    int ret;
    printf("orenda, %s entry\n", __func__);
	server_socket_t* m_server_socket_t;
	pthread_t m_server_send_pthread = -1;
	m_server_socket_t = malloc(sizeof(server_socket_t));
	if(m_server_socket_t==NULL){
		printf("m_server_socket_t malloc failed\n");
		return -1;
	}
	memset(m_server_socket_t, 0, sizeof(server_socket_t));
	server_init(m_server_socket_t);

	ret = pthread_create(&m_server_send_pthread, NULL, _server_send_test, m_server_socket_t);//does it need a lock?
    if (ret < 0) {
        ALOGE("Failed to create receiver thread errno:%d", errno);
    }
	//receive msg in main thread;
	server_recv_msg(m_server_socket_t);

	
	pthread_join(m_server_send_pthread,NULL);//wait for tid exit
	server_deinit(m_server_socket_t);

	free(m_server_socket_t);
	m_server_socket_t = NULL;
    return 0;

}
