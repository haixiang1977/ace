#include <iostream>
#include "ace/OS.h"
#include "ace/Message_Block.h"

using namespace std;

#define BUF_SIZE 5

int main (int argc, char *argv[])
{
	ACE_Message_Block *head = new ACE_Message_Block (BUF_SIZE);
	ACE_Message_Block *mblk = head;
	size_t bytes_transferred;

	int i;

	for (i = 0; i < 2; i++) {
		//cout << "read from stdin" << endl;
		size_t nbytes = ACE_OS::read_n (ACE_STDIN,
				                     mblk->wr_ptr(),
				                     mblk->size(),
				                     &bytes_transferred);
        //cout << "read nbytes " << nbytes << "bytes_transferred " << bytes_transferred << endl;

		// Advance the write pointer to the end of the buffer
		mblk->wr_ptr (nbytes);

		// Allocate 2nd message block and chain it at the end of list
		mblk->cont(new ACE_Message_Block (BUF_SIZE));
		mblk = mblk->cont(); // move to the next message block
	}

	// Print the contents of the list
	for (mblk = head; mblk != 0; mblk = mblk->cont()) {
		ACE_OS::write_n (ACE_STDOUT, mblk->rd_ptr(), mblk->length());
	}

	ACE_OS::sleep((u_int) 1);
	head->release();
	return 0;
}
