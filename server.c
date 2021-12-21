#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <ctype.h>

// Louay Ahmad (CSIT 231_01) and Dori Lerner (CSIT 231_03)

#define MAX 80 //max number of characters in a string for the buffer
#define SA struct sockaddr //stores socket address information
int sockfd, connfd, len; //for connection and sockets

// Function designed for chat between client and server.
void func(int sockfd) {
  //declares the buffer used to store the messages between the client and the server
  char buff[MAX];
  int n;
  int messageSentCount = 0;
  int messageReceivedCount = 0;
  for (;;) { //allows for the conversation to keep going....does not stop after one message
    messageSentCount++;
    messageReceivedCount++;
    bzero(buff, MAX);
    // read the message from client and copy it in buffer
    read(sockfd, buff, sizeof(buff));
    // if msg contains "Exit" then the client has exited and the server must wait for a new connection
    if ((strncmp("exit", buff, 4) == 0) || (strncmp("Exit", buff, 4) == 0) || (strncmp("eXit", buff, 4) == 0) || (strncmp("EXit", buff, 4) == 0) || (strncmp("exIt", buff, 4) == 0) || (strncmp("ExIt", buff, 4) == 0) || (strncmp("eXIt", buff, 4) == 0) || (strncmp("EXIt", buff, 4) == 0) || (strncmp("exiT", buff, 4) == 0) || (strncmp("ExiT", buff, 4) == 0) || (strncmp("eXiT", buff, 4) == 0) || (strncmp("EXiT", buff, 4) == 0) || (strncmp("exIT", buff, 4) == 0) || (strncmp("ExIT", buff, 4) == 0) || (strncmp("EXIT", buff, 4) == 0) || (strncmp("eXIT", buff, 4) == 0)) {
      printf("Connection is closed...waiting for new connection...\n");
      break;
    }
    // print buffer which contains the client contents
    printf("From client message #%d: %sEnter message %d to client : ", messageReceivedCount, buff, messageSentCount);
    bzero(buff, MAX);
    n = 0;
    // copy server message in the buffer
    while ((buff[n++] = getchar()) != '\n')
        ;
    // and send that buffer to client
    write(sockfd, buff, sizeof(buff));
  }
}

// Driver function
int main(int argc, char *argv[]) {
  struct sockaddr_in servaddr, cli;

  if (argc < 2) {
    printf("Too few arguments, please input a port number...");
    exit(1);
  }
  // clears the screen
  system("clear");

  //allowing port number to be entered on command line, default port 8080 deleted
  int portNumber = atoi(argv[1]); 
  int PORT;
  char *command;
  //checking if port is not a well known or ephemeral port number 
  if ((portNumber >= 1024 && portNumber <= 49151)) {
    PORT = portNumber;
  }
  else {
    printf("Cannot set port number. Please enter anything between 1024 to 49151.... 1-1023 are 'well-known' ports and 49152-65535 are 'ephemeral' ports.\n");
    sleep(1);
    exit(1);
  } 

  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("Failed to create socket...\n");
    exit(0);
  }
  else {
    printf("Successfully created socked..\n");
  }
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  // Binding newly created socket to given IP and verification
  if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
    printf("Socket bind failed...\n");
    exit(0);
  }
  else {
    printf("Socket successfully binded...\n");
  }

  // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening...\n");
    len = sizeof(cli);
   
    // Accept the data packet from client and verification
    while(1) { 
      connfd = accept(sockfd, (SA*)&cli, &len);\
      if (connfd < 0) {
        printf("Connection to server failed...\n");
        exit(0);
      }
      else {
        system("clear");
        printf("New client connected...\n");
        sprintf(command,"netstat -na | grep ':%d'", PORT);
        system(command);
      }
      // Function for chatting between client and server
      func(connfd);
  }
  close(connfd);
}