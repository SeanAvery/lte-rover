FROM ubuntu:18.04
ARG BUILD_SRC="/janus"
RUN apt update && apt-get upgrade -y
RUN apt install git-core \
	libjansson-dev \ 
	libconfig-dev \
	libnice-dev \
      	libssl-dev \
      	# libsrtp2-dev \ 
      	autogen \
      	autoconf \
      	libtool \
      	gengetopt \
      	wget -y

# build libsrtp
RUN wget https://github.com/cisco/libsrtp/archive/v2.3.0.tar.gz && \
	tar xfv v2.3.0.tar.gz && \
	cd libsrtp-2.3.0 && \
	./configure --prefix=/usr --enable-openssl && \
	make shared_library && \
	make install

# build janus-gateway
RUN git clone https://github.com/meetecho/janus-gateway.git ${BUILD_SRC}
RUN cd ${BUILD_SRC} \
	&& chmod +x autogen.sh \
      	&& ./autogen.sh \
      	&& ./configure --prefix=/janus \
      	--enable-http --disable-websockets --disable-data-channels --disable-rabbitmq --disable-mqt --disable-all-handlers \
      	&& make \
      	&& make install

# run janus
Add rtp-live.jcfg /rtp-live.jcfg

