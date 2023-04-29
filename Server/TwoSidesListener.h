//
// Created by vladimir on 9.4.23..
//

#ifndef SIMPLESERVER_TWOSIDESLISTENER_H
#define SIMPLESERVER_TWOSIDESLISTENER_H

#include <string>
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <pqxx/pqxx>
#include <pqxx/connection>
#include <pthread.h>
#include <set>

#define BUFLEN 16384

struct TwoSidesListener {

    TwoSidesListener() = delete;

    explicit TwoSidesListener(unsigned port, const std::string &dataBaseIP,
                                                unsigned databasePort);
    int acceptClient(); // return descriptor of connection

    void startListening();

    std::string getClientMessage(int client_socket);

    void sendClientMessage(const std::string &hello, int client_socket) const noexcept;

    void closeConnectionfd(int connfd) {
        client_sockets.erase(connfd);
        close(connfd);
    }

    pqxx::connection* getDatabaseConnection() {return pgsql;}

    ~TwoSidesListener();

private:
    // client connection
    int server_fd{};
    std::set<int> client_sockets;
    struct sockaddr_in address{};
    int opt{};
    int addrlen{};
    // database connection
    pqxx::connection *pgsql;
};


#endif //SIMPLESERVER_TWOSIDESLISTENER_H
