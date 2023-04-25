#define SERV_PORT 8080
#define DB_PORT 5432
#define DB_IP_ADDRESS "127.0.0.1"

#include "TwoSidesListener.h"
#include "RequestManager.h"

using json = nlohmann::json;

void clientHandling() {

}

int main() {
    TwoSidesListener *t{};
    try {
        t = new TwoSidesListener(SERV_PORT, DB_IP_ADDRESS, DB_PORT);
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    t->startListening();
    std::cout << "Starting listening..." << std::endl;
    t->acceptClient();
    std::cout << "Client found" << std::endl;

    while (true) {
        std::string requeststr = t->getClientMessage();
        if(requeststr.empty()) {
            std::cout << "Connection broken." << std::endl;
            std::cout << "Waiting for clients..." << std::endl;
            t->acceptClient();
            std::cout << "Client found" << std::endl;
        } else
        try {
            json clientRequest = json::parse(requeststr);
            switch ((int) clientRequest["type"]) {
                case 0: { // authentication
                    try{
                        t->sendClientMessage(RequestManager::authCheck(clientRequest, t));
                    }
                    catch (std::exception &e) {
                        std::cout << "0:   " << e.what();
                        exit(1);
                    }
                    break;
                }
                case 1: { // registration
                    try{
                        t->sendClientMessage(RequestManager::registration(clientRequest, t));
                    }
                    catch (std::exception &e) {
                        std::cout << "1:   " << e.what();
                        exit(1);
                    }
                    break;
                }
                case 2: { // conduct transfer
                    try{
                        t->sendClientMessage(RequestManager::conductTranfer(clientRequest, t));
                    }
                    catch (std::exception &e) {
                        std::cout << "2:   " << e.what();
                        exit(1);
                    }
                    break;
                }
                case 3: { // open new debit account
                    try{
                        t->sendClientMessage(RequestManager::createDebitAcc(clientRequest, t));
                    }
                    catch (std::exception &e) {
                        std::cout << "3:   " << e.what();
                        exit(1);
                    }
                    break;
                }
                case 4: { // close debit account
                    try{
                        t->sendClientMessage(RequestManager::deleteDebitAcc(clientRequest, t));
                    }
                    catch (std::exception &e) {
                        std::cout << "4:   " << e.what();
                        exit(1);
                    }
                    break;
                }
                case 5: { // get information about debit accounts
                    try{
                        t->sendClientMessage(RequestManager::getDebitAccsInfo(clientRequest, t));
                    }
                    catch (std::exception &e) {
                        std::cout << "5:   " << e.what();
                        exit(1);
                    }
                    break;
                }
                case 6: { // get information about bank account
                    try{
                        t->sendClientMessage(RequestManager::getBankAccInfo(clientRequest, t));
                    }
                    catch (std::exception &e) {
                        std::cout << "6:   " << e.what();
                        exit(1);
                    }
                    break;
                }
            }
        }
        catch (std::exception &e) {
            std::cout << e.what();
        }
    }

    delete t;
    return 0;
}