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

#!/bin/bash
#
echo ---------------------------------------------------------------------------
echo -                             GPSd2eCAL setenv.sh
echo ---------------------------------------------------------------------------
export CHECKOUT_TARGET_DIR=./../
export WORKSPACE=$PWD/../
export BUILD_DIR=./../_build/
export BUILD_SCRIPTS=build_scripts

#create build folder if not exits
if [ ! -d $BUILD_DIR ]; then
	mkdir $BUILD_DIR
fi

