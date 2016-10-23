/* the capability to tell kernel that we want to be notified if one or more */
/* I/O conditions are ready. This capability is called I/O multiplexing and */
/* provided by select() and poll () */
/* I/O multiplexing is typically used in network applications in the follow scenarios: */
/* 1. when a client handling multiple descriptors, normally interactive input and network socket */
/* 2. it is possible but rare, client handle multiple sockets at the same time */
/* 3. if a TCP server handle both listening socket and connected socket */
/* 4. if a server handle TCP and UDP */
/* select function: allows the process to instruct the kernel to wait for any */
/* one of multiple events to occur and only wake up the process only when one */
/* or more of these event occurs or when a specified amount of time passed */

/* this is a server example to select both socket fd and connect fd */
/* the code logic of select is as below: */
/* FD_ZERO (selectfd) */
/* FD_SET (fd, &selectfd) */
/* select (FD_SETSIZE, selectfd, NULL, NULL, NULL) */
/* FD_ZERO (fd, &selectfd) if want to remove */
/* FD_SET (new_fd, &selectfd) if want to add */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
	int sockfd; /* socket fd */
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
	socklen_t cliaddrlen;
	int connfd; /* connection fd */

	fd_set active_fd_set, read_fd_set;
	int i;

	char buf[256];
	int num_read = 0;

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
	servaddr.sin_port = htons (5000);
	if (bind (sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		printf ("server failed to bind\n");
		goto err;
	}

	/* 3. start listen to the socket */
	if (listen (sockfd, 128) < 0) {
		printf ("server failed to listen\n");
		goto err;
	} else {
		printf ("server start to listen at port 5000\n");
	}

	/* 4. Initialize the set of active sockets. At the beginning, there is no connect fd and only socketfd */
	FD_ZERO (&active_fd_set);
	FD_SET (sockfd, &active_fd_set);

	/* while loop to handle coming request */
	while (1) {
		/* 4. select and only care read fd */
		read_fd_set = active_fd_set;
		if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
			printf ("server failed to select\n");
			goto err;
		}

		/* check all the fdset and see which is active */
		for (i = 0; i < FD_SETSIZE; i++) {
			if (FD_ISSET (i, &read_fd_set)) { /* FD_ISSET means active */
				if (i == sockfd) { /* new connection request coming just accept */
					printf ("server sockfd %d select new connection request and accept \n", sockfd);
					cliaddrlen = sizeof (cliaddr);
					connfd = accept(sockfd, (struct sockaddr *) &cliaddr, &cliaddrlen);
					if (connfd < 0) {
						printf ("server failed to accept\n");
						goto err;
					} else {
						/* add connect fd into active fd list for later select */
						FD_SET (connfd, &active_fd_set);
					}
				} else { /* must be connect fd to indicate have data to read/write availale */
					connfd = i;
					printf ("server select connfd %d data available to read\n", connfd);
					/* read data */
					memset (buf, '\0', sizeof(buf));
					while (num_read == 0) {
						num_read = read (connfd, buf, sizeof(buf));
					}
					printf ("server num_read %d\n", num_read);
					printf ("server message %s\n", buf);
					/* close connect fd */
					close (connfd);
					/* clear the connect fd from active fd list since connection closed */
					FD_CLR (connfd, &active_fd_set);
				}
			}
		}
	}

	return 0;
err:
	return -1;
}
