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

#include <iostream>
#include "gpshandler.h"
#include "gpshelper.h"
#include "stringutils.h"
using namespace settings_utility;
using namespace std;
using namespace meniu;
using namespace utils;

//------------------------------------------------------------------------------
// isHelp()
//------------------------------------------------------------------------------
bool isHelp(const string &param)
{
    // Display meniu
    if (!(param == "--h" || param == "--help"))
    {
        return false;
    }

    cout<< _parameters + _help + _ini << endl;
    return true;
}

//------------------------------------------------------------------------------
// isIni()
//------------------------------------------------------------------------------
bool isIni(const string &param, string &path)
{
    // if it's not ini then return
    if (!(StringUtils::startsWith(param, "--i") || StringUtils::startsWith(param, "--ini")))
    {
        path = "";
        return false;
    }

    // check if Gps2eCAL.ini file was included in path
    int index = path.find(_file_name);
    if(index < 0)
    {
        cout<< _ini_not_found << endl;
        path = "";
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
// get_user_input()
//------------------------------------------------------------------------------
bool get_user_input(int argc, char **argv, string &path)
{
    switch (argc)
    {
        case 1:
            // no parameter
            return true;

        case 2:
        {
            string param = argv[1];
            if(isHelp(param)) return false;

            // not correct command
            cout<< _unrecognized_parameter + param << endl;
            return false;
        }
        case 3:
        {
            string param = argv[1];
            path = argv[2];
            if(isIni(param, path )) return true;

            // not correct command
            cout<< _unrecognized_parameter + param << endl;
            return false;
        }

        default:
        {
            cout<< _unrecognized_parameter + argv[1] << endl;
            return false;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
// main()
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    string path = "";
    if(!get_user_input(argc, argv, path))
    {
        return 0;
    }
    gpshandler handle_gps(argc, argv, path);
    handle_gps.run();
    return 0;
}
