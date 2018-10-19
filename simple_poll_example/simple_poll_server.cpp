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

#ifdef NON_BLOCKMODE_TEST
#if 0 // to make accept 
    int flags;
    flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
#endif
#endif

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
			/* poll socket to see whether have space to write or have data to read */
            /* initialize the pollfd structure */
            struct pollfd fd;

            memset(&fd, 0 , sizeof(fd));
            fd.fd = connfd;
            fd.events = POLLIN | POLLOUT;
            while (1) {
                /* start to poll now */
                int ret = poll(&fd, 1, 1000); // poll every 1 second
                if (ret < 0) {
                    printf("server poll error happen\n");
                    goto err;
                } else if (ret == 0) {
                    //printf("server poll timeout\n");
                    continue;
                } else {
                    /* check events */
                    if (fd.revents & POLLIN) {
                        printf("have data available to receive\n");
                        /* receive data from socket */
                        char buf[256];
                        memset(buf, '\0', 256);
                        int num = recv(connfd, buf, 256, 0);
                        if (num == 0) {
                            printf("connection closed by client\n");
                            goto err;
                        }
                        printf ("server num_read %d : %s\n", num, buf);
                    }
                    if (fd.revents & POLLOUT) {
#ifdef NON_BLOCKMODE_TEST
                        static int count = 0;
                        printf("have room available to send %d\n", ++count);
#else
                        printf("have room available to send\n");
#endif
                        /* send data to socket */
                        /* The default and maximum amount for the send socket memory */
                        /* cat /proc/sys/net/core/wmem_default = 112640 (110K) */
                        /* cat /proc/sys/net/core/wmem_max = 131071 (128K) */
                        char buf[1024 * 256];
                        memset(buf, '\0', 1024 * 256);
                        printf("server sending 256KB ...\n");
                        /* if client not process data from socket, the send will block here in default blocking mode */
                        int num = send(connfd, buf, 1024 * 256, 0);
                        printf ("server num_send %d\n", num);
                    }
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
