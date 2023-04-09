// TwoSidesListener side C/C++ program to demonstrate Socket
// programming

#define SERV_PORT 8080
#define DB_PORT 5432
#define DB_IP_ADDRESS "192.168.0.26"

#include "TwoSidesListener.h"
#include <iostream>

int main(int argc, char const* argv[])
{
    TwoSidesListener *t{};
    try {
        t = new TwoSidesListener (SERV_PORT, DB_IP_ADDRESS, DB_PORT);
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl; return 1;
    }
    t->startClientListening();
    std::cout << t->getClientMessage() << std::endl;
    t->sendClientMessage("Hello from server");

    delete t;
    return 0;
}