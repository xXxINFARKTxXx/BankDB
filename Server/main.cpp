#define SERV_PORT 8080
#define DB_PORT 5432
#define DB_IP_ADDRESS "127.0.0.1"

#include "TwoSidesListener.h"
#include "RequestManager.h"

#include <thread>

using json = nlohmann::json;

void clientHandling(TwoSidesListener* pListener, int clientDescriptor) {
    auto t = pListener;
    auto connfd = clientDescriptor;

    json clientRequest{};
    std::string requeststr{};

    while(!(requeststr = t->getClientMessage(connfd)).empty()) {
        try {
            clientRequest = json::parse(requeststr);
            std::cout << clientRequest.dump(4) << std::endl;
            switch ((int) clientRequest["type"]) {
                case 0: { // authentication
                    try{
                        t->sendClientMessage(RequestManager::authCheck(clientRequest, t), connfd);
                    }
                    catch (std::exception &e) {
                        std::cout << "0:   " << e.what();
                        exit(1);
                    }
                    break;
                }
                case 1: { // registration
                    try{
                        t->sendClientMessage(RequestManager::registration(clientRequest, t), connfd);
                    }
                    catch (std::exception &e) {
                        std::cout << "1:   " << e.what();
                        exit(1);
                    }
                    break;
                }
                case 2: { // conduct transfer
                    try{
                        t->sendClientMessage(RequestManager::conductTranfer(clientRequest, t), connfd);
                    }
                    catch (std::exception &e) {
                        std::cout << "2:   " << e.what();
                        exit(1);
                    }
                    break;
                }
                case 3: { // open new debit account
                    try{
                        t->sendClientMessage(RequestManager::createDebitAcc(clientRequest, t), connfd);
                    }
                    catch (std::exception &e) {
                        std::cout << "3:   " << e.what();
                        exit(1);
                    }
                    break;
                }
                case 4: { // close debit account
                    try{
                        t->sendClientMessage(RequestManager::deleteDebitAcc(clientRequest, t), connfd);
                    }
                    catch (std::exception &e) {
                        std::cout << "4:   " << e.what();
                        exit(1);
                    }
                    break;
                }
                case 5: { // get information about debit accounts
                    try{
                        t->sendClientMessage(RequestManager::getDebitAccsInfo(clientRequest, t), connfd);
                    }
                    catch (std::exception &e) {
                        std::cout << "5:   " << e.what();
                        exit(1);
                    }
                    break;
                }
                case 6: { // get information about bank account
                    try{
                        t->sendClientMessage(RequestManager::getBankAccInfo(clientRequest, t), connfd);
                    }
                    catch (std::exception &e) {
                        std::cout << "6:   " << e.what();
                        exit(1);
                    }
                    break;
                }
                case 7: {
                try {
                    t->sendClientMessage(RequestManager::getTransactionsInfo(clientRequest, t), connfd);
                }
                catch (std::exception &e) {
                    std::cout << "7:\t" << e.what();
                }
                }
            }
        }
        catch (std::exception &e) {
            std::cout << e.what();
        }
        std::cout << "Request completed!" << std::endl;
    }

    t->closeConnectionfd(connfd);
    std::cout << "Connection broken." << std::endl;
    return;
}

int main() {
    TwoSidesListener *t{};

    // try to open connection
    try {
        t = new TwoSidesListener(SERV_PORT, DB_IP_ADDRESS, DB_PORT);
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    t->startListening();
    std::cout << "Starting listening..." << std::endl;

    while (true) {
        std::cout << "Waiting for clients..." << std::endl;
        int clientDescriptor = t->acceptClient();
        if(clientDescriptor < 0) std::cout << "Unable to open connection" << std::endl;
        std::cout << "Client found" << std::endl;

        std::thread(clientHandling, t, clientDescriptor).detach();
    }

    delete t;
    std::cout << "server shutdown";
    return 0;
}

/*
 y
RZ49rXZuKtjycoPI9Ca1
9SOpaTtxDErHlDuPsjHg

y
12345678901234567890
12345678901234567890

 * */