#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

const int PORT = 8080;
const std::string DIRECTORY = "./www"; // Directory for static HTML files

void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    read(clientSocket, buffer, 1024);
    std::string request(buffer);

    // Extract the requested file path
    std::string filePath = DIRECTORY + request.substr(4, request.find(' ', 4) - 4);
    if (filePath == DIRECTORY + "/") {
        filePath = DIRECTORY + "/index.html"; // Default file
    }

    std::ifstream file(filePath);
    if (file) {
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        response += std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        send(clientSocket, response.c_str(), response.size(), 0);
    } else {
        std::string response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n404 Not Found";
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    close(clientSocket);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket to the network address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) < 0) {
        perror("Listen failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on port " << PORT << std::endl;

    while (true) {
        // Accept a new connection
        if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen)) < 0) {
            perror("Accept failed");
            continue;
        }

        // Handle the connection in a new thread
        std::thread(handleClient, clientSocket).detach();
    }

    close(serverSocket);
    return 0;
}

