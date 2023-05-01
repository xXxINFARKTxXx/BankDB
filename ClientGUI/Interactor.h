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
#include <iomanip>

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
    static json welcome(const ConnControler &conContr);


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
    static std::string authentication(const ConnControler &conContr);


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
    static std::string registration(const ConnControler &conContr);

    static void conductTransfer(const json &user, const ConnControler &conContr);

    static void createAcc(const json &user, const ConnControler &conContr);

    static void getDebitAccsInfo(const json &user, const ConnControler &conContr);

    static void getAccountInfo(const json &user, const ConnControler &conContr);

    static void deleteDebitAcc(const json &user, const ConnControler &conContr);
private:
};


#endif //CLIENT_INTERACTOR_H
