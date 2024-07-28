#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <vector>
#include "Webserver.h"

void Webserver::handleClientConnected(int clientSocket) {

}

void Webserver::handleClientDisconnected(int clientSocket) {

}

void Webserver::handleMessageReceived(int clientSocket, const char *msg, int msgSize) {
    // Parse out the client's request string e.g. GET /index.html HTTP/1.1
    std::istringstream iss(msg);
    std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)),
        std::istream_iterator<std::string>());

    std::string content = "<h1>404 Not Found</h1>";
    std::string htmlFile = "/index.html";
    int statusCode = 404;

    if (parsed.size() > 2 && parsed[0] == "GET") {
        // Open document in local filesystem
        htmlFile = parsed[1];
        if (htmlFile == "/") {
            htmlFile = "/index.html";
        }
    }

    std::ifstream f("C:\\Users\\David\\CLionProjects\\cpp-webserver\\wwwroot" + htmlFile);
    if (f.good())
    {
        std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        content = str;
        statusCode = 200;
    }
    f.close();

    std::ostringstream oss;
    oss << "HTTP/1.1 " << statusCode << " OK\r\n";
    oss << "Cache-Control: no-cache, private\r\n";
    oss << "Content-Type: text/html\r\n";
    oss << "Content-Length: " << content.size() << "\r\n";
    oss << "\r\n";
    oss << content;

    std::string output = oss.str();
    sendToClient(clientSocket, output.c_str(), output.size() + 1);
}
