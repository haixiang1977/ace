#include "ace/FILE_IO.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "logging_server.h"
#include "logging_handler.h"

class Iterative_Logging_Server : public Logging_Server
{
protected:
	ACE_FILE_IO log_file_;
	Logging_Handler logging_handler_;

public:
	Iterative_Logging_Server(): logging_handler_ (log_file_) {}
	virtual ~Iterative_Logging_Server() {log_file_.close();}

	Logging_Handler &logging_handler() {return logging_handler_;}

	virtual int open (u_short port);
	virtual int handle_connection();
	virtual int handle_data (ACE_SOCK_Stream *);
};
