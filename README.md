# gpsd2ecal
__Install the dependencies for Ubuntu 18.04:__

- Install eCAL and follow the setup steps https://continental.github.io/ecal/getting_started/setup.html#fa-ubuntu-installing-ecal-on-ubuntu
- sudo apt-get update
- sudo apt-get install cmake libprotobuf-dev protobuf-compiler libprotoc-dev libprotoc10  
- sudo apt-get install gpsd-clients gpsd libgps-dev


__How to build:__
- run make_all.sh from the build_scripts folder
