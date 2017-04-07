#include <thread>
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

void listen_messages(int sock);

int main()
{
	char buffer[1024];

	int my_sock = socket(AF_INET,SOCK_STREAM,0);

	sockaddr_in addr;

	if(my_sock < 0)
	{
		cout << "Error socket!" << endl;

		sleep(5);

		return 0;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(my_sock,(sockaddr*)&addr,sizeof(addr)) < 0)
	{
		cout << "Error connect!" << endl;

		sleep(5);

		return 0;
	}

	thread t(listen_messages,my_sock);
	t.detach();

	while(1)
	{
		cin.getline(buffer,1024);
		send(my_sock,buffer,1024,0);
	}

	close(my_sock);
	
	return 0;
}

void listen_messages(int sock)
{
	char *buffer = new char[1024];

	while(1)
	{
		recv(sock,buffer,1024,0);

		cout << buffer << endl;
	}

	delete buffer;
}