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

#ifndef IDEVICE_H
#define IDEVICE_H
#include <iostream>
#include "gpshelper.h"
#include "gpssettings.h"

using namespace std;
using namespace error_message;
using namespace utility;

class Idevice
{
public:

    virtual ~Idevice() = default;

    /**
     * @brief Initializes a GPS-data structure to hold the data collected by the GPS
     *
     * @return  True if successful.
    **/
    virtual bool open_gps() =0;

    /**
     * @brief  Ends the session
     *
     * @return  True if successful.
    **/
    virtual bool close_gps() =0;

    /**
     * @brief read for data from the daemon
     *
     * @return  True if successful.
    **/
    virtual bool read_gps_data() =0;

    /**
     * @brief Can be used to check whether there is new data from the daemon.
     *
     * @return  True if successful.
    **/
    virtual bool gps_wait() =0;

    /**
     * @brief Initialize gps with values from settings
     *
     * @param settings: gps settings
    **/
    virtual void gps_init(const gps_settings &settings) =0;

    /**
     * @brief Display a message
     *
     * @param message: the message to display
     * @param error_level: the error level of the message
     *
    **/
    virtual void display_message(const string &message, const string &error_level = " ") =0;
};

#endif // IDEVICE_H
