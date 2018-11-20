#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <pthread.h>

#define EVER ;;

#define CHANNEL_ID 0
#define PRIORITY 1
#define SERVERINFO_PATH "/root/serverinfo.txt"

struct arg_struct { int channel_id; int priority; };

void create_serverinfo(process_id, channel_id) {
	FILE* file = fopen(SERVERINFO_PATH, "w");
	fprintf(file, "%d %d", process_id, channel_id);
	fclose(file);
}

int set_priority(int priority){
    int policy;
    struct sched_param param;

    if(priority < 1 || priority > 63){
        return -1;
    }

    pthread_getschedparam(pthread_self(), &policy, &param);
    param.sched_priority = priority;
    return pthread_setschedparam(pthread_self(), policy, &param);
}


int get_priority(){
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(), &policy, &param);
    return param.sched_curpriority;
}

void * client_fn(void * args) {
	int process_id, channel_id;
	FILE* file = fopen(SERVERINFO_PATH, "r");
	fscanf(file, "%d %d", &process_id, &channel_id);
	const int connection_id = ConnectAttach(0, process_id, channel_id, 0, 0);

	const int client_id = pthread_self();
	const int priority = *((int*) args);
	set_priority(priority);

	char send_buffer[] = "Message from client";
	char recv_buffer[1024];

	printf("[C%d (%d) send] %s\n", client_id, get_priority(), send_buffer);
	MsgSend(connection_id, send_buffer, sizeof(send_buffer), recv_buffer, sizeof(recv_buffer));
	printf("[C%d (%d) reply] %s\n", client_id, get_priority(), recv_buffer);

	return NULL;
}

void * server_fn(void * args) {
	int process_id, channel_id;
	FILE* file = fopen(SERVERINFO_PATH, "r");
	fscanf(file, "%d %d", &process_id, &channel_id);

	const int priority = *((int*) args);
	set_priority(priority);

	char recv_buffer[1024];
	char send_buffer[] = "Reply from server!";
	struct _msg_info msg_info;

	for(EVER) {
		printf("[SERVER (%d)] Waiting...\n", get_priority());
		const int msg_id = MsgReceive(channel_id, recv_buffer, sizeof(recv_buffer), &msg_info);
		printf("[SERVER (%d) recv_msg][tid: %d, pid: %d] (%s) \n", get_priority(), msg_info.tid, msg_info.pid, recv_buffer);
		MsgReply(msg_id, 0, send_buffer, sizeof(send_buffer));
		printf("[SERVER (%d) sent_msg][tid: %d, pid: %d] (%s) \n", get_priority(), msg_info.tid, msg_info.pid, send_buffer);
	}

	return NULL;
}

int main(int argc, char *argv[]) {
	set_priority(7);
	pthread_t thread_handles[5];
	const int channel_id = ChannelCreate(0);//_NTO_CHF_FIXED_PRIORITY);  // Channel used for communication
	const int process_id = getpid();

	create_serverinfo(process_id, channel_id);

	int priorities[5] = { 4, 3, 2, 5, 6 };
	pthread_create(&thread_handles[0], NULL, server_fn, &priorities[0] );
	pthread_create(&thread_handles[1], NULL, client_fn, &priorities[1] );
	pthread_create(&thread_handles[2], NULL, client_fn, &priorities[2] );
	pthread_create(&thread_handles[3], NULL, client_fn, &priorities[3] );
	pthread_create(&thread_handles[4], NULL, client_fn, &priorities[4] );

	set_priority(1);
	for(EVER);
	return EXIT_SUCCESS;
}
