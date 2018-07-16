FROM ubuntu:18.04
RUN apt update && apt -y install git wget ninja-build libncurses5-dev build-essential g++ automake libeigen3-dev liblog4cpp5-dev libxerces-c-dev
WORKDIR /root/src
RUN wget https://cmake.org/files/v3.12/cmake-3.12.0-rc3.tar.gz && tar -xf cmake-3.12.0-rc3.tar.gz && wget https://www.codesynthesis.com/download/xsd/4.0/linux-gnu/x86_64/xsd_4.0.0-1_amd64.deb && dpkg -i xsd_4.0.0-1_amd64.deb
WORKDIR /root/src/cmake-3.12.0-rc3
RUN ./bootstrap --parallel=4 --prefix=/usr/local && make -j4 && make install
WORKDIR /opt/biogears