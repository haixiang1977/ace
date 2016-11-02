#include <iostream>
#include "logging_handler.h"

using namespace std;

/* receive log from remote client */
int Logging_Handler::recv_log_record(ACE_Message_Block *&mblk)
{
	ACE_INET_Addr peer_addr;
	ACE_Message_Block *payload = new ACE_Message_Block (1024);
	size_t bytes_transferred;

	cout << "Logging_Handler::recv_log_record receiving data from client" << endl;

	/* put client host name into message block */
	logging_peer_.get_remote_addr(peer_addr);
	mblk = new ACE_Message_Block(64);
	peer_addr.get_host_name(mblk->wr_ptr(), 64); // store the host name into message blk
	mblk->wr_ptr(strlen(mblk->wr_ptr()) + 1); // move ptr to next

	/* put received client log into message block */
	if (logging_peer_.recv_n(payload->wr_ptr(), 56, 0, &bytes_transferred) > 0) {
		payload->wr_ptr(bytes_transferred);
		mblk->cont(payload);
		return bytes_transferred;
	}

	payload->release();
	mblk->release();
	payload = mblk = 0;

	return -1;
}

/* write log from message block to the file */
int Logging_Handler::write_log_record(ACE_Message_Block *mblk)
{
	cout << "Logging_Handler::write_log_record receiving data from client" << endl;
	// Peer hostname is in the <mblk> and the log record data
	// is in its continuation.
	if (log_file_.send_n (mblk) == -1)
	    return -1;
	return (mblk->total_length ());
}

/* write log entry function */
int Logging_Handler::log_record()
{
	ACE_Message_Block *mblk = 0;
	int result;

	if (recv_log_record (mblk) == -1) return -1;

	result = write_log_record (mblk);
	mblk->release();

	return ((result == -1) ? -1 : 0);
}
