FROM ubuntu:latest

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        gdb \
        git \
        pkg-config \
        ninja-build \
        libncurses-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*


    WORKDIR /workspace

    CMD ["/bin/bash"]