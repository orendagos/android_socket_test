#include <cutils/sockets.h>
#include <cutils/record_stream.h>
#include <unistd.h>
#include <stdio.h>

#include "orenda_socket.h"
#include <sys/un.h>

static RecordStream *p_rs = NULL;
static int32_t m_command_fd = -1;

int server_deinit(server_socket_t* m_server_socket_t){
	if (m_server_socket_t->fd >= 0) {
            close(m_server_socket_t->fd);
            m_server_socket_t->fd = -1;
        }
	return 0;
}

int server_send_msg(server_socket_t* m_server_socket_t,void* buffer, size_t len){
    int ret;
	ret = orenda_client_send(m_server_socket_t->fd, buffer, len);
	
	return ret;
}

int server_recv_msg(server_socket_t* m_server_socket_t){

	RecordStream *p_rs;
	int ret;
	void *p_record = NULL;
    size_t recordlen = 0;
	p_record = m_server_socket_t->buffer_get;
p_rs = record_stream_new(m_server_socket_t->fd, MAX_COMMAND_BYTES);
    
    for (;;) {
        printf("begin to read stream \n");
        ret = record_stream_get_next(p_rs, &p_record, &recordlen);

        if (ret == 0 && p_record == NULL) {
            break;
        } else if ((ret < 0)&&(errno!= EAGAIN)) {
            break;
        } else if (ret == 0) {
            printf("recv****************\n");
            print_buf(p_record, recordlen);
			printf("recv end################n");
			
        }
    }
	ALOGE("ret = %d, errno = %d", ret, errno);

	if (ret == 0 || !(errno == EAGAIN || errno == EINTR)) {
        if (ret != 0) {
            ALOGE("error on reading command socket errno:%d", errno);
        } else {
            ALOGE("EOS.  Closing command socket.");
        }

        
        if (p_rs != NULL) {
            record_stream_free(p_rs);
            p_rs = NULL;
        }

    }

	return 0;
	
}


void server_init(server_socket_t* m_server_socket_t){
	int32_t m_listen_fd = -1;
	int32_t fd, ret;
	//struct sockaddr_un peeraddr;
    struct sockaddr_un peeraddr;

    //sockaddr_un peeraddr;
    socklen_t socklen = sizeof(peeraddr);
	void *p_record = NULL;
    size_t recordlen = 0;
    
	printf("orenda, %s entry\n", __func__);
	m_listen_fd = socket_local_server(SOCKET_NAME_DEFAULT, ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
    if (m_listen_fd < 0) {
        ALOGE("Failed to get socket %s", SOCKET_NAME_DEFAULT);
        return;
    }
    printf("orenda, %s begin to listen:m_listen_fd=%d\n", __func__, m_listen_fd);
    ret = listen(m_listen_fd, 4);
    printf("orenda, register m_listen_fd=%d\n", m_listen_fd);

    if (ret < 0) {
        ALOGE("Failed to listen on control socket '%d': %d", m_listen_fd, errno);
        return;
    }
    
    m_server_socket_t->fd = accept(m_listen_fd, (struct sockaddr *) &peeraddr, &socklen);
	printf("server accept successfully\n");
	m_command_fd = m_server_socket_t->fd;
    //start to return
    return;
    

}