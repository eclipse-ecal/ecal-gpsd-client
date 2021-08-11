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

#include "gpsmsg.h"
#include "gpsddevice.h"

//------------------------------------------------------------------------------
// gpsmsg::update
//------------------------------------------------------------------------------
gps_data_t gpsmsg::update(Idevice *device)
{
    gps_data_t gps_data;
    auto gpsdh = dynamic_cast<gpsddevice*> (device);
    if(nullptr == gpsdh)
    {
        return gps_data;
    }

    gpsdh->read_gps_data();
    gps_data = gpsdh->get_data();

    return gps_data;
}
//------------------------------------------------------------------------------
// gpsmsg::isUtf8
//------------------------------------------------------------------------------
bool gpsmsg::isUtf8(const string &value)
{
    int current;
    int n;
    auto length = value.length();
    for (auto idx=0; (unsigned)idx < length; idx++)
    {
        current = (unsigned char) value[idx];
        if (0x00 <= current && current <= 0x7f)
            n=0; // 0bbbbbbb

        else if ((current & 0xE0) == 0xC0)
            n=1; // 110bbbbb

        else if ( current==0xed && (unsigned)idx<(length-1) && ((unsigned char)value[idx+1] & 0xa0)==0xa0)
            return false; //U+d800 to U+dfff

        else if ((current & 0xF0) == 0xE0)
            n=2; // 1110bbbb

        else if ((current & 0xF8) == 0xF0)
            n=3; // 11110bbb

        else
            return false;

        for (auto j=0; j<n && (unsigned)idx<length; j++)
        {
            if (((unsigned)++idx == length) || (( current & 0xC0) != 0x80))
                return false;
        }
    }
    return true;
}
