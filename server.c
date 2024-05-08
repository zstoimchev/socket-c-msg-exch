#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
  struct sockaddr_in my_addr;
  socklen_t addrlen = sizeof(my_addr);
  int sockfd, newsock;
  size_t valread;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Error creating the socket../n");
    exit(1);
  }

  bzero (&my_addr, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(12137);  
  if (inet_pton(AF_INET, "127.0.0.1", &(my_addr.sin_addr)) < 1) { 
    printf("Error binding to localhost..\n");
    exit(1);
  }
  if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0 ) {
    printf("Error in binding..\n");
    perror("bind");
    exit(1);
  }

  //listen
  if ((listen(sockfd, 3)) < 0) {
    perror("listen");
    printf("Error in listening");
  }

  //accept
  // int accept (int sockfd, struct sockaddr *fromaddr, socklen_t *addrlen);
  if ((newsock = accept(sockfd, (struct sockaddr *) &my_addr, &addrlen)) < 0 ) {
    perror("accept");
    printf("Error accepting client");
    exit(1);
  }

  char buffer[1024] = { 0 };
  char*hello = "Server: Hello from the server\n";

  while (1) {
    valread = read(newsock, buffer, 1024 - 1); // subtract 1 for the null terminator at the end
    printf("Client: %s", buffer);
    send(newsock, hello, strlen(hello), 0);
    //printf("Hello message sent\n"); 
  }
  
  close(newsock);
  close(sockfd);
  return 0;	
}
