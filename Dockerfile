FROM ubuntu:22.04

RUN apt update 

ENV DEBIAN_FRONTEND noninteractive

# Dev tools
RUN apt -y install git \
    && apt -y install openssh-client \
    && git config --global core.autocrlf true \
    && git config --global --add safe.directory /home/opencv

# Build tools
RUN apt -y install \
    build-essential cmake ninja-build unzip gdb

# OpenCV
RUN apt -y install libopencv-dev

WORKDIR "/home/opencv/build"

RUN useradd -ms /bin/bash dev
USER dev