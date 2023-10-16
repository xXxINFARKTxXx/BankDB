//
// Created by vladimir on 15.4.23..
//

#include "RequestManager.h"

std::string RequestManager::authCheck(json &clientRequest, TwoSidesListener *pListener) {

    pqxx::nontransaction txn(*pListener->getDatabaseConnection());

    std::string request =
            "SELECT "
                "result::int, "
                "uid::bigint, "
                "name::text, "
                "surname::text "
            "FROM authCheck(" + txn.quote(clientRequest["login"].get<std::string>()) + ", " +
                                txn.quote(clientRequest["password"].get<std::string>()) + ")";

    pqxx::result res{txn.exec(request)};
    json result{};

    if (!res.empty())
        return json{
                {"result",  res.begin().begin().as<bool>()},
                {"user_id", (res.begin().begin() + 1).as<long>()},
                {"name",    (res.begin().begin() + 2).as<std::string>()},
                {"surname", (res.begin().begin() + 3).as<std::string>()},
        }.dump();
    else
        return R"({"result": false})";
}

std::string RequestManager::registration(json &clientRequest, TwoSidesListener *pListener) {

    pqxx::nontransaction txn(*pListener->getDatabaseConnection());
    std::string request;
    request = "INSERT INTO user_data("
              "    passport_id,"
              "    first_name,"
              "    second_name,"
              "    last_name,"
              "    sex,"
              "    date_of_birth,"
              "    place_of_birth,"
              "    country,"
              "    passport_date_of_issue,"
              "    passport_date_of_expire,"
              "    passport_place_of_issue,"
              "    passport_authority,"
              "    adress_of_living,"
              "    email"
              "    )"
              "VALUES("
              + clientRequest["passport_id"].get<std::string>() + ", "
              + txn.quote(clientRequest["first_name"].get<std::string>()) + ", "
              + txn.quote(clientRequest["second_name"].get<std::string>()) + ", "
              + txn.quote(clientRequest["last_name"].get<std::string>()) + ", "
              + std::to_string(clientRequest["sex"].get<bool>()) + "::boolean, "
              + txn.quote(clientRequest["date_of_birth"].get<std::string>()) + ", "
              + txn.quote(clientRequest["place_of_birth"].get<std::string>()) + ", "
              + txn.quote(clientRequest["country"].get<std::string>()) + ", "
              + txn.quote(clientRequest["passport_date_of_issue"].get<std::string>()) + ", "
              + txn.quote(clientRequest["passport_date_of_expire"].get<std::string>()) + ", "
              + txn.quote(clientRequest["passport_place_of_issue"].get<std::string>()) + ", "
              + txn.quote(clientRequest["passport_authority"].get<std::string>()) + ", "
              + txn.quote(clientRequest["address_of_living"].get<std::string>()) + ", "
              + txn.quote(clientRequest["email"].get<std::string>()) + ")";


    pqxx::result res;
    try {
        txn.exec(request);
    } catch (const std::exception &e) {
        return R"({"result": false})";
    }

    auto str =
            "SELECT login, password FROM users WHERE user_id = " + clientRequest["passport_id"].get<std::string>();

    res = txn.exec(str);

    return json{
            {"result",   true},
            {"user_id",  clientRequest["passport_id"].get<std::string>()},
            {"name",     clientRequest["first_name"].get<std::string>()},
            {"surname",  clientRequest["second_name"].get<std::string>()},
            {"login",    res.begin().begin().as<std::string>()},
            {"password", (res.begin().begin() + 1).as<std::string>()}
    }.dump();
}

std::string RequestManager::conductTranfer(json &clientRequest, TwoSidesListener *pListener) {
    pqxx::nontransaction txn{*pListener->getDatabaseConnection()};
    long from, to, ownersID, ownersBalance;
    double amount;
    pqxx::result accounts;

    accounts = txn.exec(
            "SELECT account_id, user_id, deposit "
            "FROM accounts "
            "WHERE user_id = " + std::to_string(clientRequest["owners_id"].get<long long>()) + " "
                                                                                               "AND account_id = " +
            std::to_string(clientRequest["from"].get<long long>()) + " "
                                                                     "OR account_id = " +
            std::to_string(clientRequest["to"].get<long long>())
    );


    if (!clientRequest["from"].get<long long>()) { // если кладем или снимаем деньги со своего аккаунта

        if (accounts.empty()) { // аккаунт не найден
            return R"({
                    "result": false,
                    "description": "This debit account does not exists"
                })";
        }

        to = accounts[0][0].as<long>();
        amount = accounts[0][2].as<double>();

        if (amount + clientRequest["amount"].get<double>() < 0) // не достаточно средств на аккаунте
            return R"({
                    "result": false,
                    "description": "Your debit account has insufficient funds"
                })";

        txn.exec("INSERT INTO transfers(from_id, to_id, amount) VALUES "
                 "( NULL, " + std::to_string(to) + ", " +
                 std::to_string(clientRequest["amount"].get<double>()) + ")");

        txn.exec("UPDATE accounts SET deposit = deposit + " +
                 std::to_string(clientRequest["amount"].get<double>()) +
                 " WHERE account_id = " + std::to_string(to));


    } else { // переводим деньги с одного аккаунта на другой

        if (accounts.size() != 2) { // не найден аккаунт откуда и куда будет происходить перевод
            return R"({
                    "result": false,
                    "description": "One of the accounts does not exists"
                })";
        }

        // инициализируем кто куда переводит сколько переводит деньги
        if (clientRequest["from"].get<long>() == accounts[0][0].as<long>()) {
            from = accounts[0][0].as<long>(),
            ownersID = accounts[0][1].as<long>(),
            ownersBalance = accounts[0][2].as<long>(),
            to = accounts[1][0].as<long>();
        } else {
            from = accounts[1][0].as<long>(),
            ownersID = accounts[1][1].as<long>(),
            ownersBalance = accounts[1][2].as<long>(),
            to = accounts[0][0].as<long>();
        }


        if (ownersID != clientRequest["owners_id"]) { // если владелец аккаунта не правильный
            return R"({
                    "result": false,
                    "description": "Your account does not exists"
                })";
        }

        if (ownersBalance < clientRequest["amount"]) { // если мало денег на балансе откуда перевод
            return R"({
                    "result": false,
                    "description": "Not enough money on your account"
                })";
        }

        // осуществляем перевод
        amount = clientRequest["amount"].get<double>();
        txn.exec("INSERT INTO transfers(from_id, to_id, amount) VALUES "
                 "(" + std::to_string(from) + ", " + std::to_string(to) + ", " +
                 std::to_string(amount) + ")");

        txn.exec("UPDATE accounts SET deposit = deposit - " +
                 std::to_string(amount) +
                 " WHERE account_id = " + std::to_string(from));

        txn.exec("UPDATE accounts SET deposit = deposit + " +
                 std::to_string(amount) +
                 " WHERE account_id = " + std::to_string(to));
    }

    return R"({
                    "result": true,
                    "description": "Operation succeed"
                })";
}

std::string RequestManager::createDebitAcc(json &clientRequest, TwoSidesListener *pListener) {
    pqxx::nontransaction txn{*pListener->getDatabaseConnection()};
    txn.exec("INSERT INTO accounts(user_id) VALUES(" +
             std::to_string(clientRequest["user_id"].get<long long>()) + ")");
    auto res = txn.exec("SELECT MAX(account_id) FROM accounts "
                        "WHERE user_id = " + std::to_string(clientRequest["user_id"].get<long long>()));
    return json{{"account_id", res[0][0].as<long long>()}}.dump();
}

std::string RequestManager::deleteDebitAcc(json &clientRequest, TwoSidesListener *pListener) {
    pqxx::work deleteAccount{*pListener->getDatabaseConnection()};

    std::string databaseRequest {"SELECT closeDebitAccount(" + clientRequest["account_id"].get<std::string>() + ", " +
                                                               clientRequest["user_id"].get<std::string>() + ")"
    };
    pqxx::result res = deleteAccount.exec(databaseRequest);

    deleteAccount.commit();

    if(res.begin().begin().as<bool>())
        return R"({"result": true})";
    else
        return R"({"result": false})";
}

std::string RequestManager::getDebitAccsInfo(json &clientRequest, TwoSidesListener *pListener) {

    pqxx::nontransaction getDebitAccsInfo(*pListener->getDatabaseConnection());

    std::string fieldNames[] {};

    json answer = json::array();

    std::string request{
            "SELECT * FROM accounts "
            "WHERE user_id = " + std::to_string(clientRequest["user_id"].get<long long>())
    };

    auto info = getDebitAccsInfo.exec(request);

    int i{};
    for(auto row: info) {

        json account {
                {
                        {"account_number", (row.begin() + 0).as<std::string>()},
                        {"deposit", (row.begin() + 2).as<std::string>()},
                        {"date_and_time_of_opening", (row.begin() + 3).as<std::string>()}
                }
        };
        answer.insert(answer.end(), account);
        i++;
    }

    return answer.dump();
}

std::string RequestManager::getBankAccInfo(json &clientRequest, TwoSidesListener *pListener) {
    pqxx::nontransaction getBankAccInfo(*pListener->getDatabaseConnection());

    auto res = getBankAccInfo.exec(
            "SELECT * "
            "FROM user_data "
            "WHERE passport_id = " + clientRequest["user_id"].get<std::string>()
    ).begin();

    json answer {
            {"name",                    (res.begin() + 1).as<std::string>()},
            {"surname",                 (res.begin() + 2).as<std::string>()},
            {"last_name",               (res.begin() + 3).as<std::string>()},
            {"sex",                     (res.begin() + 4).as<std::string>()},
            {"date_of_birth",           (res.begin() + 5).as<std::string>()},
            {"place_of_birth",          (res.begin() + 6).as<std::string>()},
            {"country",                 (res.begin() + 7).as<std::string>()},
            {"passport_date_of_issue",  (res.begin() + 8).as<std::string>()},
            {"passport_date_of_expire", (res.begin() + 9).as<std::string>()},
            {"passport_place_of_issue", (res.begin() + 10).as<std::string>()},
            {"passport_authority",      (res.begin() + 11).as<std::string>()},
            {"address_of_living",        (res.begin() + 12).as<std::string>()},
            {"email",                   (res.begin() + 13).as<std::string>()}
    };

    return answer.dump();
}

std::string RequestManager::getTransactionsInfo(json &clientRequest, TwoSidesListener *pListener) {

    pqxx::nontransaction getDebitAccsInfo(*pListener->getDatabaseConnection());

    std::string fieldNames[] {};

    json answer = json::array();

    std::string request{
            "SELECT "
            "tr_amount           ::double precision  AS amount, "
            "tr_from_id          ::bigint            AS from_id, "
            "tr_to_id            ::bigint            AS to_id, "
            "tr_date_and_time    ::text              AS date_and_time "
            "FROM getTransactionHistory(" + clientRequest["user_id"].get<std::string>() + ") "
                                                                                          "ORDER BY tr_date_and_time"
    };

    auto info = getDebitAccsInfo.exec(request);

    int i = 1;
    for(auto row: info) {
        json account {
                {
                        {"id", std::to_string(i)},
                        {"amount", (row.begin() + 0).as<std::string>()},
                        {"from_id", (row.begin() + 1).is_null() ? "empty" : (row.begin() + 1).as<std::string>()},
                        {"to_id", (row.begin() + 2).as<std::string>()},
                        {"date_and_time", (row.begin() + 3).as<std::string>()}
                }
        };
        answer.insert(answer.end(), account);
        i++;
    }

    std::cout << answer.dump(4);
    return answer.dump();
}
