//
// Created by vladimir on 9.4.23..
//

#ifndef SIMPLECLIENT_CONNCONTROLER_H
#define SIMPLECLIENT_CONNCONTROLER_H

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#define BUFLEN 16384


struct ConnControler {

    ConnControler(unsigned serverPort, const std::string &serverIPAddr) : opt{1}, addrlen{sizeof(sockaddr_in)},
                                                                          serv_addr {
                                                                 AF_INET,
                                                                  htons(serverPort),0 } {

        if(inet_pton(AF_INET, serverIPAddr.c_str(), &(serv_addr.sin_addr)) <= 0) {
            throw std::runtime_error("Invalid address");
        };

        if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            throw std::runtime_error("Socket failed");
        }

        if (connect(client_fd, (const struct sockaddr*) &serv_addr, addrlen)) {
            close(client_fd);
            throw std::runtime_error("Connection failed");
        }
    }

    void sendMessage(const std::string& msg) const {
        send(client_fd, msg.c_str(), msg.length(), 0);
    }

    [[nodiscard]] std::string getMessage() const {
        char tmp[BUFLEN]{};
        read(client_fd, tmp, BUFLEN);
        return std::string {tmp};
    }

    ~ConnControler() {
        close(client_fd);
    }

private:
    int client_fd{}, status{};
    struct sockaddr_in serv_addr{};
    int opt{};
    int addrlen{};
};


#endif //SIMPLECLIENT_CONNCONTROLER_H
