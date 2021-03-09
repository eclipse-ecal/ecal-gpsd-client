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

#ifndef devicefactory_H
#define devicefactory_H
#include <iostream>
#include "Idevice.h"
#include "gpsutil.h"
#include "gpsddevice.h"

class devicefactory
{
public:

    /**
     * @brief Creates a device
     *
     * @param type: type of gps device used
     * @return  device if successful.
    **/
    static Idevice* create(const gpsdevicetype &type);
};

#endif // devicefactory_H
