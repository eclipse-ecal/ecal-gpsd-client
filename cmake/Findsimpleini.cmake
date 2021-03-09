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


find_path(simpleini_INCLUDE_DIR
          NAMES SimpleIni.h
          HINTS include ${CMAKE_SOURCE_DIR}/simpleini)

if(simpleini_INCLUDE_DIR-NOTFOUND)
     message(FATAL_ERROR "Could not find simpleini library")
     set(simpleini_FOUND FALSE)
else()
     set(simpleini_FOUND TRUE)
endif()

if(simpleini_FOUND)
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(simpleini REQUIRED_VARS simpleini_INCLUDE_DIR)

  if(NOT TARGET simpleini::simpleini)
    set(simpleini_INCLUDE_DIRS ${simpleini_INCLUDE_DIR})
    add_library(simpleini::simpleini INTERFACE IMPORTED)
    set_target_properties(simpleini::simpleini PROPERTIES 
                          INTERFACE_INCLUDE_DIRECTORIES ${simpleini_INCLUDE_DIR})
    mark_as_advanced(simpleini_INCLUDE_DIR)
  endif()
endif()

