# Socket Message Exchange

This repository contains a simple client-server application for message exchange using sockets in C. The application also demonstrates the use of OpenSSL for RSA encryption and decryption.

## Features

- **Socket Communication**: The application uses TCP/IP sockets for communication between the client and the server.
- **Multithreading**: The client handles incoming messages in a separate thread.
- **RSA Encryption**: The application uses RSA encryption to encrypt the messages before sending and decrypts them after receiving.

## Prerequisites

- GCC Compiler
- OpenSSL Library

## Compilation

Use the following commands to compile the code:

```bash
gcc -o server server.c -lssl -lcrypto -lpthread
gcc -o client client.c -lssl -lcrypto -lpthread

Usage
After compiling the code, you can run the server using the following command:

./server

Then, in a new terminal window, you can run the client using the following command:

./client

Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

License
MIT
