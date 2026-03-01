/* 
 * udpserver.c - A simple UDP echo server 
 * usage: udpserver
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 512

/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {
  int sockfd; /* socket */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  char buf[BUFSIZE]; /* message buf */
  int n; /* message byte size */
  int portno = 7777; /* port to listen on */

  /* 
   * socket: create the parent socket 
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");

  /*
   * build the server's Internet address
   */
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(sockfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

  /* 
   * main loop: wait for a datagram, then echo it
   */
  socklen_t clientlen = sizeof(clientaddr);
  while (1) {

    /*
     * recvfrom: receive a UDP datagram from a client
     */
    n = recvfrom(sockfd, buf, BUFSIZE, 0,
		 (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0)
      error("ERROR in recvfrom");

    printf("server received datagram from %s\n",
       inet_ntoa(clientaddr.sin_addr));
    printf("source port: %d\n", ntohs(clientaddr.sin_port));
    printf("server received %d bytes\n", n);

    unsigned char* byte_pointer = (unsigned char*)buf;

    for (int i = 0; i < 11; i++) {
        printf("%02x ", byte_pointer[i]);
    }
    printf("\n");

    /* 
     * sendto: echo the input back to the client 
     */
    /* n = sendto(sockfd, buf, n, 0,  */
	   /*     (struct sockaddr *) &clientaddr, clientlen); */
    /* if (n < 0)  */
    /*   error("ERROR in sendto"); */
  }
}

