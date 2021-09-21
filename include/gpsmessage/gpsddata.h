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

#ifndef  gpsddata_H
#define  gpsddata_H
#include <iostream>
#include "gps.h"
#include "gpsmsg.h"
#include "Idevice.h"
#include <ecal/msg/protobuf/publisher.h>
#include <gpsdata.pb.h>


using namespace std;
class  gpsddata : public gpsmsg
{
public:

    // Rule of 5 default
    gpsddata() = default;
    gpsddata(const  gpsddata&) = default;
    gpsddata( gpsddata&&) = default;
    gpsddata& operator=(const  gpsddata&) = default;
    gpsddata& operator=( gpsddata&&) = default;
    virtual ~ gpsddata() = default;

    /**
     * @brief message constructor
    **/
     gpsddata(Idevice *device, const string &name);

    /**
     * @brief Populate protobuf message with gps data
    **/
    void set_gps_message() override;

    /**
     * @brief Send serialized message.
    **/
    void send_gps_message() override;

private:

    void set_gps_dop();
    void set_gps_fix();
    void set_gps_gst();
    void set_gps_data();
    void set_gps_header();
    Idevice*                                 _gpsd_handler;
    struct gps_data_t                        _gps_data;

    // create a publisher
    pb::gps::gpsdata                              _msg_gpsdata;
    eCAL::protobuf::CPublisher<pb::gps::gpsdata>  _pub_gpsdata;

};

#endif //  gpsddata_H
