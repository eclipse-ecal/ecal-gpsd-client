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

#include "gpshandler.h"
#include "msgfactory.h"
#include "devicefactory.h"
#include "gpssettings.h"
#include "gpshelper.h"

using namespace utility;
using namespace error_message;

//------------------------------------------------------------------------------
// gpshandler::gpshandler()
//------------------------------------------------------------------------------
gpshandler::gpshandler(int argc, char **argv, const string &path)
{
    // initialize eCAL API
    eCAL::Initialize(argc, argv, _ecal_unit.c_str());

    // set process state
    eCAL::Process::SetState(proc_sev_healthy, proc_sev_level1, _ecal_message.c_str());

    // create map with device and message
    if(make_map(path))
    {
        show_message(_handler_message);
    }
}

//------------------------------------------------------------------------------
// gpshandler::~gpshandler()
//------------------------------------------------------------------------------
gpshandler::~gpshandler()
{
    if(!_gps_msg.empty())
    {
        // take care of gps map
        Idevice *device = _gps_msg.begin()->first;
        delete device;

        set<gpsmsg *> msg = _gps_msg.begin()->second;
        for(auto val : msg)
        {
            gpsmsg *m = val;
            delete m;
        }
        msg.clear();
        _gps_msg.clear();
    }

    // finalize eCAL API
    eCAL::Finalize();
}

//------------------------------------------------------------------------------
// gpshandler::run()
//------------------------------------------------------------------------------
void gpshandler::run()const
{
    auto it = _gps_msg.begin();

    // open session
    if(nullptr == (*it).first || !(*it).first->open_gps())
    {
        return;
    }

    show_message(_send_ecal_message);
    while(eCAL::Ok())
    {
        //wait to receive data from gps
        if (!(*it).first->gps_wait())
        {
          continue;
        }

        // publish to ecal
        for(auto gps : (*it).second)
        {
            if(nullptr == gps) continue;
            gps->send_gps_message();
        }
    }

    // close session
    (*it).first->close_gps();
}

//------------------------------------------------------------------------------
// gpshandler::create_msg()
//------------------------------------------------------------------------------
set<gpsmsg*> gpshandler::create_msg(Idevice *device, const map<gpsproto, string> &proto_type) const
{
    set<gpsmsg*> msg_type;
    for(auto it = proto_type.begin(); it != proto_type.end(); it++)
    {
        msg_type.insert(move(msgfactory::create(device, (*it).first, (*it).second)));
    }
    return msg_type;
}

//------------------------------------------------------------------------------
// gpshandler::make_map()
//------------------------------------------------------------------------------
bool gpshandler::make_map(const string &path)
{
    gpssettings settings(path);
    auto device = devicefactory::create(settings.get_device_type());
    if(nullptr == device)
    {
        show_message(_device_message);
        return false;
    }

    device->gps_init(settings.get_gps_settings());
    auto gpsdmsg = create_msg(device, settings.get_proto_type());
    if(gpsdmsg.empty())
    {
        show_message(_proto_type_message);
        // free memory
        delete device;
        return false;
    }

    _gps_msg.insert(pair<Idevice*, set<gpsmsg*>>(move(device), move(gpsdmsg)));

    return true;
}

//------------------------------------------------------------------------------
// gpshandler::show_message()
//------------------------------------------------------------------------------
void gpshandler::show_message(const string &message) const
{
    cout<< message << endl;
}

