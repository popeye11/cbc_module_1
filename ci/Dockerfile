FROM ubuntu:20.04

ENV LANG=C

# Avoid user interaction while installing packages
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update --yes --quiet && \
    apt-get install --yes --quiet --no-install-recommends \
      build-essential \
      cmake \
      doxygen \
      git \
      graphviz \
      ninja-build \
      plantuml \
      wget

RUN useradd --home-dir /jenkins --create-home --shell /bin/bash --uid 2000 jenkins

RUN wget --quiet --output-document=- \
      'https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2' | \
    tar --directory=/opt --extract --bzip2 --file=-

ENV PATH=/opt/gcc-arm-none-eabi-9-2020-q2-update/bin:$PATH
