#ifdef _WIN32
#define _WIN32_WINNT _WIN32_WINNT_WIN7 // select minimal legacy support, needed for inet_pton, inet_ntop
#include <winsock2.h>				   //for all socket programming
#include <ws2tcpip.h>				   //for getaddrinfo, inet_pton, inet_ntop
#include <stdio.h>					   //for fprintf
#include <unistd.h>					   //for close
#include <stdlib.h>					   //for exit
#include <string.h>					   //for memset
#include <time.h>					   //for rand value
#else
#include <sys/socket.h> //for sockaddr, socket, socket
#include <sys/types.h>	//for size_t
#include <netdb.h>		//for getaddrinfo
#include <netinet/in.h> //for sockaddr_in
#include <arpa/inet.h>	//for htons, htonl, inet_pton, inet_ntop
#include <errno.h>		//for errno
#include <stdio.h>		//for fprintf, perror
#include <unistd.h>		//for close
#include <stdlib.h>		//for exit
#include <string.h>		//for memset
#include <time.h>    	//for rand value
#endif

int main()
{
	char IP[] = "000.000.000.000";
	char Port[] = "00000";
	// Uncomment if youre working on a windows machine to test.
	/*
	WSADATA wsaData;
	WSAStartup( MAKEWORD(2,0), &wsaData );
	*/
	printf("Enter IP: ");
	scanf("%s", IP);
	
	printf("Enter Port: ");
	scanf("%s", Port);
	
	// Initialization
	srand(time(0));
	struct addrinfo internet_address_setup;
	struct addrinfo *internet_address = NULL;
	memset(&internet_address_setup, 0, sizeof internet_address_setup);
	internet_address_setup.ai_family = AF_INET;
	internet_address_setup.ai_socktype = SOCK_DGRAM;
	getaddrinfo(IP, "24042", &internet_address_setup, &internet_address);
	// Get me a damn socket! Grrrrrr!
	int internet_socket;
	internet_socket = socket(internet_address->ai_family, internet_address->ai_socktype, internet_address->ai_protocol);
	char str[8];
	sprintf(str, "%d", (rand()%100)+1);
	// Execution
	sendto(internet_socket, str, sizeof(str), 0, internet_address->ai_addr, internet_address->ai_addrlen);

	// Clean up
	freeaddrinfo(internet_address);
	close(internet_socket);

	// Uncomment if youre working on a windows machine to test.
	/*
	WSACleanup();
	*/

	return 0;
}
