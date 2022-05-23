# ========================= GPSD2eCAL LICENSE =================================
#
# Copyright (C) 2016 - 2019 Continental Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# ========================= GPSD2eCAL LICENSE =================================
#
cmake_minimum_required(VERSION 3.10)
project(gpsd2ecal)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(Gpsd2ecal_VERSION_MAJOR 2)
set(Gpsd2ecal_VERSION_MINOR 0)
set(Gpsd2ecal_VERSION_PATCH 0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(GPS REQUIRED)
find_package(eCAL REQUIRED)
find_package(Protobuf  REQUIRED)
find_package(simpleini  REQUIRED)

if (UNIX)
set(LINUX_ADDON src/convert_utf.cpp include/convert_utf.h)
endif()
create_targets_protobuf()

set(PROJECT_SOURCES
    src/gpsmessage/gpsdfullmsg.cpp
    src/gpsmessage/gpsd.cpp
    src/gpsmessage/gpsmsg.cpp
    src/gpsmessage/msgfactory.cpp
    src/gpsdevice/devicefactory.cpp
    src/gpsdevice/gpsddevice.cpp
    src/main.cpp
    src/gpssettings.cpp
    src/gpshandler.cpp
    src/convert_utf.cpp
    )

set(PROJECT_HEADERS
    include/gpsmessage/gpsdfullmsg.h
    include/gpsmessage/gpsd.h
    include/gpsmessage/gpsmsg.h
    include/gpsmessage/msgfactory.h
    include/gpsdevice/devicefactory.h
    include/gpsdevice/gpsddevice.h
    include/interface/Idevice.h
    include/gpshelper.h
    include/gpssettings.h
    include/gpshandler.h
    include/gpsutil.h
    include/convert_utf.h
    include/stringutils.h
    )

if(UNIX)
  set_source_files_properties(src/convert_utf.cpp PROPERTIES COMPILE_FLAGS -Wno-implicit-fallthrough)
endif()

set(gps2ecal_proto
    ${CMAKE_CURRENT_SOURCE_DIR}/datatypes/gps2ecal.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/datatypes/gpsd.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/datatypes/header.proto
    )

add_executable(${PROJECT_NAME}
               ${PROJECT_SOURCES}
               ${PROJECT_HEADERS}
               )

protobuf_target_cpp(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/datatypes ${gps2ecal_proto})

target_include_directories(${PROJECT_NAME} PUBLIC
                           ${GPS_INCLUDE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/include/gpsmessage
                           ${CMAKE_CURRENT_SOURCE_DIR}/include/gpsdevice
                           ${CMAKE_CURRENT_SOURCE_DIR}/include/interface
                           ${CMAKE_CURRENT_SOURCE_DIR}/include
                           )

target_link_libraries(${PROJECT_NAME} PUBLIC
                      eCAL::core
                      protobuf::libprotobuf
                      simpleini::simpleini
                      gps::gps
                      )

# --------------------------------------------------------
# create package
# --------------------------------------------------------
include(cpack/cpack_variables.cmake)


