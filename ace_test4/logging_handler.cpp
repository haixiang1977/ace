#include <iostream>
#include "logging_handler.h"

using namespace std;

/* receive log from remote client */
int Logging_Handler::recv_log_record(ACE_Message_Block *&mblk)
{
	cout << "Logging_Handler::recv_log_record receiving data from client" << endl;

	return 0;
}

/* write log from message block to the file */
int Logging_Handler::write_log_record(ACE_Message_Block *mblk)
{
	cout << "Logging_Handler::write_log_record receiving data from client" << endl;

	return 0;
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
