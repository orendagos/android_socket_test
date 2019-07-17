#include <cutils/sockets.h>
#include <cutils/record_stream.h>
#include <unistd.h>
#include <stdio.h>

#include "orenda_socket.h"



int print_buf(void* buf, int len)
{
    int fd = 1;//file operation for stdout
    size_t count = 0;
    while (count<len) {
#ifndef USE_BUF_FILE_SYS
        int i = write(fd, count+(char *)buf, len-count);
#else
        int i = fwrite(count+(char *)buf, sizeof(char), len-count, stdout);
#endif
        if (i<1) return i;
        count += i;
    }
    //printf("count:%d\n", count);
    return count;
}
//client_start_recv should be run in a new thread
int client_start_recv(client_socket_t* m_client_socket_t){
	RecordStream *p_rs = NULL;
	int ret;
	void *p_record = NULL;
    size_t recordlen = 0;
	p_record = m_client_socket_t->buffer_get;
	if (p_rs != NULL) {
            record_stream_free(p_rs);
            p_rs = NULL;
        }
        if (p_rs == NULL) {
            p_rs = record_stream_new(m_client_socket_t->m_command_fd, MAX_COMMAND_BYTES);
        }

		// ??? if ret=0,errno=11 ??
		for (;;) {
            ret = record_stream_get_next(p_rs, &p_record, &recordlen);

            if (ret == 0 && p_record == NULL) {
                break;
            } else if (ret < 0 && errno != EAGAIN) {
                break;
            } else if (ret == 0) {
                printf("RECV:***************\n");
                print_buf(p_record, recordlen);
				printf("###############\n");
            }
        }

		//ALOGI("Disconnected from %s socket", socket_name);
        ALOGI("ret = %d, errno = %d", ret, errno);
		ALOGI("didn't close fd");
		return 0;
		
}



int32_t client_deinit_fd(client_socket_t* m_client_socket_t){
	close(m_client_socket_t->m_command_fd);
	return 0;
}

int32_t client_init_fd(client_socket_t* m_client_socket_t)
{
    //int32_t m_command_fd = -1;

    const char *socket_name = SOCKET_NAME_DEFAULT;
    int32_t retry_count = 0;
    printf("orenda, %s entry\n", __func__);
    for(;;) {
        m_client_socket_t->m_command_fd = socket_local_client(socket_name, ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
        printf("orenda, m_command_fd:%d\n", m_client_socket_t->m_command_fd);
        if (m_client_socket_t->m_command_fd < 0) {
            if (retry_count == 8) {
                ALOGE("Couldn't find '%s' socket after %d times, continuing to retry silently", socket_name, retry_count);
            } else if (retry_count >= 0 && retry_count < 8) {
                ALOGI("Couldn't find '%s' socket; retrying after timeout", socket_name);
            }

            usleep(SOCKET_OPEN_RETRY_DELAY);
            retry_count++;

            continue;
        } else {
            break;
        }
    }
    return 0;

}

static int32_t client_init_receiver()
{
    int32_t ret = 0;
    void *p_record = NULL;
    size_t recordlen = 0;
    int32_t m_command_fd = -1;
    RecordStream *p_rs = NULL;

    const char *socket_name = SOCKET_NAME_DEFAULT;
    int32_t retry_count = 0;
    printf("orenda, %s entry\n", __func__);
    for(;;) {
        m_command_fd = socket_local_client(socket_name, ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
        if (m_command_fd < 0) {
            if (retry_count == 8) {
                ALOGE("Couldn't find '%s' socket after %d times, continuing to retry silently", socket_name, retry_count);
            } else if (retry_count >= 0 && retry_count < 8) {
                ALOGI("Couldn't find '%s' socket; retrying after timeout", socket_name);
            }

            usleep(SOCKET_OPEN_RETRY_DELAY);
            retry_count++;

            continue;
        }
        printf("orenda, %s entry\n", __func__);
        retry_count = 0;

        if (p_rs != NULL) {
            record_stream_free(p_rs);
            p_rs = NULL;
        }
        if (p_rs == NULL) {
            p_rs = record_stream_new(m_command_fd, MAX_COMMAND_BYTES);
        }

        for (;;) {
            ret = record_stream_get_next(p_rs, &p_record, &recordlen);
            //ALOGE("record_stream_get_next :ret=%d, recordlen:%zu",ret, recordlen);
            if (ret == 0 && p_record == NULL) {
                //ALOGE("orenda,not break in p_record=NULL");
                break;
            } else if (ret < 0 && errno != EAGAIN) {
                ALOGE("orenda, ret<0");
                break;
            } else if (ret == 0) {
                print_buf(p_record, recordlen);
            }
        }

        ALOGI("Disconnected from %s socket", socket_name);
        ALOGI("ret = %d, errno = %d", ret, errno);

        if (m_command_fd >= 0) {
            close(m_command_fd);
            m_command_fd = -1;
        }

        if (p_rs != NULL) {
            record_stream_free(p_rs);
            p_rs = NULL;
        }
    }
    return m_command_fd;
}

static int32_t client_init_receiver_bak()
{
    int32_t ret = 0;
    void *p_record = NULL;
    size_t recordlen = 0;
    int32_t m_command_fd = -1;
    RecordStream *p_rs = NULL;

    const char *socket_name = SOCKET_NAME_DEFAULT;
    int32_t retry_count = 0;
    printf("orenda, %s entry\n", __func__);
    for(;;) {
        m_command_fd = socket_local_client(socket_name, ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
        if (m_command_fd < 0) {
            if (retry_count == 8) {
                ALOGE("Couldn't find '%s' socket after %d times, continuing to retry silently", socket_name, retry_count);
            } else if (retry_count >= 0 && retry_count < 8) {
                ALOGI("Couldn't find '%s' socket; retrying after timeout", socket_name);
            }

            usleep(SOCKET_OPEN_RETRY_DELAY);
            retry_count++;

            continue;
        }
        printf("orenda, %s entry\n", __func__);
        retry_count = 0;

        if (p_rs != NULL) {
            record_stream_free(p_rs);
            p_rs = NULL;
        }
        if (p_rs == NULL) {
            p_rs = record_stream_new(m_command_fd, MAX_COMMAND_BYTES);
        }

        for (;;) {
            ret = record_stream_get_next(p_rs, &p_record, &recordlen);
            ALOGE("record_stream_get_next :ret=%d, recordlen:%zu",ret, recordlen);
            if (ret == 0 && p_record == NULL) {
                //ALOGE("orenda,not break in p_record=NULL");
                break;
            } else if (ret < 0 && errno != EAGAIN) {
                ALOGE("orenda, ret<0");
                break;
            } else if (ret == 0) {
                print_buf(p_record, recordlen);
            }
        }

        ALOGI("Disconnected from %s socket", socket_name);
        ALOGI("ret = %d, errno = %d", ret, errno);

        if (m_command_fd >= 0) {
            close(m_command_fd);
            m_command_fd = -1;
        }

        if (p_rs != NULL) {
            record_stream_free(p_rs);
            p_rs = NULL;
        }
    }
    return m_command_fd;
}

int _command_data_blocking_write(int32_t fd, const void *buffer, size_t len)
{
    size_t offset = 0;
    const uint8_t *wbuffer;

    wbuffer = (const uint8_t *)buffer;
    //printf("orenda, %s,%d, len=%d\n", __func__, __LINE__, len);
    while (offset < len) {
        ssize_t written = 0;
        do {
			printf("SEND:********\n");
            print_buf( wbuffer + offset, len - offset);
            //printf("orenda,%s,%d\n", __func__, __LINE__);
            written = write(fd, wbuffer + offset, len - offset);
			printf("send_buf end:------------\n");
        } while (written < 0 && ((errno == EINTR) || (errno == EAGAIN)));

        if (written >= 0) {
            offset += written;
        } else {
            // written < 0
            ALOGE("unexpected error on write errno:%d", errno);
            close(fd);
            return -1;
        }
    }

    return 0;
}

// server and client can use the same function.
int orenda_client_send(int32_t m_command_fd, void* data, size_t len)
{
    int ret;
	//char data[] = "abcde";
	size_t data_size, test_size;
	uint32_t cmdid = 0x1;
	//char p[4] = {0};
	uint32_t header = 0;
    

    if(m_command_fd<0) {
        ALOGE("m_command_fd<0, need init first\n");
    }
    int32_t fd = m_command_fd;
    if (fd < 0) {
        ALOGE("socket is invalid");
        return -1;
    }
	//if (data_size + sizeof(uint32_t) > MAX_COMMAND_BYTES) {
	if (data_size > MAX_COMMAND_BYTES) {
			ALOGE("packet larger than %u (%u)", MAX_COMMAND_BYTES, (unsigned int )data_size);
			return -1;
		}
/*
	p[0] = (cmdid >> 24) & 0xFF;
    p[1] = (cmdid >> 16) & 0xFF;
    p[2] = (cmdid >> 8) & 0xFF;
    p[3] = cmdid & 0xFF;
    */
	//data_size = sizeof(data);
	data_size = len;
    //printf("orenda, begin to send, data_size:%u\n",data_size);
    //header = htonl(data_size + sizeof(uint32_t));
    header = htonl(data_size );// header container the buffer size
   // printf("%u\n", header);
    test_size = ntohl(header);
    //printf("test_size:%d\n", test_size);
    ret = _command_data_blocking_write(fd, (void *)&header, sizeof(header));

	if (ret >= 0) {
        //ret = _command_data_blocking_write(fd, p, sizeof(uint32_t));
        ret |= _command_data_blocking_write(fd, (void*)data, data_size);
    }
	
    return ret;
}




