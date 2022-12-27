#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345
#define BUFLEN 1024

int main(void) {
int sockfd;
struct sockaddr_in servaddr;
char buf[BUFLEN];

// Maak een socket
sockfd = socket(AF_INET, SOCK_DGRAM, 0);
if (sockfd < 0) {
perror("Aanmaken socket gefaald");
exit(EXIT_FAILURE);
}

// Initialiseer socket address
memset(&servaddr, 0, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_port = htons(PORT);
servaddr.sin_addr.s_addr = INADDR_ANY;

// Stuur random waarde naar de server
while (1) {
int randval = rand();
sprintf(buf, "%d", randval);
sendto(sockfd, buf, strlen(buf), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
printf("Stuur random waarde: %d\n", randval);
sleep(1);
}

close(sockfd);
return 0;
}