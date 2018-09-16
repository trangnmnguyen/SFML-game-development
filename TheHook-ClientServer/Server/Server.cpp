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
			throw ServerSocketCreatingError(serverSocket);
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
				throw ClientSocketConnectingError(serverSocket);
			}
			std::cout << "Client #" << clientId << " connected" << std::endl;
			clientId++;
		}
		std::cout << std::endl;

		// Create a mutex to control threads
		std::mutex* mu = new std::mutex();

		// With each client, create a thread to receive messages
		ClientReceiveHandler* clientReceiveHandler[NUM_PLAYERS];
		std::thread* clientReceiveHandlerThread[NUM_PLAYERS];
		for(clientId = 0; clientId < NUM_PLAYERS; clientId++) {
			clientReceiveHandler[clientId] = new ClientReceiveHandler();
			clientReceiveHandlerThread[clientId] = new std::thread(*clientReceiveHandler[clientId], mu, clientId, clientAddress[clientId], clientAddressLength[clientId], clientSocket[clientId], game);
		}

		// With each client, create a thread to send messsages
		ClientSendHandler* clientSendHandler[NUM_PLAYERS];
		std::thread* clientSendHandlerThread[NUM_PLAYERS];
		for(clientId = 0; clientId < NUM_PLAYERS; clientId++) {
			clientSendHandler[clientId] = new ClientSendHandler();
			clientSendHandlerThread[clientId] = new std::thread(*clientSendHandler[clientId], mu, clientId, clientAddress[clientId], clientAddressLength[clientId], clientSocket[clientId], game);
		}

		// Create a thread to process game logic
		GameLogicHandler* gameLogicHandler = new GameLogicHandler();
		std::thread* gameLogicHandlerThread = new std::thread(*gameLogicHandler, mu, game);

		// Wait for all threads to finish
		for(clientId = 0; clientId < NUM_PLAYERS; clientId++) {
			if(clientReceiveHandlerThread[clientId]->joinable()) {
				clientReceiveHandlerThread[clientId]->join();
			}
			if(clientSendHandlerThread[clientId]->joinable()) {
				clientSendHandlerThread[clientId]->join();
			}
		}
		if(gameLogicHandlerThread->joinable()) {
			gameLogicHandlerThread->join();
		}

		// Delete mutex, handlers, and threads
		delete mu;
		for(clientId = 0; clientId < NUM_PLAYERS; clientId++) {
			delete clientReceiveHandler[clientId];
			delete clientReceiveHandlerThread[clientId];
			delete clientSendHandler[clientId];
			delete clientSendHandlerThread[clientId];
		}
		delete gameLogicHandler;
		delete gameLogicHandlerThread;

		// Close server socket and clean up winsock
		closesocket(serverSocket);
		WSACleanup();
	}

	catch(ServerError& e) {
		std::cerr << e.what() << std::endl;
		if(e.getServerSocket() != INVALID_SOCKET) {
			closesocket(e.getServerSocket());
		}
	}

	catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}