#include "ace/Log_Msg.h"
#include "Reactive_Logging_Server.h"

int Reactive_Logging_Server::open(u_short port) {
	Iterative_Logging_Server::open(port);

	master_handle_set_.set_bit(acceptor().get_handle()); // enables the listen socket hadnle
	acceptor().enable(ACE_NONBLOCK);

	return 0;
}

int Reactive_Logging_Server::wait_for_multiple_events() {
	active_handles_ = master_handle_set_;
	int width = (int) active_handles_.max_set() + 1;
	// the select() succeeds, the number of active handles is returned
	// and the active_handles_.fdset() is modified to indicate each handle
	// that's now active
	if (select(width, active_handles_.fdset(), 0, 0, 0) == -1) {
		return -1;
	}
	// sync() resets the handle count and size-related values in active_handles
	// to reflect the changes made by select()
	active_handles_.sync(active_handles_.max_set() + 1);
	return 0;
}

int Reactive_Logging_Server::handle_connection() {
	if (active_handles_.is_set(acceptor().get_handle())) { // new connections to listen socket
		while (acceptor().accept(logging_handler().peer()) == 0) { // accept the connection
			master_handle_set_.set_bit(logging_handler().peer().get_handle()); // set connection socket into select
		}
		// clear the active flag for listen socket and wait for next connection
		active_handles_.clr_bit(acceptor().get_handle());
	}
	return 0;
}

int Reactive_Logging_Server::handle_data (ACE_SOCK_Stream *) {
	ACE_Handle_Set_Iterator peer_iterator (active_handles_); //iterator of the ACE Handle Set

	for (ACE_HANDLE handle; (handle = peer_iterator()) != ACE_INVALID_HANDLE;) {
		/* since in handle_connection() the listen socket fd has been cleared */
		/* here active handle must be connection fd */
		logging_handler ().peer ().set_handle (handle);
		if (logging_handler().log_record() == -1) {
			// Handle connection shutdown or comm failure.
			master_handle_set_.clr_bit (handle);
			logging_handler ().close ();
		} else {
			// after log the file, clear the bit
			master_handle_set_.clr_bit (handle);
		}
	}

	return 0;
}
