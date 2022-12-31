#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h> //for all socket programming
#include <ws2tcpip.h> //for getaddrinfo, inet_pton, inet_ntop
#include <unistd.h> //for close
#else
#include <sys/socket.h> //for sockaddr, socket, socket
#include <sys/types.h> //for size_t
#include <netdb.h> //for getaddrinfo
#include <netinet/in.h> //for sockaddr_in
#include <arpa/inet.h> //for htons, htonl, inet_pton, inet_ntop
#include <errno.h> //for errno
#include <unistd.h> //for close
#endif

int main()
{
	printf("Welcome to randValToCClient.\n");
	// This is temporary for testing purposes, the IP and port should be hardcoded.
	char IP[] = "000.000.000.000";
	char Port[] = "00000";
	printf("Enter IP: ");
	scanf("%s", IP);
	printf("Enter Port: ");
	scanf("%s", Port);

	// This is used to we can set how long we want the program to run.
	// Enter time in seconds.
	int timeToRun = 5 * 60;
	// Frequency at which we want to send a packet in seconds.
	int packetFrequency = 30;

	// Seed the rand function
	srand(time(0));

	// Add a timer to the program.
	clock_t timer_start, timer_end;
	timer_start = clock();

	while (timeToRun >= packetFrequency)
	{
		#ifdef _WIN32
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 0), &wsaData);
		#else
			// Empty for linux.
		#endif

		// Set up the struct
		struct addrinfo internet_address_setup;
		struct addrinfo *internet_address = NULL;
		memset(&internet_address_setup, 0, sizeof internet_address_setup);

		// We want IPV4 and UDP.
		internet_address_setup.ai_family = AF_INET;
		internet_address_setup.ai_socktype = SOCK_DGRAM;
		getaddrinfo(IP, Port, &internet_address_setup, &internet_address);

		// Get me a damn socket! Grrrrrr!
		int internet_socket = socket(internet_address->ai_family, internet_address->ai_socktype, internet_address->ai_protocol);

		// Prepare the random value to be sent over UDP.
		char str[4];
		sprintf(str, "%d", (rand() % 100) + 1);

		// Send my packet!
		sendto(internet_socket, str, strlen(str), 0, internet_address->ai_addr, internet_address->ai_addrlen);

		// Clean up
		freeaddrinfo(internet_address);
		close(internet_socket);

		#ifdef _WIN32
			WSACleanup();
		#else
			// Empty for linux.
		#endif

		timeToRun -= packetFrequency;
		sleep(packetFrequency);
	}

	timer_end = clock(); // End the timer.
	double time_spent = (double)(timer_end - timer_start) / CLOCKS_PER_SEC;
	printf("\nProgram stopped, it ran for %.3fs.\n", time_spent);
	return 0;
}
