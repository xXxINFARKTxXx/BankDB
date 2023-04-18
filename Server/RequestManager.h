//
// Created by vladimir on 15.4.23..
//

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include "TwoSidesListener.h"

using json = nlohmann::json;

#ifndef SERVER_REQUESTMANAGER_H
#define SERVER_REQUESTMANAGER_H

struct RequestManager {
    static std::string authCheck(json &clientRequest, TwoSidesListener *pListener) {

        pqxx::nontransaction txn(*pListener->getDatabaseConnection());

        std::string request = "SELECT "
                              "    COUNT(*), "
                              "    user_id, "
                              "    first_name, "
                              "    second_name "
                              "FROM users "
                              "INNER JOIN user_data "
                              "ON users.user_id = user_data.passport_id "
                              "WHERE login = " + txn.quote(clientRequest["login"].get<std::string>()) +
                              " AND password = " + txn.quote(clientRequest["password"].get<std::string>()) +
                              "GROUP BY user_id, first_name, second_name";

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

    static std::string registration(json &clientRequest, TwoSidesListener *pListener) {

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


    /*
        clientRequest = {
            "owners_id":    user id of the owner's of debit account
            "from":         account id from(can be 0, long)
            "to":           account id to(long)
            "amount":       amount of money transfer(double)
        }

        returnValue {
            "result":           TRUE if successful, else FALSE
            "description": description if error happened
        }
     */
    static std::string conductTranfer(json &clientRequest, TwoSidesListener *pListener) {
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

            for(auto i: accounts) {
                for (auto j: i)
                    std::cout << j << "\t\t";
                std::cout << std::endl;
            }

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


    /*
     * json {"user_id": long long}
     * */
    static std::string createDebitAcc(json &clientRequest, TwoSidesListener *pListener) {
        pqxx::nontransaction txn{*pListener->getDatabaseConnection()};
        txn.exec("INSERT INTO accounts(user_id) VALUES(" +
                 std::to_string(clientRequest["user_id"].get<long long>()) + ")");
        auto res = txn.exec("SELECT MAX(account_id) FROM accounts "
                            "WHERE user_id = " + std::to_string(clientRequest["user_id"].get<long long>()));
        return json{{"account_id", res[0][0].as<long long>()}}.dump();
    }

    static std::string deleteDebitAcc(json &clientRequest, TwoSidesListener *pListener) {
        return std::string{};
    }

    static std::string getDebitAccsInfo(json &clientRequest, TwoSidesListener *pListener) {
        return std::string{};
    }

    /*
     > {"user_id", long long}
     < {"accounts", []}
     */
    static std::string getBankAccInfo(json &clientRequest, TwoSidesListener *pListener) {

        return std::string{};
    }
};


#endif //SERVER_REQUESTMANAGER_H
