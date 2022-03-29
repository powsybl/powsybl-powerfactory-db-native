# Copyright (c) 2022, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.21)

project(powsybl-powerfactory-db-native)

find_package(JNI REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(POWERFACTORY_HOME "C:\\Program Files\\DIgSILENT\\PowerFactory 2022 SP1")

add_library(powerfactory-api STATIC IMPORTED)
set_target_properties(powerfactory-api PROPERTIES IMPORTED_LOCATION ${POWERFACTORY_HOME}\\Api\\lib\\VS2019\\digapivalue.lib)
set_target_properties(powerfactory-api PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${POWERFACTORY_HOME}\\Api\\include)

set(SOURCES src/db.cpp src/main.cpp src/Api.cpp)
add_library(powsybl-powerfactory-db-native SHARED ${SOURCES})
set_target_properties(powsybl-powerfactory-db-native PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target/classes/natives/windows_64")

#add_executable(powsybl-powerfactory-db-native)
target_include_directories(powsybl-powerfactory-db-native PUBLIC ${JNI_INCLUDE_DIRS})
target_link_libraries(powsybl-powerfactory-db-native powerfactory-api)
