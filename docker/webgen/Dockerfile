FROM biogears-external

# may need to change to specific version of doxygen
RUN apt update && apt -y install ghostscript doxygen graphviz perl default-jdk libgtest-dev
#libhunspell libicu55 libmpfr4 libpng12

# install MiKTeX
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys D6BC243565B2087BC3F897C9277A7293F59E4889
RUN echo "deb http://miktex.org/download/ubuntu bionic universe" | tee /etc/apt/sources.list.d/miktex.list
RUN apt update && apt -y install miktex

#RUN git clone https://github.com/BioGearsEngine/core.git && mkdir core/build

WORKDIR /usr/src/gtest
RUN cmake CMakeLists.txt
RUN make install

#WORKDIR core/build
#RUN cmake -DCMAKE_INSTALL_PREFIX=/opt/biogears/usr -DARA_Biogears_BUILD_JAVATOOLS=ON -DARA_Biogears_BUILD_DOCUMENTATION=ON -DARA_Biogears_BUILD_HOWTOS=ON -DCMAKE_BUILD_TYPE=Release .. && make -j4
#RUN cmake -DCMAKE_INSTALL_PREFIX=/opt/biogears/usr -DBiogears_BUILD_JAVATOOLS=ON -DBiogears_BUILD_DOCUMENTATION=ON -DBiogears_BUILD_HOWTOS=ON -DCMAKE_BUILD_TYPE=Release .. && make -j4
#WORKDIR runtime
#RUN ../outputs/bin/cmd_bio GENSTATES
#RUN ../outputs/bin/cmd_bio VALIDATE all
#RUN ../outputs/bin/cmd_bio "GENTABLES"

