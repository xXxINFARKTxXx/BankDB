//
// Created by vladimir on 10.4.23..
//

#ifndef CLIENT_INTERACTOR_H
#define CLIENT_INTERACTOR_H

#include <iostream>
#include <string>
#include <regex>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>
#include <limits>

#define BAD_RESULT (R"({ "result": false })")

#include "ConnControler.h"

using json = nlohmann::json;

struct Interactor {


    /*
     * returns json  user = {
     *      {user_id: long long}
     *      {name: string}
     *      {surname: string}
     * }
     */
    static auto welcome(const ConnControler &conContr) {
        std::string option;

        while (true) {
            std::cout << "Welcome! Do you have account? y\\n : ";
            std::cin >> option;
            if (option == "y" || option == "yes" || option == "Y" || option == "YES") {
                auto res = json::parse(authentication(conContr));

                if (res["result"]) {
                    std::cout << "Authentication success" << std::endl;
                    std::cout << "Welcome, " << res["name"].get<std::string>() << " "
                              << res["surname"].get<std::string>() << "!" << std::endl;
                    res.erase("result");
                    return res;
                } else {
                    std::cout << "Wrong login or password!\n";
                }
            } else if (option == "n" || option == "no" || option == "N" || option == "NO") {
                auto res = json::parse(registration(conContr));
                std::cout << res << std::endl;
                if (res["result"]) {
                    std::cout << "Registration success\n";
                    std::cout << "Welcome aboard, " << res["name"].get<std::string>() << " "
                              << res["surname"].get<std::string>() << "!" << std::endl;
                    std::cout << "Your login is " << res["login"].get<std::string>() << " and password is "
                              << res["password"].get<std::string>() << std::endl;
                    res.erase("result");
                    return res;
                } else {
                    std::cout << "Wrong personal data input!\n";
                }
            } else std::cout << "Wrong option\n";
        }
    }


    /*
     * returns
     * json_string result_of_operation = {
     *      {result: true}
     *      {user_id: long long}
     *      {name: string}
     *      {surname: string}
     * }
     * OR
     * json_string result_of_operation = {
     *      {result: false}
     * }
     */
    static std::string authentication(const ConnControler &conContr) {
        std::string login{}, password{};
        std::string pattern = "^[0-9a-zA-Z]{20}+$";
        const std::regex exp(pattern);

        std::cout << "Enter login:";
        std::cin >> login;
        if (!std::regex_match(login, exp))
            return BAD_RESULT;

        std::cout << "Enter password:";
        std::cin >> password;
        if (!std::regex_match(password, exp))
            return BAD_RESULT;

        json auth = {
                {"type",     0},
                {"login",    login.c_str()},
                {"password", password.c_str()}
        };

        conContr.sendMessage(auth.dump());
        return conContr.getMessage();
    }


    /*
     * returns
     * json_string result_of_operation = {
     *      {result: true}
     *      {user_id: long long}
     *      {name: string}
     *      {surname: string}
     * }
     * OR
     * json_string result_of_operation = {
     *      {result: false}
     * }
     */
    static std::string registration(const ConnControler &conContr) {

        bool gender{};
        std::string passport_id{}, first_name{}, second_name{},
                last_name{}, date_of_birth{}, place_of_birth{},
                country{},
                passport_date_of_issue{}, passport_date_of_expire{}, passport_place_of_issue{},
                passport_authority{}, address_of_living{}, email{};
        const std::regex name_exp{
                R"(^[\- a-zA-ZабвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ]+$)"};
        const std::regex pid_exp{R"(^[1-9][0-9]{0,}$)"};
        const std::regex email_exp{R"(^\S+@\S+\.\S+$)"};
        const std::regex date_exp{R"(^(0?[1-9]|[12][0-9]|3[01])[\-](0?[1-9]|1[012])[\-]\d{4}$)"};
        const std::regex country_exp{
                R"(^[\- a-zA-ZабвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ]+$)"};

        std::cout << "Enter passport number:";
        std::cin >> passport_id;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!std::regex_match(passport_id, pid_exp))
            return BAD_RESULT;


        std::cout << "Enter first name:";
        std::getline(std::cin, first_name);
        if (!std::regex_match(first_name, name_exp))
            return BAD_RESULT;

        std::cout << "Enter second name:";
        std::getline(std::cin, second_name);
        if (!std::regex_match(second_name, name_exp))
            return BAD_RESULT;

        std::cout << "Enter last name:";
        std::getline(std::cin, last_name);
        if (!std::regex_match(last_name, name_exp))
            return BAD_RESULT;

        std::cout << "Enter gender (1-Male/0-Female):";
        std::cin >> gender;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Enter date of birth in format DD-MM-YYYY:";
        std::getline(std::cin, date_of_birth);
        if (!std::regex_match(date_of_birth, date_exp))
            return BAD_RESULT;

        std::cout << "Enter place of birth:";
        std::getline(std::cin, place_of_birth);

        std::cout << "Enter country of living:";
        std::getline(std::cin, country);
        if (!std::regex_match(country, country_exp))
            return BAD_RESULT;

        std::cout << "Enter date of passport's issue in format DD-MM-YYYY:";
        std::getline(std::cin, passport_date_of_issue);
        if (!std::regex_match(passport_date_of_issue, date_exp))
            return BAD_RESULT;

        std::cout << "Enter date of passport's expire in format DD-MM-YYYY:";
        std::getline(std::cin, passport_date_of_expire);
        if (!std::regex_match(passport_date_of_expire, date_exp))
            return BAD_RESULT;

        std::cout << "Enter passport place of issue:";
        std::getline(std::cin, passport_place_of_issue);

        std::cout << "Enter passport authority:";
        std::getline(std::cin, passport_authority);

        std::cout << "Enter address of living:";
        std::getline(std::cin, address_of_living);

        std::cout << "Enter email:";
        std::getline(std::cin, email);
        if (!std::regex_match(email, email_exp))
            return BAD_RESULT;


        json registration = {
                {"type",                    1},
                {"passport_id",             passport_id.c_str()},
                {"first_name",              first_name.c_str()},
                {"second_name",             second_name.c_str()},
                {"last_name",               last_name.c_str()},
                {"date_of_birth",           date_of_birth.c_str()},
                {"place_of_birth",          place_of_birth.c_str()},
                {"country",                 country.c_str()},
                {"passport_date_of_issue",  passport_date_of_issue.c_str()},
                {"passport_date_of_expire", passport_date_of_expire.c_str()},
                {"passport_place_of_issue", passport_place_of_issue.c_str()},
                {"passport_authority",      passport_authority.c_str()},
                {"address_of_living",       address_of_living.c_str()},
                {"email",                   email.c_str()},
                {"sex",                     gender}
        };
        conContr.sendMessage(registration.dump());
        return conContr.getMessage();
    }


    static void conductTransfere(const json &user, const ConnControler &conContr){
        long long from{-1}, to{-1};
        double amount{-1};
        std::cout << "Enter number of account from which do you want to make transfer\n"
                     "(enter 0 if you want to put money on any debit account):";
        std::cin >> from;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter number of account to which do you want to make transfer:";
        std::cin >> to;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter amount of money (to get money from your debit account enter negative value):";
        std::cin >> amount;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if(!(from == 0 || from >= 1000000000000000) ||
            (to < 1000000000000000) ||
            (from != 0 && amount < 0)) {
            std::cout << "Wrong Input!\n";
            return;
        }
        conContr.sendMessage(
                json {
                        {"type", 2},
                        {"owners_id", user["user_id"].get<long long>()},
                        { "from", from},
                        { "to", to},
                        { "amount", amount},
                }.dump());

        std::cout << json::parse(conContr.getMessage())["description"] << std::endl;

    }

    static void createAcc(const json &user, const ConnControler &connControler) {
        connControler.sendMessage(json {{"type", 3}, {"user_id", user["user_id"].get<long long>()}}.dump());
        std::cout <<"Your new account ID number:" << json::parse(connControler.getMessage())["account_id"].get<long long>() << std::endl;
    }
private:
};


#endif //CLIENT_INTERACTOR_H
