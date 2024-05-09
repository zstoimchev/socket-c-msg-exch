#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

/*
void* handle_incomming_msg(void *arg) {
  int newsock = *(int *) arg;
  free(arg);

  char buffer[1024] = { 0 };
  while (1) {
    memset(buffer, 0, sizeof(buffer));
    ssize_t valread = read(newsock, buffer, sizeof(buffer) - 1);
    if (valread <= 0) {
      printf("read empty\n");
    }
    printf("Client some: %s\n", buffer);
  }
}
*/

void* handle_incoming_msg(void *arg) {
    int newsock = *(int *)arg;
    // free(arg);

    char buffer[1024]; // No need to initialize with zeros
    ssize_t valread;

    while ((valread = read(newsock, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[valread] = '\0'; // Null-terminate the received message
        printf("Client message: %s\n", buffer);
    }

    if (valread == 0) {
        printf("Client disconnected\n");
    } else {
        perror("read");
        printf("Error reading from client\n");
    }

    close(newsock); // Close the socket
    return NULL;
}

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

  char* hello = "hello from the other side\n";


  pthread_t thread_incoming;
  if (pthread_create(&thread_incoming, NULL, &handle_incoming_msg, (void *) &clientfd) != 0) {
    printf("Error threading incomming messages\n");
    exit(1);
  }


  char buffer[1024] = { 0 };
  while (1) {
    printf("Me: ");
    fgets(buffer, sizeof(buffer) - 1, stdin);
    send(clientfd, buffer, strlen(buffer), 0);
  }
  //send(clientfd, hello, strlen(hello), 0);
  //printf("msg sent");

  if (pthread_join(thread_incoming, NULL) != 0) {
    printf("Error joining the incoming thread\n");
  }
  
  close(clientfd);
  return 0;
}
