#include "Client.h"

void Client::startClient(unsigned long serverIp, int serverPort, Game* game) const {
	try {

		// Initialize winsock
		std::cout << "Initizlizing winsock..." << std::endl;
		WSADATA wsa;
		if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			throw WinsockInitializingError(INVALID_SOCKET);
		}
		std::cout << "Initizlizing winsock done" << std::endl;
		std::cout << std::endl;

		// Create server address
		sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = serverIp;
		serverAddress.sin_port = htons(serverPort);

		// Create client socket
		std::cout << "Creating client socket..." << std::endl;
		SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(clientSocket == INVALID_SOCKET) {
			throw SocketCreatingError(clientSocket);
		}
		setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&RECEIVE_TIMEOUT, sizeof(RECEIVE_TIMEOUT));
		std::cout << "Creating client socket done" << std::endl;
		std::cout << std::endl;

		// Connect to server
		std::cout << "Connecting to server..." << std::endl;
		int connectStatus = connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
		if(connectStatus == SOCKET_ERROR) {
			throw ConnectingError(clientSocket);
		}
		std::cout << "Connecting to server done" << std::endl;
		std::cout << std::endl;

		// Create a mutex to control threads
		std::mutex* mu = new std::mutex();

		// Create a thread to receive messages from server
		ClientReceiveHandler* clientReceiveHandler = new ClientReceiveHandler();
		std::thread* clientReceiveHandlerThread = new std::thread(*clientReceiveHandler, mu, clientSocket, game);

		// Create a thread to send messages to server
		ClientSendHandler* clientSendHandler = new ClientSendHandler();
		std::thread* clientSendHandlerThread = new std::thread(*clientSendHandler, mu, clientSocket, game);

		// Wait for all threads to finish
		if(clientReceiveHandlerThread->joinable()) {
			clientReceiveHandlerThread->join();
		}
		if(clientSendHandlerThread->joinable()) {
			clientSendHandlerThread->join();
		}

		// Delete mutex, handlers, and threads
		delete mu;
		delete clientReceiveHandler;
		delete clientReceiveHandlerThread;
		delete clientSendHandler;
		delete clientSendHandlerThread;

		// Close client socket and clean up winsock
		closesocket(clientSocket);
		WSACleanup();
	}

	catch(SocketError& e) {
		std::cerr << e.what() << std::endl;
		if(e.getSocket() != INVALID_SOCKET) {
			closesocket(e.getSocket());
		}
		WSACleanup();
	}

	catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
		WSACleanup();
	}
}
