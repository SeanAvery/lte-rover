FROM ubuntu:18.04

ARG LEVEL="devel"

# janus installation
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
      	wget \ 
        curl \
        libmicrohttpd-dev -y

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

# dev installs

# RUN apt update && \
	# apt install ffmpeg \
	# vim  -y

# run janus
Add rtp-live.jcfg /rtp-live.jcfg
ADD clean.sh /clean.sh
run chmod +x /clean.sh
Add run.sh /run.sh
run chmod +x /run.sh

# CMD /clean.sh && /run.sh

