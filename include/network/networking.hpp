#pragma once

#include <atomic>
#include <netinet/in.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>

/**
 * @brief Native TCP Server for FastHash.
 */
class TCPServer
{
public:
    explicit TCPServer(int port);
    ~TCPServer();

    void start();
    void stop();

private:
    int port_;                  // 2609
    int serverSocket_;          // socket file descriptor
    std::atomic<bool> running_; // thread safe flag to control server loop

    void acceptClients();                // accepts new clients in loop
    void handleClient(int clientSocket); // handles individual client I/O
};
