FROM biogears-external
RUN git clone https://github.com/BioGearsEngine/core.git && mkdir core/build
WORKDIR core/build
RUN cmake -DCMAKE_INSTALL_PREFIX=/opt/biogears/usr -DARA_Biogears_BUILD_JAVATOOLS=OFF -DARA_Biogears_BUILD_HOWTOS=ON -DCMAKE_BUILD_TYPE=Release .. && make -j4
WORKDIR runtime

