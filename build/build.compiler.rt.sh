#!/usr/bin/env bash

# Copyright (C) 2020, 2021 University of Rochester
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#
# Path to the project root directory.
#
ROOT_DIR=`dirname $0 | sed 's/$/\/../' | xargs realpath`

#
# Path to the LLVM source directory.
#
LLVM_SRC="$ROOT_DIR/llvm-project"

#
# Path to the LLVM build directory.
#
LLVM_BUILD="$ROOT_DIR/build/llvm"

#
# Path to the LLVM install directory.
#
LLVM_INSTALL="$ROOT_DIR/build/llvm/install"

#
# The target for which to build compiler-rt.
#
TARGET="arm-none-eabi"

#
# Set up a configuration.
#
# $1: name of the configuration.
#
configure() {
    #
    # Path to the newlib install directory.
    #
    NEWLIB_INSTALL="$ROOT_DIR/build/newlib-$1/install"
    export NEWLIB_INSTALL

    #
    # Path to the compiler-rt build directory.
    #
    COMPILER_RT_BUILD="$ROOT_DIR/build/compiler-rt-$1"
    export COMPILER_RT_BUILD

    #
    # Path to the compiler-rt install directory.
    #
    COMPILER_RT_INSTALL="$ROOT_DIR/build/compiler-rt-$1/install"
    export COMPILER_RT_INSTALL

    #
    # CFLAGS to use to build compiler-rt.
    #
    CFLAGS="--target=$TARGET"
    CFLAGS="$CFLAGS -mcpu=cortex-m3"
    CFLAGS="$CFLAGS -mfloat-abi=soft"
    CFLAGS="$CFLAGS -mthumb"
    CFLAGS="$CFLAGS -g"
    CFLAGS="$CFLAGS -Os"
    CFLAGS="$CFLAGS -ffunction-sections"
    CFLAGS="$CFLAGS -fdata-sections"
    CFLAGS="$CFLAGS -ffreestanding"
    CFLAGS="$CFLAGS -fomit-frame-pointer"
    CFLAGS="$CFLAGS -I$NEWLIB_INSTALL/$TARGET/include"
    if [ "$1" = "silhouette" ]; then
        CFLAGS="$CFLAGS -DSILHOUETTE"
        CFLAGS="$CFLAGS -DSILHOUETTE_SS_OFFSET=0x1ffc"
        CFLAGS="$CFLAGS -mllvm -enable-arm-silhouette-str2strt"
        CFLAGS="$CFLAGS -mllvm -enable-arm-silhouette-shadowstack"
        CFLAGS="$CFLAGS -mllvm -arm-silhouette-shadowstack-offset=0x1ffc"
    fi
    export CFLAGS
    export CXXFLAGS="$CFLAGS"

    #
    # ASMFLAGS to use to build compiler-rt.
    #
    ASMFLAGS="--target=$TARGET"
    ASMFLAGS="$ASMFLAGS -mcpu=cortex-m3"
    ASMFLAGS="$ASMFLAGS -mfloat-abi=soft"
    ASMFLAGS="$ASMFLAGS -mthumb"
    ASMFLAGS="$ASMFLAGS -g"
    ASMFLAGS="$ASMFLAGS -Os"
    ASMFLAGS="$ASMFLAGS -ffunction-sections"
    ASMFLAGS="$ASMFLAGS -fdata-sections"
    ASMFLAGS="$ASMFLAGS -ffreestanding"
    ASMFLAGS="$ASMFLAGS -fomit-frame-pointer"
    ASMFLAGS="$ASMFLAGS -I$NEWLIB_INSTALL/$TARGET/include"
    if [ "$1" = "silhouette" ]; then
        ASMFLAGS="$ASMFLAGS -DSILHOUETTE"
        ASMFLAGS="$ASMFLAGS -DSILHOUETTE_SS_OFFSET=0x1ffc"
    fi
    export ASMFLAGS
}

CONFIGURATIONS=(
    "baseline"
    "silhouette"
)

###############################################################################

set -e

for conf in ${CONFIGURATIONS[@]}; do
    configure $conf

    mkdir -p "$COMPILER_RT_BUILD" && cd "$COMPILER_RT_BUILD"

    rm -rf CMakeCache.txt

    cmake -G Ninja                                                         \
          -DCMAKE_BUILD_TYPE=Release                                       \
          -DCOMPILER_RT_INSTALL_PATH="$COMPILER_RT_INSTALL"                \
          -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY                   \
          -DCOMPILER_RT_OS_DIR="baremetal"                                 \
          -DCOMPILER_RT_BUILD_BUILTINS=ON                                  \
          -DCOMPILER_RT_BUILD_SANITIZERS=OFF                               \
          -DCOMPILER_RT_BUILD_XRAY=OFF                                     \
          -DCOMPILER_RT_BUILD_LIBFUZZER=OFF                                \
          -DCOMPILER_RT_BUILD_PROFILE=OFF                                  \
          -DCMAKE_C_COMPILER="$LLVM_INSTALL/bin/clang"                     \
          -DCMAKE_AR="$LLVM_BUILD/bin/llvm-ar"                             \
          -DCMAKE_NM="$LLVM_BUILD/bin/llvm-nm"                             \
          -DCMAKE_RANLIB="$LLVM_BUILD/bin/llvm-ranlib"                     \
          -DCOMPILER_RT_BAREMETAL_BUILD=ON                                 \
          -DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON                             \
          -DLLVM_CONFIG_PATH="$LLVM_BUILD/bin/llvm-config"                 \
          -DCMAKE_C_COMPILER_TARGET="$TARGET"                              \
          -DCMAKE_ASM_COMPILER_TARGET="$TARGET"                            \
          -DCMAKE_C_FLAGS="$CFLAGS"                                        \
          -DCMAKE_ASM_FLAGS="$ASMFLAGS"                                    \
          "$LLVM_SRC/compiler-rt"

    ninja install
done
