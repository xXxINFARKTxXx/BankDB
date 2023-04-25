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
    static std::string authCheck(json &clientRequest, TwoSidesListener *pListener);

    static std::string registration(json &clientRequest, TwoSidesListener *pListener);


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
    static std::string conductTranfer(json &clientRequest, TwoSidesListener *pListener);


    /*
     * json {"user_id": long long}
     * */
    static std::string createDebitAcc(json &clientRequest, TwoSidesListener *pListener);

    static std::string deleteDebitAcc(json &clientRequest, TwoSidesListener *pListener);

    static std::string getDebitAccsInfo(json &clientRequest, TwoSidesListener *pListener);

    /*
     > {"user_id", long long}
     < {"accounts", []}
     */
    static std::string getBankAccInfo(json &clientRequest, TwoSidesListener *pListener);
};

#endif //SERVER_REQUESTMANAGER_H