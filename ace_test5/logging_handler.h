#include "ace/FILE_IO.h"
#include "ace/SOCK_Stream.h"
#include "ace/Message_Block.h"

class Logging_Handler
{
protected:
	ACE_FILE_IO &log_file_; // reference to log file
	ACE_SOCK_Stream logging_peer_; // connected to the client

public:
	Logging_Handler (ACE_FILE_IO &log_file) : log_file_ (log_file) {}
	int close() {return logging_peer_.close();} // close the connection socket

	/* receive a log from a connected client */
	int recv_log_record (ACE_Message_Block *&mblk);

	/* write one record to the log file */
	int write_log_record (ACE_Message_Block *mblk);

	/* log one record entry point */
	int log_record();

	ACE_SOCK_Stream &peer () {return logging_peer_;}
};

