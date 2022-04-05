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

#include "gpsddevice.h"
#include "gpshelper.h"

using namespace utility;
using namespace settings_utility;
const int miliseconds = 1000;
#include <chrono>
#include <thread>

//------------------------------------------------------------------------------
// gpsddevice::open_gps()
//------------------------------------------------------------------------------
bool gpsddevice::open_gps()
{
    _valid_session = true;
    if( gps_open(_settings.adress.c_str(), _settings.port.c_str(), &_gps_data) == -1)
    {
       display_message(GPS_OPEN_ERROR_MESSAGE, Error);
       _valid_session = false;
       return false;
    }
    gps_stream(&_gps_data, WATCH_ENABLE, NULL);
    return true;
}

//------------------------------------------------------------------------------
// gpsddevice::close_gps()
//------------------------------------------------------------------------------
bool gpsddevice::close_gps()
{
    // session didn't open succesfully
    if(!_valid_session)
        return true;

    gps_stream(&_gps_data, WATCH_DISABLE, NULL);

    // close gps session only if succesfully opened
    if( gps_close(&_gps_data) == -1 )
    {
        display_message(GPS_CLOSE_ERROR_MESSAGE, Error);
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
// gpsddevice::read_gps_data()
//------------------------------------------------------------------------------
bool gpsddevice::read_gps_data()
{
    auto data_read = 0;
#if GPSD_API_MAJOR_VERSION < 9
    data_read = gps_read(&_gps_data);
#else
    char message;
    int message_len = 1;
    data_read = gps_read(&_gps_data, &message, message_len);
#endif

    if( data_read == -1 || !_valid_session)
    {
        display_message(GPS_READ_ERROR_MESSAGE, Error);
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
// gpsddevice::gps_wait()
//------------------------------------------------------------------------------
bool gpsddevice::gps_wait()
{
    if(_settings.sent_out_frequency <= 0) return false;
    auto mili = miliseconds/_settings.sent_out_frequency;
    std::this_thread::sleep_for(std::chrono::milliseconds(mili));
    return true;
}

//------------------------------------------------------------------------------
// gpsddevice::gps_init()
//------------------------------------------------------------------------------
void gpsddevice::gps_init(const gps_settings &settings)
{
    _settings = settings;
}

//------------------------------------------------------------------------------
// gpsddevice::display_message()
//------------------------------------------------------------------------------
void gpsddevice::display_message(const string &message, const string &error_level)
{
    cout<< error_level << message << errno <<" "<< gps_errstr(errno)<<endl;
}

//------------------------------------------------------------------------------
// gpsddevice::get_data()
//------------------------------------------------------------------------------
gps_data_t gpsddevice::get_data() const
{
    return _gps_data;
}
