#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <syslog.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>

#define SOCKET_TARGET_PORT "9000"
#define BACKLOG 20
#define MSG_BUFFER_SIZE 1000

const char* TMP_FILE = "/var/tmp/aesdsocketdata";
int sockfd;
int accepted_fd;
char *msg = NULL;

bool server_is_running = true;

static void signal_handler(int signal_number)
{
	if ((signal_number == SIGINT) || (signal_number == SIGTERM))
	{
		server_is_running = false;
		free(msg);
		close(sockfd);
		close(accepted_fd);
		remove(TMP_FILE);
		syslog(LOG_DEBUG, "Killed aesdsocket");
		exit(EXIT_SUCCESS);
	}
	else
	{
		syslog(LOG_DEBUG, "Failed to kill aesdsocket");
		exit(EXIT_FAILURE);
	}
}

// adapted from https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux
static void start_daemon()
{
	pid_t pid;
	
	pid = fork();
	
	if (pid < 0)
	{
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		exit(EXIT_SUCCESS);
	}
	
	if (setsid() < 0)
	{
		exit(EXIT_FAILURE);
	}
	
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	
	pid = fork();
	if (pid < 0)
	{
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		exit(EXIT_SUCCESS);
	}
	
	umask(0);
	
	chdir("/");
	int x;
	for ( x = 2; x>=0; x--)
	{
		close(x);
	}
}


void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int argc, char *argv[])
{
	struct sockaddr_storage received_addr;
	socklen_t addr_size;
	struct addrinfo send_conf, *complete_conf;
	int status;
	char received_IP[INET6_ADDRSTRLEN];
	int msg_len = 0;
	int bytes_received = 0;
	char msg_buffer[MSG_BUFFER_SIZE];
	char output_buffer[MSG_BUFFER_SIZE];
	int bytes_read;
	bool is_receiving_message = true;
	FILE *fp;
	bool do_start_daemon = false;
	int opt;
	
	while ((opt = getopt(argc, argv, "d")) != -1)
	{
		if (opt == 'd')
		{
			do_start_daemon = true;
		}
	}
	
	memset(&send_conf, 0 , sizeof(send_conf));
	send_conf.ai_family = AF_UNSPEC;
	send_conf.ai_socktype = SOCK_STREAM;
	send_conf.ai_flags = AI_PASSIVE;
	
	status = getaddrinfo(NULL, SOCKET_TARGET_PORT, &send_conf, &complete_conf);
	
	if (status == -1)
	{
		perror("getaddrinfo failed: ");
		return -1;
	}
	
	sockfd = socket(complete_conf->ai_family, complete_conf->ai_socktype, complete_conf->ai_protocol);
	
	if (sockfd == -1)
	{
		perror("socket failed: ");
		return -1;
	}
	
	status = bind(sockfd, complete_conf->ai_addr, complete_conf->ai_addrlen);
	
	if (status == -1)
	{
		perror("bind failed: ");
		return -1;
	}
	
	if (do_start_daemon)
	{
		start_daemon();
	}
	else
	{
		signal(SIGINT, signal_handler);
		signal(SIGTERM, signal_handler);
	}
	
	
	
	syslog(LOG_DEBUG, "aesdsocket started daemon");
	
	freeaddrinfo(complete_conf);
	
	status = listen(sockfd, BACKLOG);
	
	if (status == -1)
	{
		perror("listen failed: ");
		return -1;
	}
	
	while (server_is_running)
	{
		addr_size = sizeof(received_addr);
		
		accepted_fd = accept(sockfd, (struct sockaddr *)&received_addr, &addr_size);
		
		if (accepted_fd == -1)
		{
			syslog(LOG_DEBUG, "aesdsocket failed to accept connection");
			perror("accept failed: ");
			continue;
		}
		
		inet_ntop(received_addr.ss_family, get_in_addr((struct sockaddr *) &received_addr), received_IP, sizeof(received_IP));
		syslog(LOG_DEBUG, "Accepted connection from %s\n", received_IP);
		if (!fork())
		{
			close(sockfd);
			
			while (is_receiving_message)
			{
				bytes_received = recv(accepted_fd, msg_buffer, MSG_BUFFER_SIZE, 0);
				if (bytes_received < 0)
				{
					perror("Bytes received: ");
					break;
				}
				else if (bytes_received == 0)
				{
					is_receiving_message = false;
				}
				else
				{
					
					msg = realloc(msg, msg_len + bytes_received);
				
					if (NULL == msg)
					{
						printf("Not enough memory\n\r");
					}
					else
					{
						for (int i = 0; i<bytes_received;i++)
						{
							msg[msg_len] = msg_buffer[i];
							++msg_len;
							if (msg_buffer[i] == '\n')
							{
								fp = fopen(TMP_FILE, "a+");
								fwrite(msg, sizeof(char), msg_len, fp);
								rewind(fp);
								while ((bytes_read = fread(output_buffer, 1, MSG_BUFFER_SIZE, fp)) > 0)
								{
									send(accepted_fd, output_buffer, bytes_read, 0);
								}
								fclose(fp);
								msg_len = 0;
							}
						}
					}
				}
			}
			
			
			syslog(LOG_DEBUG, "Closed Connection from %s\n", received_IP);
			close(accepted_fd);
			exit(0);
		}
		close(accepted_fd);
	}
	return 0;
}
