
#define SOCKET_NAME_DEFAULT "ORENDA_SOCKET_DEFAULT"
#define SOCKET_OPEN_RETRY_DELAY (1000*1000)
// ????
#define MAX_COMMAND_BYTES (150 * 1024)

#define ALOGE printf
#define ALOGI printf

typedef struct server_socket{
int32_t fd;
void* buffer_get[MAX_COMMAND_BYTES];

}server_socket_t;

typedef struct client_socket{
int32_t m_command_fd;
void* buffer_get[MAX_COMMAND_BYTES];

}client_socket_t;



int print_buf(void* buf, int len);
int orenda_client_send(int32_t m_command_fd, void* data, size_t len);
int client_start_recv(client_socket_t* m_client_socket_t);
int32_t client_init_fd(client_socket_t* m_client_socket_t);
int32_t client_deinit_fd(client_socket_t* m_client_socket_t);



void server_init(server_socket_t* m_server_socket_t);
int server_send_msg(server_socket_t* m_server_socket_t,void* buffer, size_t len);
int server_recv_msg(server_socket_t* m_server_socket_t);

int server_deinit(server_socket_t* m_server_socket_t);

