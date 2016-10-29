#ifndef _LOGGING_SERVER_H
#define _LOGGING_SERVER_H

#include "ace/FILE_IO.h"
#include "ace/SOCK_Acceptor.h"

class ACE_SOCK_Stream;

class Logging_Server {
public:
	// close the listen socket (not connection socket)
	virtual ~Logging_Server() {acceptor_.close();}

	// Template Method that runs logger server's event loop
	virtual int run (int argc, char *argv[]);

	// the following 4 methods can be overridden by subclass
	virtual int open (u_short port = 0);
	virtual int wait_for_multiple_events () {return 0;}
	virtual int handle_connection() = 0;
	virtual int handle_data (ACE_SOCK_Stream* = 0) = 0;

	int make_log_file (ACE_FILE_IO &);

	// Accessor
	ACE_SOCK_Acceptor &acceptor () {return acceptor_;}

private:
	//Socket acceptor
	ACE_SOCK_Acceptor acceptor_;
};
#endif /* _LOGGING_SERVER_H */

