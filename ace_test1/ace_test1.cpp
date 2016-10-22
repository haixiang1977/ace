/* show how ACE_SOCK_Connector can be used to connect a client application to a web server */
#include <iostream>

#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"

using namespace std;

int main()
{
	const char *pathname = "index.html";
	const char *server_hostname = "216.58.221.36"; // www.google.com
	                                               // here must use ip address not dns, or it will not able to set hostname

	ACE_SOCK_Connector connector;
	ACE_SOCK_Stream peer;

	/* according to ace-6.4.1 */
	ACE_INET_Addr peer_addr;

	if (peer_addr.set(80, server_hostname) == -1) {
		cout << "failed to set server_hostname " << server_hostname << endl;
		return 1;
	}

	if (connector.connect (peer, peer_addr) == -1) {
		cout << "failed to connect server_hostname" << endl;
		return 1;
	}

	cout << "connected to " << server_hostname << endl;
	cout << "connected to " << peer_addr.get_host_name() << endl;

	return 0;
}
