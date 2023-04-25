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

#define BUFLEN 16384

struct TwoSidesListener {

    explicit TwoSidesListener(unsigned port, const std::string &dataBaseIP,
                                                unsigned databasePort);
    void acceptClient();

    void startListening();

    std::string getClientMessage();

    void sendClientMessage(const std::string &hello) const noexcept;

    pqxx::connection* getDatabaseConnection() {return pgsql;}

    ~TwoSidesListener();

private:
    // client connection
    int server_fd{}, new_socket{};
    struct sockaddr_in address{};
    int opt{};
    int addrlen{};
    // database connection
    pqxx::connection *pgsql;
};


#endif //SIMPLESERVER_TWOSIDESLISTENER_H
