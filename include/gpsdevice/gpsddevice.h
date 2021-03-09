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

#ifndef gpsddevice_H
#define gpsddevice_H
#include <iostream>
#include "gps.h"
#include "Idevice.h"
#include "gpssettings.h"

using namespace std;

class gpsddevice : public Idevice
{
public:

    // Rule of 5 default
    gpsddevice() = default;
    gpsddevice(const gpsddevice&) = default;
    gpsddevice(gpsddevice&&) = default;
    gpsddevice& operator=(const gpsddevice&) = default;
    gpsddevice& operator=(gpsddevice&&) = default;
    virtual ~gpsddevice() = default;

    /**
     * @brief Initializes a GPS-data structure to hold the data collected by the GPS
     *
     * @return  True if successful.
    **/
    bool open_gps() override;

    /**
     * @brief  Ends the session
     *
     * @return  True if successful.
    **/
    bool close_gps() override;

    /**
     * @brief read for data from the daemon
     *
     * @return  True if successful.
    **/
    bool read_gps_data() override;

    /**
     * @brief Can be used to check whether there is new data from the daemon.
     *
     * @return  True if successful.
    **/
    bool gps_wait() override;

    /**
     * @brief Initialize gps with values from settings
     *
     * @param settings: gps settings
    **/
    void gps_init(const gps_settings &settings) override;

    /**
     * @brief Display a message
     *
     * @param message: the message to display
     * @param error_level: the error level of the message
    **/
    void display_message(const string &message, const string &error_level = " ") override;

    /**
     * @brief Returns GPS-data structure collected by the GPS
     *
     * @return gps data structure
    **/
    gps_data_t get_data()const;

private:

    struct gps_data_t    _gps_data;
    struct gps_settings  _settings;
    bool                 _valid_session;
};

#endif // gpsddevice_H
