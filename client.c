#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <ctype.h>

//Louay Ahmad (CSIT 231_01) and Dori Lerner (CSIT 231_03)

#define MAX 80 //max number of characters in a string for the buffer
#define SA struct sockaddr //stores socket address information
int sockfd, connfd; //for connection and sockets

void func(int sockfd)
{
  //declares the buffer used to store the messages between the client and the server
  char buff[MAX]; 
  int n;
  //stores the message counts for the sending and receviving end of messages between the client and the server
  int messageSentCount = 0;
  int messageReceivedCount = 0;
  //add an infinite for loop that allows for the conversation to keep going....does not stop after one message until the client exits or types "exit"
  for (;;) { 
    //increments the messages after each message is sent and recived from and to the server
    messageSentCount++;
    messageReceivedCount++;
    bzero(buff, MAX);
    printf("Enter Message %d", messageSentCount);
    printf(" ");
    // prints buffer which contains the client contents
    n = 0;
    // copies server message in the buffer
    while ((buff[n++] = getchar()) != '\n')
        ;
    // sends the message in the buffer to the client
    write(sockfd, buff, sizeof(buff));
    // if msg contains "Exit" then client exits...case insensitive (created a program that listed all possible permutations)
    if ((strncmp("exit", buff, 4) == 0) || (strncmp("Exit", buff, 4) == 0) || (strncmp("eXit", buff, 4) == 0) || (strncmp("EXit", buff, 4) == 0) || (strncmp("exIt", buff, 4) == 0) || (strncmp("ExIt", buff, 4) == 0) || (strncmp("eXIt", buff, 4) == 0) || (strncmp("EXIt", buff, 4) == 0) || (strncmp("exiT", buff, 4) == 0) || (strncmp("ExiT", buff, 4) == 0) || (strncmp("eXiT", buff, 4) == 0) || (strncmp("EXiT", buff, 4) == 0) || (strncmp("exIT", buff, 4) == 0) || (strncmp("ExIT", buff, 4) == 0) || (strncmp("EXIT", buff, 4) == 0) || (strncmp("eXIT", buff, 4) == 0)) {
      printf("Connection is closed...\n");
      exit(0);
    }
    bzero(buff, sizeof(buff));
    // reads the message from the client and copies it in buffer
    read(sockfd, buff, sizeof(buff)); 
    printf("From Server message #%d : %s", messageReceivedCount, buff);
  }
}
  
int main(int argc, char *argv[])
{
  struct sockaddr_in servaddr, cli;
  struct hostent *server;

  // clears the screen before any conversation is held
  system("clear");

  // checks if the arguments on the command line are less than 2, which means that the user did not enter a port number...exits program if it is not entered
  if (argc < 2) {
       printf("Please enter host port number...");
       exit(1);
  }

  //allowing port number to be entered on command line, default port 8080 deleted
  //converts the command line argument port number to an int to be used as input
  int portNumber = atoi(argv[1]); 
  //stores the port number inputted by the user 
  int PORT; 
  //stores the netstat command that proves the client and sever are on the inputted port number
  char *command;
  //checking if port is not a well known or ephemeral port number 
  if ((portNumber >= 1024 && portNumber <= 49151)) {
    PORT = portNumber;
  }
  else {
    //lets the user know why the port was not accepted and lets them know which ones are acceptable
    printf("Invalid Port Number. Please enter anything between 1024 to 49151.... 1-1023 are 'well-known' ports and 49152-65535 are 'ephemeral' ports.\n");
    sleep(1);
    exit(1);
  } 

  // create the socket and verifify if it was successfully created
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("socket creation failed...\n");
    exit(0);
  }
  else {
  printf("Socket successfully created..\n");
  }

  bzero(&servaddr, sizeof(servaddr));

  //assigns the IP and the PORT number (specified by the user) (port should be same as the server)
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(PORT);
  //  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  // connect the client socket to server socket and check for any errors
  if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
    printf("connection with the server failed...\n");
    exit(0);
  }
  else {
    printf("connected to the server..\n");
  }

  // function to initialize the back and forth messaging with the server and client
  func(sockfd);
  close(connfd); //closes connection after the messaging is terminated
  close(sockfd); //closes the socket after the messaging is terminated
}                            