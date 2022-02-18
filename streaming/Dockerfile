FROM ubuntu:18.04
ARG BUILD_SRC="/janus"
RUN apt update && apt-get upgrade -y
RUN apt install git-core \
      libjansson-dev \ 
      libconfig-dev \
      libnice-dev \
      libssl-dev \
      libsrtp2-dev \ 
      autogen \
      autoconf \
      libtool \
      gengetopt -y
# make janus-gateway
RUN git clone https://github.com/meetecho/janus-gateway.git ${BUILD_SRC}
RUN cd ${BUILD_SRC} \
      && chmod +x autogen.sh \
      && ./autogen.sh \
      && ./configure --prefix=/janus \
      --disable-websockets --disable-data-channels --disable-rabbitmq --disable-mqt --disable-all-handlers \
      && make \
      && make install

