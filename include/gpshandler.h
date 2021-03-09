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

#ifndef gpshandler_H
#define gpshandler_H
#include <iostream>
#include "gpsutil.h"
#include "gpshelper.h"
#include <ecal/ecal.h>
#include "Idevice.h"
#include "gpsmsg.h"
#include <map>
#include <set>

using namespace std;
class gpshandler
{
public:

    // Rule of 5 default
    gpshandler() = default;
    gpshandler(const gpshandler&) = default;
    gpshandler(gpshandler&&) = default;
    gpshandler& operator=(const gpshandler&) = default;
    gpshandler& operator=(gpshandler&&) = default;

    /**
     * @brief gps handler constructor
    **/
    gpshandler(int argc, char **argv, const string &path);
    virtual ~gpshandler();

    /**
     * @brief Create and send message to eCAL from a gps device
    **/
    void run() const;

private:

    set<gpsmsg*>  create_msg(Idevice *device, const map<gpsproto, string> &proto_type) const;
    void show_message(const string &message) const;    
    bool make_map(const string &path);

    std::map<Idevice*,set<gpsmsg*>>       _gps_msg;
    string                                _user_path;
};

#endif // gpshandler_H
