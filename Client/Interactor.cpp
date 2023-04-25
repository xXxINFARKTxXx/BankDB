//
// Created by vladimir on 10.4.23..
//

#include "Interactor.h"

json Interactor::welcome(const ConnControler &conContr) {
    std::string option;

    while (true) {
        std::cout << "Welcome! Do you have account? y\\n\\exit: ";
        std::cin >> option;
        if (option == "y" || option == "yes" || option == "Y" || option == "YES") {
            auto res = json::parse(authentication(conContr));

            if (res["result"]) {
                std::cout << "Authentication success" << std::endl;
                std::cout << std::endl << "Welcome, " << res["name"].get<std::string>() << " "
                          << res["surname"].get<std::string>() << "!" << std::endl;
                res.erase("result");
                return res;
            } else {
                std::cout << "Wrong login or password!\n";
            }
        } else if (option == "n" || option == "no" || option == "N" || option == "NO") {
            auto res = json::parse(registration(conContr));

            if (res["result"]) {
                std::cout << "Registration success\n";
                std::cout << std::endl << "Welcome aboard, " << res["name"].get<std::string>() << " "
                          << res["surname"].get<std::string>() << "!" << std::endl;
                std::cout << "Your login is " << res["login"].get<std::string>() << " and password is "
                          << res["password"].get<std::string>() << std::endl;
                res.erase("result");
                return res;
            } else {
                std::cout << "Wrong personal data input!\n";
            }
        } else if (option == "exit") {
            std::cout << "See ya!\n";
            return json{};
        }else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Wrong option\n";
        }
    }
}

std::string Interactor::authentication(const ConnControler &conContr) {
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

std::string Interactor::registration(const ConnControler &conContr) {

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

void Interactor::conductTransfer(const json &user, const ConnControler &conContr) {
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

void Interactor::createAcc(const json &user, const ConnControler &conContr) {
    conContr.sendMessage(json {{"type", 3}, {"user_id", user["user_id"].get<long long>()}}.dump());
    std::cout << "Your new account ID number:" << json::parse(conContr.getMessage())["account_id"].get<long long>() << std::endl;
}

void Interactor::getDebitAccsInfo(const json &user, const ConnControler &conContr) {
    conContr.sendMessage(json {{"type", 5}, {"user_id", user["user_id"].get<long long>()}}.dump());
    json answer = json::parse(conContr.getMessage());
    answer.empty();
    if(answer.empty())
        std::cout << "You have no accounts" << std::endl;
    else {
        int i{};
        for (auto &it: answer) {
            std::cout << "Account № " << std::left << std::setw(3) << ++i <<
                      "\tAccount number: " << std::left << it[0]["account_number"].get<std::string>() <<
                      "\tDeposit: " << std::left << std::setw(10) << it[0]["deposit"].get<std::string>() <<
                      "\tDate and time of opening: " << std::left << std::setw(25) << it[0]["date_and_time_of_opening"].get<std::string>().erase(19);
            std::cout << std::endl;
        }
    }
}

void Interactor::getAccountInfo(const json &user, const ConnControler &conContr) {
    std::string request = json{
            {"type", 6},
            {"user_id", std::to_string(user["user_id"].get<long long>())}
    }.dump();
    conContr.sendMessage(request);
    json answer = json::parse(conContr.getMessage());
    std::cout << std::endl << "Name: \t\t\t\t" << answer["name"].get<std::string>()<< std::endl;
    std::cout << "Surname: \t\t\t" << answer["surname"].get<std::string>()<< std::endl;
    std::cout << "Last name: \t\t\t" << answer["last_name"].get<std::string>()<< std::endl;
    std::cout << "Sex: \t\t\t\t" << (answer["sex"].get<std::string>() == "f" ? "female" : "male") << std::endl;
    std::cout << "Date of birth: \t\t\t" << answer["date_of_birth"].get<std::string>()<< std::endl;
    std::cout << "Place of birth: \t\t" << answer["place_of_birth"].get<std::string>()<< std::endl;
    std::cout << "Country: \t\t\t" << answer["country"].get<std::string>()<< std::endl;
    std::cout << "Passport date of issue: \t" << answer["passport_date_of_issue"].get<std::string>()<< std::endl;
    std::cout << "Passport date of expire: \t" << answer["passport_date_of_expire"].get<std::string>()<< std::endl;
    std::cout << "Passport place of issue: \t" << answer["passport_place_of_issue"].get<std::string>()<< std::endl;
    std::cout << "Passport authority: \t\t" << answer["passport_authority"].get<std::string>()<< std::endl;
    std::cout << "Address_of_living: \t\t" << answer["address_of_living"].get<std::string>()<< std::endl;
    std::cout << "Email: \t\t\t\t" << answer["email"].get<std::string>()<< std::endl;
}

void Interactor::deleteDebitAcc(const json &user, const ConnControler &conContr) {
    std::string account_id;
    std::cout << "Enter ID of account, that you would like to close" << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, account_id);

    std::string request = json{
            {"type", 4},
            {"user_id", std::to_string(user["user_id"].get<long long>())},
            {"account_id", account_id},
    }.dump();

    conContr.sendMessage(request);

    if(json::parse(conContr.getMessage())["result"].get<bool>()) {
        std::cout << "Your account successfully deleted!" << std::endl;
        return;
    } else {
        std::cout << "Account not found!" << std::endl;
    }

}
