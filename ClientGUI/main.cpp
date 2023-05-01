#include "login.h"

#include <QApplication>
#include <QMessageBox>

#include "ConnControler.h"

#define SERVER_PORT 8080
#define SERVER_IP_ADDRESS "127.0.0.1"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ConnControler *cc{};
    try {
        cc = new ConnControler(SERVER_PORT, SERVER_IP_ADDRESS);
    } catch(std::exception &e) {
        QMessageBox::critical(nullptr, "Error", "Can not connect to server!\n");
        exit(1);
    }

    Login login(nullptr, cc);

    login.show();

    return a.exec();
}
