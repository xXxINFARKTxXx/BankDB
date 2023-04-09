// Client side C/C++ program to demonstrate Socket
// programming
#include "ConnControler.h"

#define SERVER_PORT 8080
#define SERVER_IP_ADDRESS "192.168.0.27"

int main(int argc, char const* argv[])
{
    ConnControler contr(SERVER_PORT, SERVER_IP_ADDRESS);
    contr.sendMessage("SELECT COUNT(*) FROM users");
    std::cout << contr.getMessage() << std::endl;
    return 0;
}