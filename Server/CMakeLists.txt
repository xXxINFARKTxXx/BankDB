cmake_minimum_required(VERSION 3.25)
project(Server)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(Server main.cpp TwoSidesListener.cpp TwoSidesListener.h RequestManager.h)

target_link_libraries(Server PRIVATE -lpqxx -lpq)
