cmake_minimum_required(VERSION 3.21)
project(powsybl-powerfactory-db-native)

set(CMAKE_CXX_STANDARD 14)
set(POWERFACTORY_HOME "C:\\Program Files\\DIgSILENT\\PowerFactory 2022 SP1")

add_library(powerfactory-api STATIC IMPORTED)
set_target_properties(powerfactory-api PROPERTIES IMPORTED_LOCATION ${POWERFACTORY_HOME}\\Api\\lib\\VS2019\\digapivalue.lib)
set_target_properties(powerfactory-api PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${POWERFACTORY_HOME}\\Api\\include)

add_executable(powsybl-powerfactory-db-native main.cpp)
target_link_libraries(powsybl-powerfactory-db-native powerfactory-api)
