#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define RSA_KEY_LENGTH 2048
#define PADDING RSA_PKCS1_PADDING

RSA *create_RSA (RSA *keypair, int pem_type, char *filename) {
    //RSA *rsa= NULL;
    FILE *fp = NULL;

    if (pem_type == 1) {
        fp = fopen(filename, "w");
        PEM_write_RSAPublicKey(fp, keypair);
    } else if (pem_type == 2) {
        fp = fopen(filename, "w");
        PEM_write_RSAPrivateKey(fp, keypair, NULL, NULL, 0, NULL, NULL);
    }

    //RSA_free(keypair);
    if (fp == NULL) {
        printf("Unable to open file %s \n", filename);
        exit(1);
    }
    //RSA_free(rsa);
    fclose(fp);
    return keypair;
}

void* handle_incoming_msg(void *arg) {
    int newsock = *(int *)arg;
    // free(arg);

    char buffer[1024]; // No need to initialize with zeros
    ssize_t valread;

    while ((valread = read(newsock, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[valread] = '\0'; // Null-terminate the received message
        printf("%s", buffer);
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

  //==========================================================================================================
    char plainText[2048/8] = "This is a text to be encrypted by OpenSSL"; //key length : 2048

    char * publicKeyFile = "public.pem";
    char * privateKeyFile = "private.pem";

    printf("plain text: %s\n", plainText);

    // Generate key pair
    RSA *keypair = RSA_generate_key(RSA_KEY_LENGTH, 3, NULL, NULL);

    // Write the keypair to disk
    create_RSA(keypair, 2, privateKeyFile);
    create_RSA(keypair, 1, publicKeyFile);

    // Load the public key
    FILE* pubKeyFile = fopen(publicKeyFile, "rb");
    RSA* pubKey = PEM_read_RSAPublicKey(pubKeyFile, NULL, NULL, NULL);
    fclose(pubKeyFile);

    // Encrypt the plaintext
    char encrypted[4098] = {};
    int encrypted_length = RSA_public_encrypt(strlen(plainText)+1, (unsigned char*)plainText, (unsigned char*)encrypted, pubKey, PADDING);
    if (encrypted_length == -1) {
        printf("Public Encrypt failed \n");
        exit(0);
    }

    RSA_free(pubKey);

    // Load the private key
    FILE* privKeyFile = fopen(privateKeyFile, "rb");
    RSA* privKey = PEM_read_RSAPrivateKey(privKeyFile, NULL, NULL, NULL);
    fclose(privKeyFile);

    // Decrypt the ciphertext
    char decrypted[4098] = {};
    int decrypted_length = RSA_private_decrypt(encrypted_length, (unsigned char*)encrypted, (unsigned char*)decrypted, privKey, PADDING);
    if (decrypted_length == -1) {
        printf("Private Decrypt failed \n");
        exit(0);
    }

    RSA_free(privKey);

    printf("decrypted text: %s\n", decrypted);
  //==========================================================================================================
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


  pthread_t thread_incoming;
  if (pthread_create(&thread_incoming, NULL, &handle_incoming_msg, (void *) &clientfd) != 0) {
    printf("Error threading incomming messages\n");
    exit(1);
  }


  char buffer[1024] = { 0 };
  while (1) {
//    printf("Me: ");
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
