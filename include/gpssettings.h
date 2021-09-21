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

#ifndef SETTINGS_H
#define SETTINGS_H
#include <iostream>
#include "gpsutil.h"
#include <map>
#include <set>
#include <vector>

using namespace std;

struct gps_settings
{
  int  gps_source;
  int  sent_out_frequency;
  bool gps2ecal_full;
  bool gps2ecal_reduced;
  bool gps2ecal_data;
  const char*  gps2ecal_full_name;
  const char*  gps2ecal_reduced_name;
  const char*  gps2ecal_data_name;
  const char*  port;
  const char*  adress;
};

class gpssettings
{
public:

    // Rule of 5 default
    gpssettings(const gpssettings&) = default;
    gpssettings(gpssettings&&) = default;
    gpssettings& operator=(const gpssettings&) = default;
    gpssettings& operator=(gpssettings&&) = default;
    virtual ~gpssettings() = default;

    /**
     * @brief settings constructor
    **/
    gpssettings(const string &path);

    /**
     * @brief Reads data from ini file populates the gps_settings struct with value
     *
     * @return  settings struct.
    **/
    gps_settings get_gps_settings() const;

    /**
     * @brief gets the device type from gps_settings
     *
     * @return  device type.
    **/
    gpsdevicetype get_device_type() const;

    /**
     * @brief gets the settings.ini file path
     *
     * @return  path to settings file.
    **/
    string get_settings_path()const;

    /**
     * @brief get proto type from gps_settings.
     * Adds the gpsproto in the set only if the value in settings is true
     *
     * @return  a map of pair of GpsProto and name .
    **/
    map<gpsproto, string> get_proto_type() const;


private:

    void set_gps_settings();
    gps_settings _settings;
    string _path;
};

#endif // SETTINGS_H
