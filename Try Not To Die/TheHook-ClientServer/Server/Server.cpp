#include "Server.h"

void Server::startServer(int serverPort, Game* game) const {
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
		serverAddress.sin_addr.s_addr = INADDR_ANY;
		serverAddress.sin_port = htons(serverPort);

		// Create server socket
		std::cout << "Creating server socket..." << std::endl;
		SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(serverSocket == INVALID_SOCKET) {
			throw SocketCreatingError(serverSocket);
		}
		std::cout << "Creating server socket done" << std::endl;
		std::cout << std::endl;

		// Bind server socket with server address
		std::cout << "Binding server socket with server address..." << std::endl;
		int bindStatus = bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
		if(bindStatus == SOCKET_ERROR) {
			throw BindingError(serverSocket);
		}
		std::cout << "Binding server socket with server address done" << std::endl;
		std::cout << std::endl;

		// Listen to clients
		std::cout << "Waiting for clients..." << std::endl;
		int listenStatus = listen(serverSocket, SOMAXCONN);
		if(listenStatus == SOCKET_ERROR) {
			throw ListeningError(serverSocket);
		}

		// Create client address and socket to hold client information from accept()
		sockaddr_in clientAddress[NUM_PLAYERS];
		int clientAddressLength[NUM_PLAYERS];
		SOCKET clientSocket[NUM_PLAYERS];

		// Accept connections from first NUM_PLAYERS clients and store client information
		int clientId = 0;
		while(clientId < NUM_PLAYERS) {
			clientAddressLength[clientId] = sizeof(clientAddress[clientId]);
			clientSocket[clientId] = accept(serverSocket, (sockaddr*)&clientAddress[clientId], &clientAddressLength[clientId]);
			if(clientSocket[clientId] == INVALID_SOCKET) {
				throw AcceptingError(serverSocket);
			}
			setsockopt(clientSocket[clientId], SOL_SOCKET, SO_RCVTIMEO, (const char*)&RECEIVE_TIMEOUT, sizeof(RECEIVE_TIMEOUT));
			std::cout << "Client #" << clientId << " connected" << std::endl;
			clientId++;
		}
		std::cout << std::endl;

		// Create a mutex to control threads
		std::mutex* mu = new std::mutex();

		// With each client, create a thread to receive messages
		ServerReceiveHandler* serverReceiveHandler[NUM_PLAYERS];
		std::thread* serverReceiveHandlerThread[NUM_PLAYERS];
		for(clientId = 0; clientId < NUM_PLAYERS; clientId++) {
			serverReceiveHandler[clientId] = new ServerReceiveHandler();
			serverReceiveHandlerThread[clientId] = new std::thread(*serverReceiveHandler[clientId], mu, clientId, clientSocket[clientId], game);
		}

		// With each client, create a thread to send messsages
		ServerSendHandler* serverSendHandler[NUM_PLAYERS];
		std::thread* serverSendHandlerThread[NUM_PLAYERS];
		for(clientId = 0; clientId < NUM_PLAYERS; clientId++) {
			serverSendHandler[clientId] = new ServerSendHandler();
			serverSendHandlerThread[clientId] = new std::thread(*serverSendHandler[clientId], mu, clientId, clientSocket[clientId], game);
		}

		// Create a thread to process server logic
		ServerLogicHandler* serverLogicHandler = new ServerLogicHandler();
		std::thread* serverLogicHandlerThread = new std::thread(*serverLogicHandler, mu, game);

		// Wait for all threads to finish
		for(clientId = 0; clientId < NUM_PLAYERS; clientId++) {
			if(serverReceiveHandlerThread[clientId]->joinable()) {
				serverReceiveHandlerThread[clientId]->join();
			}
			if(serverSendHandlerThread[clientId]->joinable()) {
				serverSendHandlerThread[clientId]->join();
			}
		}
		if(serverLogicHandlerThread->joinable()) {
			serverLogicHandlerThread->join();
		}

		// Delete mutex, handlers, and threads
		delete mu;
		for(clientId = 0; clientId < NUM_PLAYERS; clientId++) {
			delete serverReceiveHandler[clientId];
			delete serverReceiveHandlerThread[clientId];
			delete serverSendHandler[clientId];
			delete serverSendHandlerThread[clientId];
		}
		delete serverLogicHandler;
		delete serverLogicHandlerThread;

		// Close server socket and clean up winsock
		closesocket(serverSocket);
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
