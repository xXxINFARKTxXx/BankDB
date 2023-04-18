cmake_minimum_required(VERSION 3.25)
project(Client)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(Client main.cpp ConnControler.cpp ConnControler.h Interactor.h)
