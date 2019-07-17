#include <pthread.h>

#include <orenda_socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void *_client_start_recv_test(void *handle){
    int ret;
    printf("orenda, %s entry\n", __func__);
	client_socket_t* m_client_socket_t = (client_socket_t*)handle;
	ret = client_start_recv(m_client_socket_t);
    return NULL;
}
int main()
{
    int32_t ret = 0;
	size_t retry = 0;
	char buf[] = "client_msg\n";
	int32_t fd = -1;
	client_socket_t* m_client_socket_t;
	pthread_t m_client_start_recv_pthread = -1;
    printf("orendai0, %s entry\n", __func__);
	m_client_socket_t = malloc(sizeof(client_socket_t));
	if(m_client_socket_t == NULL){
		printf("m_client_socket_t malloc failed\n");
		return -1;
	}
	memset(m_client_socket_t, 0, sizeof(client_socket_t));
    ret = client_init_fd(m_client_socket_t);

	//create new thread to receive msg from server
    ret = pthread_create(&m_client_start_recv_pthread, NULL, _client_start_recv_test, m_client_socket_t);//does it need a lock?
    if (ret < 0) {
        ALOGE("Failed to create receiver thread errno:%d", errno);
    }
    
	while(retry<2){
		usleep(1000*1000*2);
    	//printf("fd:%d\n", m_client_socket_t->m_command_fd);
    	orenda_client_send(m_client_socket_t->m_command_fd, buf, sizeof(buf));
		retry ++;
	}
    
    pthread_join(m_client_start_recv_pthread,NULL);//wait for tid exit
    free(m_client_socket_t);
	client_deinit_fd(m_client_socket_t);
	m_client_socket_t = NULL;
    return 0;

}
