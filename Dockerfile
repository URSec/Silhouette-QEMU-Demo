# syntax=docker/dockerfile:1

# Set up an Ubuntu image
FROM ubuntu

# Install dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    binutils-arm-none-eabi \
    scons \
    python3 \
    qemu-system-arm \
    gdb-multiarch \
    screen \
    vim-tiny

# Copy the already-built compiler and libraries, source files of programs, and
# our demo script to the image
WORKDIR /Silhouette-QEMU-Demo
COPY build/llvm/install build/llvm/install
COPY build/newlib-baseline/install build/newlib-baseline/install
COPY build/newlib-silhouette/install build/newlib-silhouette/install
COPY build/compiler-rt-baseline/install build/compiler-rt-baseline/install
COPY build/compiler-rt-silhouette/install build/compiler-rt-silhouette/install
COPY projects projects
COPY demo.py demo.py

# Copy textual files to the image
COPY README.md README.md
COPY LICENSE.txt LICENSE.txt

# Start with bash
CMD /usr/bin/env bash
