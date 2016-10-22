/* TCP server example as Linux socket with C */
/* From Unix network programming volumn 1 */
/* client read a line from input and send to the server */
/* server read this line from network input and send back to the client */
/* client read this line from network input and send back to the output */
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
			/* read data from connect socket first */
			memset (buf, '\0', sizeof(buf));
			while (num_read == 0) {
				num_read = read (connfd, buf, sizeof(buf));
			}
			printf ("server num_read %d\n", num_read);
			printf ("server message %s\n", buf);
			strcpy (buf, "hello from server\n");
			num_written = write (connfd, buf, strlen(buf));
			printf ("server num_written %d\n", num_written);
		}
		/* 5. close the connection */
		close (connfd);
	}

	return 0;
err:
	return -1;
}
