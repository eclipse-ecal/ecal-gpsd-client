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

#include "msgfactory.h"
#include "gpsdfullmsg.h"
#include "gpsdreducedmsg.h"
#include "gpsddata.h"
#include "gpsmsg.h"

//------------------------------------------------------------------------------
// msgfactory::create()
//------------------------------------------------------------------------------
gpsmsg *msgfactory::create(Idevice *device, const gpsproto &type, const string &name)
{
    switch (type)
    {
        case gpsproto::gps2ecalfull:
            return new gpsdfullmsg(device, name);

        case gpsproto::gps2ecalreduced:
            return new gpsdreducedmsg(device, name);

        case gpsproto::gps2ecaldata:
            return new gpsddata(device, name);
    }
    return nullptr;
}
