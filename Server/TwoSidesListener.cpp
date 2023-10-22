#include "TwoSidesListener.h"

#define DBNAME bank
#define USER postgres
#define PASSWORD postgres

#define VALUE(string) #string
#define TO_LITERAL(string) VALUE(string)

TwoSidesListener::TwoSidesListener(const unsigned port, const std::string &dataBaseIP,
                                   const unsigned databasePort) : opt{1}, addrlen{sizeof(sockaddr_in)},
                                                                  address{AF_INET, htons(port),
                                                                          INADDR_ANY} {
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw std::runtime_error("Socket failed\n");
    }

    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        throw std::runtime_error("Setting socket options failed\n");
    }

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *) &address,
             sizeof(address))
        < 0) {
        throw std::runtime_error("Bind failed\n");
    }

    std::string params =
            "dbname=" TO_LITERAL(DBNAME) " user=" TO_LITERAL(USER)
            " password=" TO_LITERAL(PASSWORD) " hostaddr=" + dataBaseIP + " port=" +
            std::to_string(databasePort);
    try {
        // make DB connection
        pgsql = new pqxx::connection(params);
    } catch (const std::exception &log) {
        // connection failed
        throw std::runtime_error("DB connection failed\n");
    }
}

int TwoSidesListener::acceptClient() {
    int new_socket{};
    if ((new_socket
                 = accept(server_fd, (struct sockaddr *) &address,
                          (socklen_t *) &addrlen))
        < 0) {
        throw std::runtime_error("Error accepting\n");
    }
    client_sockets.insert(new_socket);
    return new_socket;
}


std::string TwoSidesListener::getClientMessage(int client_socket) {
    char tempBuffer[BUFLEN]{};
    if (read(client_socket, tempBuffer, BUFLEN) < 0)
        throw std::runtime_error("Error reading\n");

    return std::string{tempBuffer};
}

void TwoSidesListener::sendClientMessage(const std::string &hello, int client_socket) const noexcept {
    send(client_socket, hello.c_str(), hello.length(), 0);
}

TwoSidesListener::~TwoSidesListener() {
    // closing the connected sockets
    for (auto i: client_sockets) {
        close(i);
    }
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);

    delete pgsql;
}

void TwoSidesListener::startListening() {
    if (listen(server_fd, 3) < 0) {
        throw std::runtime_error("Error listening\n");
    }
}

