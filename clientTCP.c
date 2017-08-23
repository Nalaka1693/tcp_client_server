/* Sample TCP client */

/* Client sends the request string server respons with the file size 
   in bytes and create a new file client.txt and append the received
   data form server to file 
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1000

int main(int argc, char**argv) {
	int sockfd, n, itr, i = 0;
	struct sockaddr_in servaddr;
	char req[] = "request";
	char buffer[SIZE] = {};
	FILE *fp;

	if (argc != 2) {
		printf("usage: ./%s <IP address>\n", argv[0]);
		return -1;
	}

	/* socket to connect */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* IP address information of the server to connect to */
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(32000);

	connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	sendto(sockfd, req, strlen(req), 0, (struct sockaddr	*) &servaddr, sizeof(servaddr));

	n = recvfrom(sockfd, buffer, SIZE, 0, NULL, NULL);
	buffer[n] = 0; 

	printf("Received: Size of the file in bytes = %s\n", buffer); 	//printing the size of the file

	fp = fopen("clientfile.txt", "w+");								//create new fle on client side

	if (fp == NULL) {
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

	if (atoi(buffer) < SIZE) {
		itr = 1;
	} else {
		if (atoi(buffer) % (SIZE - 1)){
			itr = atoi(buffer) / (SIZE - 1);
		} else {
			itr = atoi(buffer) / (SIZE - 1) - 1;
		}
	}	

	int j = 1;
	for (; i < itr; i++) {
		n = recvfrom(sockfd, buffer, SIZE+1, 0, NULL, NULL);
		fputs(buffer, fp);									//append the file with receiving strings
	}

	shutdown(sockfd, 1);

	return 0;
}