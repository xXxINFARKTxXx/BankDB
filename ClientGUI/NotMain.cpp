#include "ConnControler.h"
#include "Interactor.h"

#define SERVER_PORT 8080
#define SERVER_IP_ADDRESS "127.0.0.1"

int notMainFinctions()
{
    ConnControler contr(SERVER_PORT, SERVER_IP_ADDRESS);

    json user = Interactor::welcome(contr);

    if(user.empty()) return 0;

    for(;;) {
        int option;
        std::cout << std::endl << std::endl << "Menu" << std::endl <<
                     "Conduct transfer...........................1" << std::endl <<
                     "Open new debit acc.........................2" << std::endl <<
//                     "Close debit account........................3" << std::endl <<
                     "Get information about all debit accounts...3" << std::endl <<
                     "Get information about bank account.........4" << std::endl <<
                     "Exit.......................................5" << std::endl <<
                     "Choose option: ";
        std::cin >> option;

        switch (option) {
            case 1: {
                Interactor::conductTransfer(user, contr);
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            case 2: {
                Interactor::createAcc(user, contr);
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
//            case 3: {
//                Interactor::deleteDebitAcc(user, contr);
//                break;
//            }
            case 3: {
                Interactor::getDebitAccsInfo(user, contr);
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            case 4: {
                Interactor::getAccountInfo(user, contr);
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            case 5: {
                std::cout << "See ya!\n";
                return 0;
            }
            default:{
                std::cout << "Wrong option!\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
}

/*
y
RZ49rXZuKtjycoPI9Ca1
9SOpaTtxDErHlDuPsjHg
1000000000000286

 0
1000000000000165
 -5000
1000000000000088
50000
120000
 */


// https://github.com/xXxINFARKTxXx/BankDB.git
/*
n
4874241622
Елена
Монакевич
Сергеевна
0
08-09-1982
г. Нижний Новгород
Россия
27-02-2014
27-02-2029
Россия, г. Невинномысск, Речной пер., д. 21 кв.63
Управление внутренних дел по г. Нижний Новгород
Россия, г. Нижний Новгород, Мирная ул., д. 25 кв.112
lyudmila26@gmail.com
 */
