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

#ifndef  gpsdreducedmsg_H
#define  gpsdreducedmsg_H
#include <iostream>
#include "gps.h"
#include "gpsmsg.h"
#include "Idevice.h"
#include <ecal/msg/protobuf/publisher.h>
#include <gps2ecal.pb.h>


using namespace std;
class  gpsdreducedmsg : public gpsmsg
{
public:

    // Rule of 5 default
    gpsdreducedmsg() = default;
    gpsdreducedmsg(const  gpsdreducedmsg&) = default;
    gpsdreducedmsg( gpsdreducedmsg&&) = default;
    gpsdreducedmsg& operator=(const  gpsdreducedmsg&) = default;
    gpsdreducedmsg& operator=( gpsdreducedmsg&&) = default;
    virtual ~ gpsdreducedmsg() = default;

    /**
     * @brief message constructor
    **/
     gpsdreducedmsg(Idevice *device, const string &name);

    /**
     * @brief Populate protobuf message with gps data
    **/
    void set_gps_message() override;

    /**
     * @brief Send serialized message.
    **/
    void send_gps_message() override;

private:

    void set_gps_time();
    void set_gps_dop();
    void set_gps_fix();
    bool is_data_available();

    Idevice*                                 _gpsd_handler;
    struct gps_data_t                        _gps_data;

    // create a publisher
    pb::gps::GPSDReduced                              _msg_gpsdreduced;
    eCAL::protobuf::CPublisher<pb::gps::GPSDReduced>  _pub_gpsdreduced;
    int _msgCnt;

    double  _latitude;
    double  _longitude;
    double  _altitude;

};

#endif //  gpsdreducedmsg_H
