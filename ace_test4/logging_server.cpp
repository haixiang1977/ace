#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "logging_server.h"

/* handle in sequence */
int Logging_Server::run (int argc, char *argv[])
{
	if (open (argc > 1 ? atoi (argv[1]) : 0) == -1)
		return -1;

	for (;;) {
		if (wait_for_multiple_events () == -1) return -1;
		if (handle_connection() == -1) return -1;
		if (handle_data() == -1) return -1;
	}
	return 0;
}

int Logging_Server::open(u_short logger_port) {
	ACE_INET_Addr server_addr;
	int result;

	result = server_addr.set (logger_port, INADDR_ANY);

	if (result == -1) return -1;

	// start listening
	result = acceptor_.open(server_addr, 1);

	return result;
}

int Logging_Server::make_log_file (ACE_FILE_IO &logging_file) {

	char filename [256];
	ACE_FILE_Connector connector;
	int fd;

	strcpy (filename, "logging_server.log");
	fd = connector.connect(logging_file,
			               ACE_FILE_Addr (filename),
			               0,
			               ACE_Addr::sap_any,
			               0,
			               O_RDWR|O_CREAT|O_APPEND,
			               ACE_DEFAULT_FILE_PERMS);

	return fd;
}
