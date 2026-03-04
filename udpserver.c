/*
 * udpserver.c - A simple UDP echo server
 * usage: udpserver
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define BUFSIZE 512

/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {
  int sockfd;                    /* socket */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  char buf[BUFSIZE];             /* message buf */
  int n;                         /* message byte size */
  int portno = 7777;             /* port to listen on */

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
  if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    error("ERROR on binding");

  /*
   * main loop: wait for a datagram, then echo it
   */
  socklen_t clientlen = sizeof(clientaddr);
  while (1) {

    /*
     * recvfrom: receive a UDP datagram from a client
     */
    n = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&clientaddr,
                 &clientlen);
    if (n < 0)
      error("ERROR in recvfrom");

    printf("server received datagram from %s\n",
           inet_ntoa(clientaddr.sin_addr));
    printf("source port: %d\n", ntohs(clientaddr.sin_port));
    printf("server received %d bytes\n", n);

    unsigned char *byte_pointer = (unsigned char *)buf;

    for (int i = 0; i < 12; i++) {
      printf("%02x ", byte_pointer[i]);
    }
    printf("\n");

    // print the full n bytes in hex
    printf("Full message in hex:\n");
    for (int i = 0; i < n; i++) {
      printf("%02x ", byte_pointer[i]);
    }
    printf("\n");

    uint16_t header_fields[6] = {};

    // parse header
    printf("Concatenated header (first 12 bytes):\n");
    for (int i = 0; i < 12; i += 2) {
      uint8_t high_byte = byte_pointer[i];
      uint8_t low_byte = byte_pointer[i + 1];
      header_fields[i / 2] = (high_byte << 8) | low_byte;
    }
    printf("\n");

    printf("Parsed header fields:\n");
    uint16_t ID = header_fields[0];
    uint16_t FLAGS = header_fields[1];
    uint16_t QDCOUNT = header_fields[2];
    uint16_t ANCOUNT = header_fields[3];
    uint16_t NSCOUNT = header_fields[4];
    uint16_t RCOUNT = header_fields[5];
    printf("ID: %u\n", ID);
    printf("FLAGS: %u\n", FLAGS);
    printf("QDCOUNT: %u\n", QDCOUNT);
    printf("ANCOUNT: %u\n", ANCOUNT);
    printf("NSCOUNT: %u\n", NSCOUNT);
    printf("RCOUNT: %u\n", RCOUNT);
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
