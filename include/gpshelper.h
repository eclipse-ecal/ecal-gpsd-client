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

#ifndef GPS_HELPER_H
#define GPS_HELPER_H
#include <iostream>
using namespace std;

namespace error_message
{
    const string Error                   = "ERROR! ";
    const string GPS_OPEN_ERROR_MESSAGE  = "Please check port/address defined in gps2ecal.ini! GPS has failed to open with code: " ;
    const string GPS_READ_ERROR_MESSAGE  = "GPS has failed to read gps data with code: " ;
    const string GPS_CLOSE_ERROR_MESSAGE = "GPS has failed to end session with code: ";
    const string GPS_MESSAGE             = "Waiting for GPS message... ";
    const string _ecal_message           = "I feel good!";
    const string _settings_message       = "Path to settings.ini not found! Please add it in the build/ini folder. Using default path to settings.ini";
    const string _handler_message        = "Message created! Preparing to send it to eCAL!";
    const string _send_ecal_message      = "Everything OK! Send message to eCAL!";
    const string _proto_type_message     = "Could not create message! Please check the defined eCAL publishers in gps2ecal.ini";
    const string _device_message         = "Could not create device! Please check the defined eCAL publishers in gps2ecal.ini";
    const string _unrecognized_parameter = "Unrecognized Parameter: ";
}

namespace utility
{
    const string _local_host     = "localhost";
    const string _default_port   = "2947";
    const string _ecal_unit      = "GPS publisher";
    const string _gps2ecal_pub   = "Gps2eCal";
    const string _gpsrecpos_pub  = "GpsRecPos";
    const string _gpsd_undefined = "?";
}

namespace settings_utility
{
    const string _path_to_config   = "Documents/work/GPSd2eCAL/GPSd2eCAL/ini";
    const string _file_name        = "/gps2ecal.ini";
    const string _config_error     = "ERROR! Loading gps2ecal.ini failed with code: -3 File error. \n gps2ecal.ini not found!Use a different path or add gps2ecal.ini in the build/ini folder. \n gps2ecal will use default values. \n";
    const string _general          = "general";
    const string _gpsd             = "gpsd";
    const string _gps_proto_1      = "gps2ecal_full";
    const string _gps_proto_2      = "gps2ecal_data";
    const string _gps_full_name    = "gps2ecal_full_name";
    const string _gpsd_data_name   = "gps2ecal_data_name";
    const string _frequency        = "sent_out_frequency";
    const string _gps_source       = "gps_source";
    const string _port             = "port";
    const string _address          = "address";
    const string _protocol_type    = "protocol_type";
    const string _home             = "HOME";
}

namespace meniu
{
    const string _parameters      = "Parameters: \n";
    const string _help            = "--h            --help           Display this help \n";
    const string _ini             = "--i PATH       --ini PATH       Use the path to an ini file to load the settings. \n";
    const string _ini_not_found   = "The path to /gps2ecal.ini is missing or is not complete. Please make sure the path contains the file name";
}

namespace gps_fix_constants
{
    const int _relative_year     = 1900;
    const int _hour2sec          = 3600;
    const int _min2sec           = 60;
}

#endif // GPS_HELPER_H
