#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345
#define Waarde 1024

int main(void) {
int sockfd;
struct sockaddr_in servaddr, cliaddr;
char buf[Waarde];
socklen_t len;

// socket aangemaakt
sockfd = socket(AF_INET, SOCK_DGRAM, 0);
if (sockfd < 0) {
perror("Aanmaken socket gefaald");
exit(EXIT_FAILURE);
}

// socket address initallizeren
memset(&servaddr, 0, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = INADDR_ANY;
servaddr.sin_port = htons(PORT);

// socket binden aan address
if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
perror("Binden gefaald");
exit(EXIT_FAILURE);
}

// random waarde sturen naar de client
while (1) {
int randval = rand();
sprintf(buf, "%d", randval);
sendto(sockfd, buf, strlen(buf), 0, (const struct sockaddr *)&cliaddr, len);
printf("Stuur random waarde: %d\n", randval);
sleep(1);
}

close(sockfd);
return 0;
}