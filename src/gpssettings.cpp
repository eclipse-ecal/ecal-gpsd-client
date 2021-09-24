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

#include "gpssettings.h"
#include <SimpleIni.h>
#include "gpshelper.h"
#include <cstdlib>
#include <unistd.h>

using namespace settings_utility;
using namespace error_message;

//------------------------------------------------------------------------------
// gpssettings::gpssettings()
//------------------------------------------------------------------------------
gpssettings::gpssettings(const string &path)
{
    // read values from settings and initialize the gps settings data structure
    _path = path;
    set_gps_settings();
}

//------------------------------------------------------------------------------
// gpssettings::get_device_type()
//------------------------------------------------------------------------------
string gpssettings::get_settings_path() const
{
    auto path = getcwd(nullptr,0);
    string full_path = path;
    if(full_path.empty())
    {
        auto HOME = getenv(_home.c_str()) ? getenv(_home.c_str()) : ".";
        full_path = HOME;
        cout<< _settings_message << endl;
        return full_path + _path_to_config + _file_name;
    }

    return  full_path + _file_name;
}

//------------------------------------------------------------------------------
// gpssettings::set_gps_settings()
//------------------------------------------------------------------------------
void gpssettings::set_gps_settings()
{
    CSimpleIni config(false, false, true);
    SI_Error config_load_error = config.LoadFile(_path.c_str());
    if (config_load_error == SI_FILE)
    {
        // get path from current working directory
        config_load_error = config.LoadFile(get_settings_path().c_str());
        if (config_load_error == SI_FILE)
            cout<< _config_error << endl;
    }

    // read values from settings.ini file
    _settings.gps2ecal_full                          = config.GetBoolValue(_general.c_str(),  _gps_proto_1.c_str(),           true);
    _settings.gps2ecal_data                          = config.GetBoolValue(_general.c_str(),  _gps_proto_2.c_str(),           true);
    _settings.gps2ecal_full_name                     = config.GetValue    (_general.c_str(),  _gps_full_name.c_str(),         "Gpsd2eCalFull");
    _settings.gps2ecal_data                          = config.GetValue    (_general.c_str(),  _gpsd_data_name.c_str(),        "Gpsd2eCalData" );
    _settings.sent_out_frequency                     = config.GetLongValue(_general.c_str(),  _frequency.c_str(),             10);
    _settings.gps_source                             = config.GetLongValue(_general.c_str(),  _gps_source.c_str(),            1);
    _settings.port                                   = config.GetValue    (_gpsd.c_str()   ,  _port.c_str(),                  "2947");
    _settings.adress                                 = config.GetValue    (_gpsd.c_str()   ,  _address.c_str(),               "127.0.0.1");
}

//------------------------------------------------------------------------------
// gpssettings::get_gps_settings()
//------------------------------------------------------------------------------
gps_settings gpssettings::get_gps_settings() const
{
    return _settings;
}

//------------------------------------------------------------------------------
// gpssettings::get_device_type()
//------------------------------------------------------------------------------
gpsdevicetype gpssettings::get_device_type() const
{
    // currently we support only gpsd
    // to be extended with other sources
    return gpsdevicetype::gpsd;
}

//------------------------------------------------------------------------------
// gpssettings::get_proto_type()
//------------------------------------------------------------------------------
map<gpsproto, string> gpssettings::get_proto_type() const
{
    // currently we support only these 2 types
    // to be extended with other proto types
    map<gpsproto, string> proto;
    if(_settings.gps2ecal_full)
        proto.insert(std::pair<gpsproto,string>(gpsproto::gps2ecalfull,_settings.gps2ecal_full_name));

    if(_settings.gps2ecal_data)
        proto.insert(std::pair<gpsproto,string>(gpsproto::gps2ecaldata, _settings.gps2ecal_data_name));

    return proto;
}
