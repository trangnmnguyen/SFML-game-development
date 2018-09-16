#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <winsock.h>
#include <exception>

// Errors when passing arguments into main function
class MainArgumentError : public std::exception {
public:
	virtual const char* what() const throw() {
		return "Error: Wrong arguments";
	}
};

// Errors when handling servers
// Store a server socket that needs to be closed when handling errors
class ServerError : public std::exception {
private:
	SOCKET serverSocket;

public:
	ServerError(SOCKET serverSocket) {
		this->serverSocket = serverSocket;
	}

	SOCKET getServerSocket() const {
		return serverSocket;
	}
};

// Errors when initializing winsock
class WinsockInitializingError : public ServerError {
public:
	WinsockInitializingError(SOCKET serverSocket) : ServerError(serverSocket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed initializing winsock";
	}
};

// Errors when creating server socket
class ServerSocketCreatingError : public ServerError {
public:
	ServerSocketCreatingError(SOCKET serverSocket) : ServerError(serverSocket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed creating server socket";
	}
};

// Errors when binding server socket with server address
class BindingError : public ServerError {
public:
	BindingError(SOCKET serverSocket) : ServerError(serverSocket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed binding server socket with server address";
	}
};

// Errors when listening to clients
class ListeningError : public ServerError {
public:
	ListeningError(SOCKET serverSocket) : ServerError(serverSocket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed listening to clients";
	}
};

// Errors when accepting client connection
class ClientSocketConnectingError : public ServerError {
public:
	ClientSocketConnectingError(SOCKET serverSocket) : ServerError(serverSocket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed accepting client connection";
	}
};

// Errors when sending messages to client
class SendingError : public ServerError {
public:
	SendingError(SOCKET serverSocket) : ServerError(serverSocket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed sending messages to client";
	}
};

// Errors when receiving messages from client
class ReceivingError : public ServerError {
public:
	ReceivingError(SOCKET serverSocket) : ServerError(serverSocket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed sending messages to client";
	}
};

#endif