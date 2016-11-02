#include "ace/Handle_Set.h"
#include "iterative_logging_server.h"

class Reactive_Logging_Server : public Iterative_Logging_Server
{
protected:
	// keep track of the acceptor socket handle and all connected stream socket handles
	ACE_Handle_Set master_handle_set_;

	// keep track of handles marked as active by select
	ACE_Handle_Set active_handles_;

public:
	// override the functions defined in iteractive_logging_server.h
	virtual int open (u_short port);
	virtual int wait_for_multiple_events ();
	virtual int handle_connection();
	virtual int handle_data (ACE_SOCK_Stream *);
};
