#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void *handle_client(void *arg) {
  pthread_t thread_id = pthread_self();
  int newsock = *(int *) arg;
  free(arg);

  char buffer[1024] = { 0 };
  char *hello = "Server: Hello from the server\n";

  while (1) {
    memset(buffer, 0, sizeof(buffer));  // clear buffer
    ssize_t valread = read(newsock, buffer, sizeof(buffer) - 1); // subtract 1 for the null terminator
    if (valread <= 0) { // read is NULL
      printf("Client %lu DISCONNECTED!\n", thread_id);
      break;
    }
    printf("Client (Thread ID: %lu): %s", thread_id, buffer);
    //send(newsock, hello, strlen(hello), 0);
    //printf("Hello message sent\n"); 
  }

  close(newsock);
  return NULL;
}

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

  while (1) {
    //accept
    int *newsock = malloc(sizeof(int));
    if ((*newsock = accept(sockfd, (struct sockaddr *) &my_addr, &addrlen)) < 0 ) {
      perror("accept");
      printf("Error accepting client");
      exit(1);
    }

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, &handle_client, newsock) != 0) {
      printf("Error creating a thread\n");
      exit(1);
    }
    pthread_detach(thread_id); // deatach the thread
  }
  
  // close(newsock);
  close(sockfd);
  return 0;	
}
