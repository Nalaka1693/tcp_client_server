/* Sample TCP server */

/* Client sends the request string server respons with the file size 
   in bytes and create a new file client.txt and append the received
   data form server to file 
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SIZE 1000

int main(int argc,  char**argv) {
	int listenfd, connfd, n;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t clilen;
	char buffer[SIZE] = {};

	/* one socket is dedicated to listening */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	/* initialize a sockaddr_in struct with our own address information for	binding the socket */
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(32000); //convert between little and big endian

	/* binding */
	bind(listenfd,  (struct sockaddr *) &servaddr, sizeof(servaddr));
	listen(listenfd, 0);
	clilen = sizeof(cliaddr);

	/* accept the client with a different socket. */
	while (1) {
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen); 				// the uninitialized cliaddr variable is filled in with the
		n = recvfrom(connfd, buffer, SIZE, 0, (struct sockaddr *) &cliaddr, &clilen);	// information of the client by recvfrom function
		buffer[n] = 0;

		printf("Received: %s\n", buffer);

		if (buffer[0] == 'r' && buffer[1] == 'e' && buffer[2] == 'q') {		
	   		char ch, file_name[] = "s.txt";
			FILE *fp = fopen(file_name,"r");
			struct stat st;

			if (fp == NULL) {
				perror("Error while opening the file.\n");
				exit(EXIT_FAILURE);
			}

			stat(file_name, &st);				//t oget the size of the file
			int i = 0, size = st.st_size;

			char send_buff[SIZE];
			sprintf(send_buff, "%d", size);		//convert int to string

			sendto(connfd, send_buff, SIZE, 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr)); 			//send size of the file

			int j = 1;
			while((ch = fgetc(fp)) != EOF) {
				send_buff[i++] = ch;

				if (i >= SIZE - 1) {
					sendto(connfd, send_buff, SIZE+1, 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr)); 	//send SIZE bytes sized data chuncks 
					i = 0; j++;
				}			
			}

			printf("i = %d\n", i);
			printf("%s\n", send_buff);

			if (size < SIZE || i > 1) {				
				sendto(connfd, send_buff, SIZE+1, 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr)); 		//send SIZE bytes sized data chuncks 
				printf("kkkkk\n");
			}
		}

		shutdown(listenfd, 1);
	}
}