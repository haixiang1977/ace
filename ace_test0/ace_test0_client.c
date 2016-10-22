/* TCP client example as Linux socket with C */
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
#include <arpa/inet.h>

int main ()
{
	int sockfd; /* socket fd */
	struct sockaddr_in servaddr;
	char buf[256];
	int num_read = 0;
	int num_written = 0;

	/* 1. create socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0); /* IPV4 TCP Socket */
	if (sockfd < 0) {
		printf ("client failed to create TCP socket\n");
		goto err;
	}

	/* 2. prepare server information */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons (5000);

	/* convert 127.0.0.1 into servaddr */
	if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr.s_addr) < 0) { /* see man inet_pton */
		printf ("client failed to inet_pton 127.0.0.1 \n");
		goto err;
	}

	/* 3. connect to the server */
	if (connect (sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		printf ("client failed to connect the server\n");
	} else {
		printf ("client connected to the server\n");
	}

	/* 4. send message to the server */
	strcpy (buf, "hello from client\n");
	num_written = write (sockfd, buf, strlen(buf));
	printf ("client num_written %d\n", num_written);
	while (num_read == 0) {
		num_read = read (sockfd, buf, sizeof(buf));
	}
	printf ("client num_read %d\n", num_read);
	printf ("client received %s", buf);

	return 0;
err:
	return -1;
}
