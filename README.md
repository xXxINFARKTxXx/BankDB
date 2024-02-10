# BankDB
Simple bank client <-> bank service <-> database implementation.  
  - Client: QT6, nlohmann/json
  - Service: libpqxx, nlohmann/json, stdlib C row sockets.
  - Database: PostgreSQL, PL/PGSQL, bash

Required:
  - QT6
  - nlohmann/json
  - libpqxx
  - PostgreSQL

To run application:
  1) Config and run Database
     - run psql with user `postgres` and password `postgres` on localhost:5432
     - `cd Database`
     - `./reinitdb.sh`
  3) Build and run Server
     - `cd Server`
     - `mkdir build && cd build && cmake .. && make`
     - `./Server`
  4) Build and run Client
     - `cd ClientGUI`
     - `mkdir build && cd build && qmake6 .. && make -j16`
     - `./BankClient`
  

