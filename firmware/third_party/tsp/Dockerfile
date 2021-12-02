FROM ubuntu:20.04

# Avoid user interaction while installing packages
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update --yes --quiet && \
    apt-get install --yes --quiet --no-install-recommends \
    ca-certificates \
    cmake \
    clang \
    clang-tidy-10 \
    doxygen \
    g++ \
    gcovr \
    git \
    graphviz \
    libboost-system-dev \
    libboost-test-dev \
    libboost-timer-dev \
    ninja-build \
    plantuml

RUN groupadd --system --gid 2000 jenkins
RUN useradd --system --uid 2000 --gid 2000 --create-home --comment "Jenkins agent user" jenkins
USER jenkins
