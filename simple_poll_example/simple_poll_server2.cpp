/* TCP server example as Linux socket with C */
/* From Unix network programming volumn 1 */
/* server poll operation to wait for receive socket and send socket */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h> /* Added for the nonblocking socket */
#include <errno.h>

#define NON_BLOCKMODE_TEST 1

int main() {
	int sockfd; /* socket fd */
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
	socklen_t cliaddrlen;
	int connfd; /* connection fd */

	char buf[256];
	int num_read = 0;
	int num_written = 0;

	/* 1. create a socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0); /* IPV4 TCP Socket */
	if (sockfd < 0) {
		printf ("server failed to create TCP socket\n");
		goto err;
	}

	/* 2. bind socket to the port */
	/* bind assign a local protocol address to the socket */
	/* local protocol address = local IP + port number */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_port = htons (50000);
	if (bind (sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		printf ("server failed to bind\n");
		goto err;
	}

	/* 3. start listen to the socket */
	if (listen (sockfd, 128) < 0) {
		printf ("server failed to listen\n");
		goto err;
	} else {
		printf ("server start to listen at port 50000\n");
	}


	/* while loop to handle coming request */
	while (1) {
		cliaddrlen = sizeof (cliaddr);
		/* 4. wait for the client request and process */
		connfd = accept(sockfd, (struct sockaddr *) &cliaddr, &cliaddrlen);
		if (connfd < 0) {
			printf ("server failed to accept\n");
			goto err;
		} else {
			printf ("coming connection fd %d connection\n", connfd);
#ifdef NON_BLOCKMODE_TEST
            int flags;
            flags = fcntl(connfd, F_GETFL, 0);
            fcntl(connfd, F_SETFL, O_NONBLOCK);
#endif
            while (1) {
                /* send data to socket */
                /* The default and maximum amount for the send socket memory */
                /* cat /proc/sys/net/core/wmem_default = 112640 (110K) */
                /* cat /proc/sys/net/core/wmem_max = 131071 (128K) */
                char buf[1024 * 256];
                memset(buf, '\0', 1024 * 256);
                printf("server sending 256KB ...\n");
                /* if client not process data from socket, the send will block here in default blocking mode */
                int num = send(connfd, buf, 1024 * 256, 0);
                if (num > 0) {
                    printf("server num_send %d\n", num);
                } else {
                    /* EAGAIN Resource temporarily unavailable (may be the same value as EWOULDBLOCK)*/
                    printf("server num_send %d err %d %s\n", num, errno, strerror(errno));
                }
            }
		}
		/* 5. close the connection */
		close (connfd);
	}

	return 0;
err:
	return -1;
}
