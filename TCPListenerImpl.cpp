#include "TCPListener.h"
#include <iostream>
#include <string>
#include <sstream>

int TCPListener::init() {
    // Initialze winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsRes = WSAStartup(ver, &wsData);
    if (wsRes != 0) {
        return wsRes;
    }

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == INVALID_SOCKET) {
        return WSAGetLastError();
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(m_port);
    inet_pton(AF_INET, m_ipAddress, &hint.sin_addr);

    if (bind(m_socket, (sockaddr*) &hint, sizeof(hint)) == SOCKET_ERROR) {
        return WSAGetLastError();
    }

    if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR) {
        return WSAGetLastError();
    }

    FD_ZERO(&m_master_fds);
    FD_SET(m_socket, &m_master_fds);
    return 0;
}

int TCPListener::run() {
	bool running = true;
	while (running) {
		fd_set copy = m_master_fds;  // make a copy of m_master_fds to pass to select

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		for (int i = 0; i < socketCount; i++) {
			SOCKET sock = copy.fd_array[i];

			// inbound communication
			if (sock == m_socket) {
				SOCKET client = accept(m_socket, nullptr, nullptr);

				// Add the new connection to the list of connected clients
				FD_SET(client, &m_master_fds);
				handleClientConnected(client);
			} else {  // inbound msg
				char buf[4096];
				ZeroMemory(buf, 4096);

				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0) {  // client dropped
					handleClientDisconnected(sock);
					closesocket(sock);
					FD_CLR(sock, &m_master_fds);
				} else {
					handleMessageReceived(sock, buf, bytesIn);
				}
			}
		}
	}

	// TODO add logic for setting running to false on quit
	// Remove the listening socket from the master file descriptor set and close it
	// to prevent anyone else trying to connect.
	FD_CLR(m_socket, &m_master_fds);
	closesocket(m_socket);

	while (m_master_fds.fd_count > 0) {
		// Get the socket number
		SOCKET sock = m_master_fds.fd_array[0];
		//
		// // Send the goodbye message
		// send(sock, msg.c_str(), msg.size() + 1, 0);

		// Remove it from the master file list and close the socket
		FD_CLR(sock, &m_master_fds);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();
	return 0;
}

void TCPListener::handleClientConnected(int clientSocket) {

}

void TCPListener::handleClientDisconnected(int clientSocket) {

}

void TCPListener::handleMessageReceived(int clientSocket, const char *msg, int msgSize) {

}

void TCPListener::sendToClient(int clientSocket, const char * msg, int msgSize) {
	send(clientSocket, msg, msgSize, 0);
}

void TCPListener::broadcastToClients(int sendingClientSocket, const char * msg, int msgSize) {
	for (int i = 0; i < m_master_fds.fd_count; i++) {
		SOCKET outSock = m_master_fds.fd_array[i];
		if (outSock != m_socket && outSock != sendingClientSocket) {
			sendToClient(outSock, msg, msgSize);
		}
	}
}
