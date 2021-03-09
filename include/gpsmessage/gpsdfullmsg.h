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

#ifndef gpsdfullmsg_H
#define gpsdfullmsg_H
#include <iostream>
#include "gps.h"
#include "gpsmsg.h"
#include <ecal/msg/protobuf/publisher.h>
#include <gps2ecal.pb.h>

using namespace std;

class gpsdfullmsg: public gpsmsg
{
public:

    // Rule of 5 default
    gpsdfullmsg() = default;
    gpsdfullmsg(const gpsdfullmsg&) = default;
    gpsdfullmsg(gpsdfullmsg&&) = default;
    gpsdfullmsg& operator=(const gpsdfullmsg&) = default;
    gpsdfullmsg& operator=(gpsdfullmsg&&) = default;
    virtual ~gpsdfullmsg() =default;

    /**
     * @brief message constructor
    **/
    gpsdfullmsg(Idevice *device, const string &name);

    /**
     * @brief Populate protobuf message with gps data
    **/
    void set_gps_message() override;

    /**
     * @brief Send serialized message.
    **/
    void send_gps_message() override;

private:

    void set_gps_data();
    void set_gps_fix_data();
    void set_gps_gst();
    void set_gps_rtcm2();
    void set_gps_rtcm3_t();
    void set_gps_subframe();
    void set_gps_navdata();
    void set_gps_attitude();
    void set_gps_devconfig();
    void set_gps_dop();

    // create a publisher
    eCAL::protobuf::CPublisher<pb::gps::GPSDFull> _pub_gpsdfull;
    pb::gps::GPSDFull                             _msg_gpsdfull;
    Idevice*                                      _gpsd_handler;
    struct gps_data_t                             _gps_data;

};

#endif // gpsdfullmsg_H
