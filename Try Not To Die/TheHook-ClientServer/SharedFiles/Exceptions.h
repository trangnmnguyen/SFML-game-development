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

// Errors when handling server and client sockets
// Store a socket that needs to be closed when handling errors
class SocketError : public std::exception {
private:
	SOCKET socket;

public:
	SocketError(SOCKET socket) {
		this->socket = socket;
	}

	SOCKET getSocket() const {
		return socket;
	}
};

// Errors when initializing winsock
class WinsockInitializingError : public SocketError {
public:
	WinsockInitializingError(SOCKET socket) : SocketError(socket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed initializing winsock";
	}
};

// Errors when creating socket
class SocketCreatingError : public SocketError {
public:
	SocketCreatingError(SOCKET socket) : SocketError(socket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed creating socket";
	}
};

// Errors when binding socket with address
class BindingError : public SocketError {
public:
	BindingError(SOCKET socket) : SocketError(socket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed binding socket with address";
	}
};

// Errors when listening to clients
class ListeningError : public SocketError {
public:
	ListeningError(SOCKET socket) : SocketError(socket) {
	}

	virtual const char* what() const throw() {
		return "Error: Failed listening to clients";
	}
};

// Errors when accepting client connection
class AcceptingError : public SocketError {
public:
	AcceptingError(SOCKET socket) : SocketError(socket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed accepting client connection";
	}
};

// Error when connecting to server
class ConnectingError : public SocketError {
public:
	ConnectingError(SOCKET socket) : SocketError(socket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed connecting to server";
	}
};

// Errors when sending messages
class SendingError : public SocketError {
public:
	SendingError(SOCKET socket) : SocketError(socket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed sending messages";
	}
};

// Errors when receiving messages
class ReceivingError : public SocketError {
public:
	ReceivingError(SOCKET socket) : SocketError(socket) {		
	}

	virtual const char* what() const throw() {
		return "Error: Failed receiving messages";
	}
};

#endif
