#include "ace/OS.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Record.h"
#include "ace/streams.h"

class Logging_Client
{
public:
	int send (const ACE_Log_Record &log_reord); // send the log to the server
	ACE_SOCK_Stream &peer () {return logging_peer_;}
	~ Logging_Client() {logging_peer_.close();}
private:
	ACE_SOCK_Stream logging_peer_; // connected to the server
};

int Logging_Client::send (const ACE_Log_Record &log_record) {
	const ACE_TCHAR *buf;
	size_t len;
	size_t bytestransferred;

	int result;

	buf = log_record.msg_data();
	len = log_record.length();

	result = logging_peer_.send_n(buf, len, 0, &bytestransferred); /* buffer point and length */
	ACE_DEBUG((LM_DEBUG, "bytestransferred %d bytes\n", bytestransferred));

	return result;
}

int main (int argc, char *argv[])
{
	u_short logger_port = argc > 1 ? atoi (argv[1]) : 5555;
	const char *logger_host = argc > 2 ? argv[2] : "127.0.0.1";
	int result;

	ACE_INET_Addr server_addr;

	ACE_SOCK_Connector connector;
	Logging_Client logging_client;

	result = server_addr.set (logger_port, logger_host);
	if (result == -1) {
		ACE_ERROR_RETURN((LM_ERROR, "loopup %s, %p\n", logger_port, logger_host), -1);
	}

	result = connector.connect (logging_client.peer(), server_addr);
	if (result < 0) {
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "connect()"), -1);
	}

	// start to collect the log
	{
		ACE_TCHAR* user_input = const_cast<ACE_TCHAR*> ("hello world from client");
		ACE_Time_Value now (ACE_OS::gettimeofday());
		ACE_Log_Record log_record (LM_INFO, now, ACE_OS::getpid());

		log_record.msg_data (user_input);

		result = logging_client.send(log_record);
		if (result == -1) {
			ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "logging_client.send()"), -1);
		}
	}

	return 0;
}
