# ========================= GPSD2eCAL LICENSE =================================
#
# Copyright (C) 2016 - 2019 Continental Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# ========================= GPSD2eCAL LICENSE =================================
#

find_path(GPS_INCLUDE_DIR
          NAMES gps.h)

find_library(GPS_LIBRARIES
             NAMES gps)

if(GPS_INCLUDE_DIR-NOTFOUND)
     message(FATAL_ERROR "Could not find Gps library")
     set(GPS_FOUND FALSE)
else()
     set(GPS_FOUND TRUE)
endif()

if(GPS_FOUND)
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(GPS REQUIRED_VARS GPS_INCLUDE_DIR)

 if(NOT TARGET gps::gps)
    set(GPS_INCLUDE_DIRS ${GPS_INCLUDE_DIR})

    add_library(gps::gps SHARED IMPORTED)
    set_target_properties(gps::gps PROPERTIES 
                          INTERFACE_INCLUDE_DIRECTORIES ${GPS_INCLUDE_DIR}
                          IMPORTED_LOCATION  ${GPS_LIBRARIES})

    mark_as_advanced(GPS_INCLUDE_DIR)
  endif()
endif()



