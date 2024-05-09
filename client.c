//#include <stdlib.h>
//#include <arpa/inet.h>
//#include <string.h>
//#include <sys/socket.h>
//#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
//#include <pthread.h>


int main() {
  struct sockaddr_in server_addr;
  int clientfd;
  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    printf("Error creating client socket\n");
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(12137);

  if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) < 1) {
    printf("Error connecting to address");
    exit(1);
  }

  if (connect(clientfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
    printf("Error connecting\n");
    exit(1);
  }

  char* hello = "hello from the other size";

  char buffer[1024] = { 0 };
  send(clientfd, hello, strlen(hello), 0);
  printf("msg sent");
  int valread = read(clientfd, buffer, sizeof(buffer) - 1);
  printf("%s\n", buffer);

  return 0;


}
