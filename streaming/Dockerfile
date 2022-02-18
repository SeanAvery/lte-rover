FROM ubuntu:18.04
ARG BUILD_SRC="/janus"
RUN apt update && apt-get upgrade -y
RUN apt install git-core \
      libjansson-dev \ 
      libconfig-dev \
      libnice-dev \
      libssl-dev \
      libsrtp2-dev -y
