FROM biogears-external

RUN apt-get update && apt-get -y install ghostscript graphviz perl openjdk-8-jdk libgtest-dev texlive-bibtex-extra

# install MiKTeX (shouldn't be needed after switching to texlive)
#RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys D6BC243565B2087BC3F897C9277A7293F59E4889
#RUN echo "deb http://miktex.org/download/ubuntu bionic universe" | tee /etc/apt/sources.list.d/miktex.list
#RUN apt update && apt -y install miktex

# the custom bootstrap for doxygen specifically works with doxygen 1.8.10
RUN wget -O "doxygen-1.8.10.linux.bin.tar.gz" "https://sourceforge.net/projects/doxygen/files/rel-1.8.10/doxygen-1.8.10.linux.bin.tar.gz"
RUN tar -zxvf "doxygen-1.8.10.linux.bin.tar.gz"
WORKDIR "doxygen-1.8.10"
RUN ./configure --prefix /usr

# comment out lines that are not needed and cause the installation to fail
RUN sed -i 's/$(INSTTOOL) -m 755 bin\/doxytag    $(INSTALL)\/bin/#&/' Makefile
RUN sed -i 's/cp -r html $(INSTALL)\/doc\/doxygen/#&/' Makefile
RUN sed -i 's/cp -r examples $(INSTALL)\/doc\/doxygen/#&/' Makefile

RUN make install

# install GTest
WORKDIR /usr/src/gtest
RUN cmake CMakeLists.txtm
RUN make
RUN make install
