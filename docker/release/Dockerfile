FROM timhoer/biogears-external:latest
WORKDIR /opt
RUN git clone https://github.com/BioGearsEngine/core.git && mkdir core/build
WORKDIR /opt/core/build
RUN cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/linux-gcc5.4-core2_64 -DCMAKE_PREFIX_PATH='/opt/xsd-4.0.0-x86_64-linux-gnu;/opt/eigen-eigen-5a0156e40feb' -DCodeSynthesis_INCLUDE_DIR=/opt/xsd-4.0.0-x86_64-linux-gnu/libxsd -DCodeSynthesis_EXECUTABLE=/opt/xsd-4.0.0-x86_64-linux-gnu/bin/xsd -DXerces-c_USE_STATIC_LIBRARY=OFF -DCMAKE_BUILD_TYPE=Release .. && make -j4 && make install
RUN cmake --build . --config Release --target gen-states

