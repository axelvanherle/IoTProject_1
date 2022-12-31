#include <stdio.h>	//for fprintf, perror
#include <unistd.h> //for close
#include <stdlib.h> //for exit
#include <string.h> //for memset
#include <pthread.h>

//

#ifdef _WIN32
#include <winsock2.h> //for all socket programming
#include <ws2tcpip.h> //for getaddrinfo, inet_pton, inet_ntop
void OSInit(void)
{
	WSADATA wsaData;
	int WSAError = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (WSAError != 0)
	{
		fprintf(stderr, "WSAStartup errno = %d\n", WSAError);
		exit(-1);
	}
}
#define perror(string) fprintf(stderr, string ": WSA errno = %d\n", WSAGetLastError())
#else
#include <sys/socket.h> //for sockaddr, socket, socket
#include <sys/types.h>	//for size_t
#include <netdb.h>		//for getaddrinfo
#include <netinet/in.h> //for sockaddr_in
#include <arpa/inet.h>	//for htons, htonl, inet_pton, inet_ntop
#include <errno.h>		//for errno
#endif

int counter = 0;
int avgVal = 0;

int initializationServer(void);
void executionServer(int internet_socket);

// thread used to send messages.
void *sendThread()
{
	// This is temporary for testing purposes, the IP and port should be hardcoded.
	char IP[] = "000.000.000.000";
	char Port[] = "00000";
	printf("Enter IP to send to: ");
	scanf("%s", IP);
	printf("Enter Port to send to: ");
	scanf("%s", Port);

	while (1)
	{
		if (counter == 2)
		{
			#ifdef _WIN32
				WSADATA wsaData;
				WSAStartup(MAKEWORD(2, 0), &wsaData);
			#else
			// nothing on linux
			#endif

			struct addrinfo internet_address_setup;
			struct addrinfo *internet_address = NULL;
			memset(&internet_address_setup, 0, sizeof internet_address_setup);
			internet_address_setup.ai_family = AF_INET;
			internet_address_setup.ai_socktype = SOCK_DGRAM;

			getaddrinfo(IP, Port, &internet_address_setup, &internet_address);

			int internet_socket;
			internet_socket = socket(internet_address->ai_family, internet_address->ai_socktype, internet_address->ai_protocol);

			char buffer[] = "0000";
			sprintf(buffer, "%d", avgVal/2);

			sendto(internet_socket, buffer, strlen(buffer), 0, internet_address->ai_addr, internet_address->ai_addrlen);
			printf("\n========\nSENT %d TO %s ON %s\n========\n", avgVal/2,IP,Port);
			freeaddrinfo(internet_address);
			close(internet_socket);

			#ifdef _WIN32
				WSACleanup();
			#else
				// nothing on linux
			#endif
			counter = 0;
			avgVal = 0;
		}
		else if (counter > 2)
		{
			// Something went wrong.
			counter = 0;
			avgVal = 0;
		}
	}
}

// thread used to receive messages.
void *receiveThread()
{
	// Code for the server
	// Get me a socket! Grrrrr!
	int internet_socket_server = initializationServer();
	while (1)
		executionServer(internet_socket_server);

	close(internet_socket_server);
}

int main(int argc, char *argv[])
{
	// Initialization
	#ifdef _WIN32
		OSInit();
	#else
	// Do nothing on linux
	#endif

	// Create my threads and run those suckers.
	pthread_t p1, p2;
	if (pthread_create(&p1, NULL, &sendThread, NULL) != 0)
	{
		return 1;
	}
	if (pthread_create(&p2, NULL, &receiveThread, NULL) != 0)
	{
		return 2;
	}

	if (pthread_join(p1, NULL) != 0)
	{
		return 3;
	}
	if (pthread_join(p2, NULL) != 0)
	{
		return 4;
	}

	#ifdef _WIN32
		WSACleanup();
	#else
	// Do nothing on linux
	#endif

	return 0;
}

int initializationServer()
{
	struct addrinfo internet_address_setup;
	struct addrinfo *internet_address_result;
	memset(&internet_address_setup, 0, sizeof internet_address_setup);
	internet_address_setup.ai_family = AF_INET;
	internet_address_setup.ai_socktype = SOCK_DGRAM;
	internet_address_setup.ai_flags = AI_PASSIVE;
	int getaddrinfo_return = getaddrinfo(NULL, "24042", &internet_address_setup, &internet_address_result);
	if (getaddrinfo_return != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(getaddrinfo_return));
		exit(1);
	}

	int internet_socket = -1;
	struct addrinfo *internet_address_result_iterator = internet_address_result;
	while (internet_address_result_iterator != NULL)
	{
		internet_socket = socket(internet_address_result_iterator->ai_family, internet_address_result_iterator->ai_socktype, internet_address_result_iterator->ai_protocol);
		if (internet_socket == -1)
		{
			perror("socket");
		}
		else
		{
			int bind_return = bind(internet_socket, internet_address_result_iterator->ai_addr, internet_address_result_iterator->ai_addrlen);
			if (bind_return == -1)
			{
				close(internet_socket);
				perror("bind");
			}
			else
			{
				break;
			}
		}
		internet_address_result_iterator = internet_address_result_iterator->ai_next;
	}

	freeaddrinfo(internet_address_result);

	if (internet_socket == -1)
	{
		fprintf(stderr, "socket: no valid socket address found\n");
		exit(2);
	}

	return internet_socket;
}

void executionServer(int internet_socket)
{
	int number_of_bytes_received = 0;
	char buffer[1000];
	struct sockaddr_storage client_internet_address;
	socklen_t client_internet_address_length = sizeof client_internet_address;
	number_of_bytes_received = recvfrom(internet_socket, buffer, (sizeof buffer) - 1, 0, (struct sockaddr *)&client_internet_address, &client_internet_address_length);
	if (number_of_bytes_received == -1)
	{
		perror("recvfrom");
	}
	else
	{
		buffer[number_of_bytes_received] = '\0';
		printf("Received : %s\n", buffer);
		counter += 1;
		avgVal += atoi(buffer);
		printf("AVG VAL : %d\n", avgVal);
	}
}
