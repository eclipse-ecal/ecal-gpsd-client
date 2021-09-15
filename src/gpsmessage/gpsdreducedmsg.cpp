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

#include "gpsdreducedmsg.h"
#include "gpshelper.h"
#include <cmath>
#include "ecal/ecal_time.h"

using namespace utility;
using namespace gps_fix_constants;

//------------------------------------------------------------------------------
//  gpsdreducedmsg:: gpsdreducedmsg
//------------------------------------------------------------------------------
 gpsdreducedmsg:: gpsdreducedmsg(Idevice* device, const string &name):gpsmsg()
  , _gpsd_handler(device)
  , _msgCnt      (0)
{
    _pub_gpsdreduced.Create(name);
}

//------------------------------------------------------------------------------
//  gpsdreducedmsg::send_gps_message()
//------------------------------------------------------------------------------
void  gpsdreducedmsg::send_gps_message()
{
    // read the new gps data
    _gps_data = update(_gpsd_handler);

    // set all gps data to proto struct
    set_gps_message();

    // publish message to ecal
    _pub_gpsdreduced.Send(_msg_gpsdreduced);
}

//------------------------------------------------------------------------------
//  gpsdreducedmsg::set_gps_time()
//------------------------------------------------------------------------------
void  gpsdreducedmsg::set_gps_time()
{
#if GPSD_API_MAJOR_VERSION < 9
    time_t time = _gps_data.fix.time;
    auto rawTime = localtime ( &time );
    if(nullptr == rawTime)
    {
        return;
    }

    _msg_gpsdreduced.set_day(rawTime->tm_mday);
    _msg_gpsdreduced.set_month(rawTime->tm_mon +1);

    // tm_year is relative to 1900 as the C library specification says
    _msg_gpsdreduced.set_year(rawTime->tm_year + _relative_year);

    auto secsaftermidnight = rawTime->tm_hour*_hour2sec + rawTime->tm_min*_min2sec + rawTime->tm_sec;
    _msg_gpsdreduced.set_secsaftermidnight(secsaftermidnight);

    // NOT human readable.
    _msg_gpsdreduced.set_timestamp(_gps_data.fix.time);

    // _msg_gpsdreduced.timeUTC          = _gps_data.gst.utctime;
    _msg_gpsdreduced.set_timeutc(secsaftermidnight);
#endif
}

//------------------------------------------------------------------------------
//  gpsdreducedmsg::set_gps_time()
//------------------------------------------------------------------------------
void  gpsdreducedmsg::set_gps_dop()
{
    /* Dilution of precision factors */
    _msg_gpsdreduced.set_hdop(_gps_data.dop.hdop);
    _msg_gpsdreduced.set_pdop(_gps_data.dop.pdop);
    _msg_gpsdreduced.set_vdop(_gps_data.dop.vdop);
}

//------------------------------------------------------------------------------
//  gpsdreducedmsg::set_gps_time()
//------------------------------------------------------------------------------
void  gpsdreducedmsg::set_gps_fix()
{    
    /* Speed over ground, meters/sec */
    _msg_gpsdreduced.set_speed(_gps_data.fix.speed);

    /* Number of satellites used in solution */
    _msg_gpsdreduced.set_satellitesused(_gps_data.satellites_used);

    /* Latitude position uncertainty, meters */
    _msg_gpsdreduced.set_poserr(_gps_data.fix.epy);

    /* Mode of fix */
    _msg_gpsdreduced.set_fixmode(static_cast<pb::gps::GPSDReduced::EFixMode>(_gps_data.fix.mode));

    /* Latitude in degrees (valid if mode >= 2) */
    _msg_gpsdreduced.set_latitude(_gps_data.fix.latitude);

    /* Longitude in degrees (valid if mode >= 2) */
    _msg_gpsdreduced.set_longitude(_gps_data.fix.longitude);

    /* Altitude in meters (valid if mode == 3) */
    _msg_gpsdreduced.set_altitude(_gps_data.fix.altitude);
}

//------------------------------------------------------------------------------
//  gpsdreducedmsg::set_gps_message()
//------------------------------------------------------------------------------
void  gpsdreducedmsg::set_gps_message()
{
    set_gps_time();
    set_gps_dop();
    set_gps_fix();

    /* Types 1-3 Common navigation info */
    // if heading not correct check ==> _gps_data.ais.type1.course
    _msg_gpsdreduced.set_heading(_gps_data.ais.type1.heading);
    _msg_gpsdreduced.set_msgcnt(_msgCnt++);

    // Set the timestamp
    auto header = new pb::Header();
    header->set_timestamp (eCAL::Time::GetMicroSeconds());
    _msg_gpsdreduced.set_allocated_header(move(header));
}

//------------------------------------------------------------------------------
//  gpsdreducedmsg::is_data_available()
//------------------------------------------------------------------------------
bool  gpsdreducedmsg::is_data_available()
{
    // check if no data received
    return !isnan(_gps_data.fix.latitude) && !isnan(_gps_data.fix.longitude);
}

