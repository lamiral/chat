#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <thread>

using namespace std;

vector <int> users;


void listen_message();

int main()
{
	cout << "Start server..." << endl;

	int listen_socket = 0,user = 0;

	sockaddr_in addr;

	listen_socket = socket(AF_INET,SOCK_STREAM,0);

	if(listen_socket < 0)
	{
		cout << "Error lisen socket!" << endl;


		return 0;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(listen_socket,(sockaddr*)&addr,sizeof(addr)) < 0)
	{
		cout << "Error bind!" << endl;

		return 0;
	}

	listen(listen_socket,20);

	while(user = accept(listen_socket,NULL,NULL))
	{
		cout << "New user connected..." << endl;

		users.push_back(user);

		thread th(listen_message);

		th.detach();
	}

	clean_sockets();
	
	return 0;
}

void clean_sockets()
{
	short size = users.size();

	for(short i = 0;i < size;++i)
	{
		close(users[i]);
	}
}

void listen_message()
{
	int my_sock = users[users.size()-1];

	char *buffer = new char [1024];

	while(1)
	{
		recv(my_sock,buffer,1024,0);

		short i = 0;
		short size = users.size();

		while(i < size)
		{
			send(users[i],buffer,1024,0);
			++i;
		}
	}

	close(my_sock);
	delete buffer;
}