/* ========================= GPSD2eCAL LICENSE =================================
 *
 * Copyright (C) 2016 - 2019 Continental Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ========================= GPSD2eCAL LICENSE =================================
*/

#ifndef gpsmsg_H
#define gpsmsg_H
#include <iostream>
#include "gps.h"
#include "Idevice.h"


using namespace std;
class gpsmsg
{
public:

    // Rule of 5 default
    gpsmsg() = default;
    gpsmsg(const gpsmsg&) = default;
    gpsmsg(gpsmsg&&) = default;
    gpsmsg& operator=(const gpsmsg&) = default;
    gpsmsg& operator=(gpsmsg&&) = default;
    virtual ~gpsmsg() = default;

    /**
     * @brief Populate protobuf message with gps data
    **/
    virtual void set_gps_message() =0;

    /**
     * @brief Send serialized message.
    **/
    virtual void send_gps_message() =0;

    /**
     * @brief Read and Update gps data
     *
     * @return  structure with gps data.
    **/
    gps_data_t update(Idevice *device);

    /**
     * @brief Checks if the input value is valid
     * @param value: string to be checked
     * @return  true if valid, false otherwise
    **/
    bool isUtf8(const string& value);
};

#endif // gpsmsg_H
