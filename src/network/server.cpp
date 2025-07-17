#include "network/networking.hpp"

TCPServer::TCPServer(int port)
    : port_(port), serverSocket_(-1), running_(false) {}

TCPServer::~TCPServer() { stop(); }

void TCPServer::start()
{
    /*
    Create a new socket of type TYPE in domain DOMAIN, using
  protocol PROTOCOL. If PROTOCOL is zero, one is chosen automatically.
  Returns a file descriptor for the new socket, or -1 for errors.

  socket(2)                       System Calls Manual socket(2)

  NAME
       socket - create an endpoint for communication

  LIBRARY
       Standard C library (libc, -lc)

  SYNOPSIS
       #include <sys/socket.h>
       int socket(int domain, int type, int protocol);

  DESCRIPTION
       socket()  creates an endpoint for communication and returns a file descrip‐
       tor that refers to that endpoint.  The file descriptor returned by  a
  successful call will be the lowest-numbered file descriptor not currently open
  for the process.
  */
    //    int socket(int domain, int type, int protocol);

    //       AF_INET      IPv4 Internet protocols                    ip(7)

    this->serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket_ == -1)
    {
        std::perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET; // ipv4

    //        The  htons()  function  converts  the unsigned short integer
    //        hostshort from host byte order to network byte order.
    serverAddr.sin_port = htons(port_);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;

    /**
            int setsockopt(int sockfd, int level, int optname,
                      const void optval[.optlen],
                      socklen_t optlen);
     */
    setsockopt(this->serverSocket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(
            this->serverSocket_,
            (sockaddr *)&serverAddr, sizeof(serverAddr) < 0))
    {
        std::perror("Bind failed");
        close(serverSocket_);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket_, SOMAXCONN) < 0)
    {
        perror("Listen failed");
        close(serverSocket_);
        exit(EXIT_FAILURE);
    }

    std::cout << "[FastHash] TCP server started on port " << this->port_ << std::endl;

    this->running_ = true;
    std::thread(&TCPServer::acceptClients, this).detach();
}

void TCPServer::stop()
{
    if (this->running_)
    {
        this->running_ = false;
        close(serverSocket_);
        std::cout << "[FastHash] Server stopped." << std::endl;
    }
}

void TCPServer::acceptClients()
{
    while (this->running_)
    {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket_, (sockaddr *)&clientAddr, &clientLen);

        if (clientSocket < 0)
        {
            if (this->running_)
                std::perror("Accept failed");
            continue;
        }

        std::thread(&TCPServer::handleClient, this, clientSocket).detach();
    }
}
