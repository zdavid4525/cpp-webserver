#pragma once

#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

class TCPListener {

public:
    TCPListener(const char * ipAddress, int port):
        m_ipAddress(ipAddress), m_port(port) { }
    int init();  // initialize the listener
    int run();  // run the listener

protected:
    // handler for client connections
    virtual void handleClientConnected(int clientSocket);
    // handler for client disconnections
    virtual void handleClientDisconnected(int clientSocket);
    // handler for when a msg is received from the client
    virtual void handleMessageReceived(int clientSocket, const char * msg, int msgSize);

    // send msg to specific client
    void sendToClient(int clientSocket, const char * msg, int msgSize);
    // broadcast msg from a client to all other clients
    void broadcastToClients(int sendingClientSocket, const char * msg, int msgSize);

private:
    const char *    m_ipAddress;  // IP address the server will run on
    int             m_port;  // port for the web service
    int             m_socket;  // internal file descriptor for the listening socket
    fd_set          m_master_fds;  // master file descriptor set
};