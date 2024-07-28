#pragma once
#include "TCPListener.h"

class Webserver : public TCPListener {
public:
    Webserver(const char * ipAddress, int port):
        TCPListener(ipAddress, port) { }

protected:
    // handler for client connections
    virtual void handleClientConnected(int clientSocket);
    // handler for client disconnections
    virtual void handleClientDisconnected(int clientSocket);
    // handler for when a msg is received from the client
    virtual void handleMessageReceived(int clientSocket, const char * msg, int msgSize);
};
