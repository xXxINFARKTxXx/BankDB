#include "ConnControler.h"
#include "Interactor.h"

#define SERVER_PORT 8080
#define SERVER_IP_ADDRESS "127.0.0.1"

int main(int argc, char const* argv[])
{
    ConnControler contr(SERVER_PORT, SERVER_IP_ADDRESS);

    json user = Interactor::welcome(contr);

    Interactor::createAcc(user, contr);

    Interactor::conductTransfere(user, contr);

    return 0;
}

/*
y
KQRkbeCEwAFSPmAlYKQ7
Q6nlueQ9AWKK3FDvP9b5
 0
1000000000000165
 -5000
1000000000000088
50000
120000
 */

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