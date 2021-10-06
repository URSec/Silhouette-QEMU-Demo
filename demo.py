#!/usr/bin/env python3

# Copyright (C) 2021 University of Rochester
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

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time


#
# Path to the root directory of the repository.
#
root = os.path.abspath(os.path.dirname(sys.argv[0]))

#
# Path to the build directory where we place the compiler and libraries.
#
build_dir = os.path.join(root, 'build')

#
# List of configurations.
#
configurations = [
    'baseline',
    'silhouette',
]

#
# Dict of benchmark suites and their benchmark programs.
#
benchmarks = {
    'beebs': [
        'aha-compress',
        'aha-mont64',
        'bs',
        'bubblesort',
        'cnt',
        'compress',
        'cover',
        'crc',
        'crc32',
        'ctl-stack',
        'ctl-string',
        'ctl-vector',
        'cubic',
        'dijkstra',
        'dtoa',
        'duff',
        'edn',
        'expint',
        'fac',
        'fasta',
        'fdct',
        'fibcall',
        'fir',
        'frac',
        'huffbench',
        'insertsort',
        'janne_complex',
        'jfdctint',
        'lcdnum',
        'levenshtein',
        'ludcmp',
        'matmult-float',
        'matmult-int',
        'mergesort',
        'miniz',
        'minver',
        'nbody',
        'ndes',
        'nettle-aes',
        'nettle-arcfour',
        'nettle-cast128',
        'nettle-des',
        'nettle-md5',
        'nettle-sha256',
        'newlib-exp',
        'newlib-log',
        'newlib-mod',
        'newlib-sqrt',
        'ns',
        'nsichneu',
        'picojpeg',
        'prime',
        'qrduino',
        'qsort',
        'qurt',
        'recursion',
        'rijndael',
        'select',
        'sglib-arraybinsearch',
        'sglib-arrayheapsort',
        'sglib-arrayquicksort',
        'sglib-dllist',
        'sglib-hashtable',
        'sglib-listinsertsort',
        'sglib-listsort',
        'sglib-queue',
        'sglib-rbtree',
        'slre',
        'sqrt',
        'st',
        'statemate',
        'stb_perlin',
        'stringsearch1',
        'strstr',
        'tarai',
        'trio-snprintf',
        'trio-sscanf',
        'ud',
        'whetstone',
        'wikisort',
    ],
    'coremark': [
        'coremark',
    ],
    'tests': [
        'backward',
        'forward',
        'overflow',
        'ditto',
    ],
}

###############################################################################

#
# Build one or more programs in a benchmark suite with a specified
# configuration.
#
# @config: the configuration.
# @benchmark: the benchmark suite.
# @programs: a list of programs to build (empty means all).
#
def build(config, benchmark, programs):
    # Make sure the compiler and libraries have been built
    clang_path = os.path.join(build_dir, 'llvm', 'install', 'bin', 'clang')
    if not os.path.exists(clang_path):
        print('The Silhouette compiler cannot be found')
        print('Run \'' + os.path.join(build_dir, 'build.llvm.sh') + '\' to build it')
        exit(1)
    newlib_path = os.path.join(build_dir, 'newlib-' + config, 'install',
                               'arm-none-eabi', 'lib', 'libc.a')
    if not os.path.exists(newlib_path):
        print('Newlib for ' + config + ' cannot be found')
        print('Run \'' + os.path.join(build_dir, 'build.newlib.sh') + '\' to build it')
        exit(1)
    crt_path = os.path.join(build_dir, 'compiler-rt-' + config, 'install',
                            'lib', 'baremetal', 'libclang_rt.builtins-arm.a')
    if not os.path.exists(crt_path):
        print('Compiler-rt for ' + config + ' cannot be found')
        print('Run \'' + os.path.join(build_dir, 'build.compiler.rt.sh') + '\' to build it')
        exit(1)

    # Make sure SCons is installed
    if not shutil.which('scons'):
        print('SCons cannot be found')
        exit(1)

    targets = []
    for p in programs:
        targets += [os.path.join('build-' + config, config + '-' + p + '.elf')]
    if len(targets) == 0:
        targets += ['build-' + config]

    process = subprocess.Popen(['scons'] + targets,
                               cwd=os.path.join(root, 'projects', benchmark),
                               start_new_session=True)
    if process.wait() != 0:
        exit(1)


#
# Run one or more programs in a benchmark suite under a specified
# configuration, using QEMU.
#
# @config: the configuration.
# @benchmark: the benchmark suite.
# @programs: a list of programs to run (empty means all).
#
def run(config, benchmark, programs):
    targets = []
    if len(programs) == 0:
        programs = benchmarks[benchmark]
    for p in programs:
        targets += [os.path.join('build-' + config, config + '-' + p + '.elf')]

    # Make sure QEMU and Screen are installed
    if not shutil.which('qemu-system-arm'):
        print('QEMU cannot be found')
        exit(1)
    if not shutil.which('screen'):
        print('Screen cannot be found')
        exit(1)

    # Make sure the programs to run have been built
    for t in targets:
        elf = os.path.join(root, 'projects', benchmark, t)
        if not os.path.exists(elf):
            print(elf + ' cannot be found')
            print('Build it by \'' + ' '.join([sys.argv[0], 'build', config, benchmark]) + '\'')
            exit(1)

    # Run QEMU via Screen
    qemu_args = [
        'qemu-system-arm', '-M', 'lm3s6965evb', '-serial', 'pty',
        '-nographic', '-kernel',
    ]
    for t in targets:
        print('================================================================')
        print('Running ' + t)
        print()

        with tempfile.TemporaryDirectory() as d:
            # Create a temporary .screenrc file
            screenrc = os.path.join(d, 'screenrc')
            screenlog = os.path.join(d, 'screenlog')
            with open(screenrc, mode='at') as f:
                f.writelines('logfile flush 0\n')
                f.writelines('split\n')
                f.writelines('focus\n')
                f.writelines(' '.join(['screen'] + qemu_args + [t]) + '\n')
                f.writelines('focus\n')

            # Spawn the Screen process
            process = subprocess.Popen(['screen', '-S', 'silhouette-qemu-demo',
                                        '-L', '-Logfile', screenlog,
                                        '-c', screenrc],
                                       cwd=os.path.join(root, 'projects', benchmark),
                                       start_new_session=True)
            time.sleep(1)

            # Find out which PTY was created by QEMU
            pattern = 'char device redirected to (.*) \(label .*\)'
            pts = None
            with open(screenlog) as f:
                for line in f:
                    match = re.search(pattern, line)
                    if match:
                        pts = match.group(1)
            assert pts is not None

            # Tell the Screen process to monitor the PTY
            process2 = subprocess.Popen(['screen', '-S', 'silhouette-qemu-demo',
                                         '-X', 'screen', pts],
                                        cwd=os.path.join(root, 'projects', benchmark),
                                        start_new_session=True)
            if process2.wait() != 0:
                exit(1)
            if process.wait() != 0:
                exit(1)


#
# Debug one or more programs in a benchmark suite under a specified
# configuration, using QEMU and GDB.
#
# @config: the configuration.
# @benchmark: the benchmark suite.
# @programs: a list of programs to debug (empty means all).
#
def debug(config, benchmark, programs):
    targets = []
    if len(programs) == 0:
        programs = benchmarks[benchmark]
    for p in programs:
        targets += [os.path.join('build-' + config, config + '-' + p + '.elf')]

    # Make sure QEMU, GDB, and Screen are installed
    if not shutil.which('qemu-system-arm'):
        print('QEMU cannot be found')
        exit(1)
    gdb = 'gdb-multiarch'
    if not shutil.which(gdb):
        gdb = 'arm-none-eabi-gdb'
        if not shutil.which(gdb):
            print('GDB cannot be found')
            exit(1)
    if not shutil.which('screen'):
        print('Screen cannot be found')
        exit(1)

    # Make sure the programs to debug have been built
    for t in targets:
        elf = os.path.join(root, 'projects', benchmark, t)
        if not os.path.exists(elf):
            print(elf + ' cannot be found')
            print('Build it by \'' + ' '.join([sys.argv[0], 'build', config, benchmark]) + '\'')
            exit(1)

    # Run QEMU and GDB via screen
    qemu_args = [
        'qemu-system-arm', '-M', 'lm3s6965evb', '-serial', 'pty',
        '-nographic', '-S', '-s', '-kernel',
    ]
    gdb_args = [
        gdb, '-ex', '"target remote :1234"',
    ]
    for t in targets:
        print('================================================================')
        print('Debugging ' + t)
        print()

        with tempfile.TemporaryDirectory() as d:
            # Create a temporary .screenrc file
            screenrc = os.path.join(d, 'screenrc')
            screenlog = os.path.join(d, 'screenlog')
            with open(screenrc, mode='at') as f:
                f.writelines('logfile flush 0\n')
                f.writelines('split -v\n')
                f.writelines('split\n')
                f.writelines('focus\n')
                f.writelines(' '.join(['screen'] + qemu_args + [t]) + '\n')
                f.writelines('focus\n')
                f.writelines(' '.join(['screen'] + gdb_args + [t]) + '\n')
                f.writelines('focus\n')

            # Spawn the Screen process
            process = subprocess.Popen(['screen', '-S', 'silhouette-qemu-demo',
                                        '-L', '-Logfile', screenlog,
                                        '-c', screenrc],
                                       cwd=os.path.join(root, 'projects', benchmark),
                                       start_new_session=True)
            time.sleep(1)

            # Find out which PTY was created by QEMU
            pattern = 'char device redirected to (.*) \(label .*\)'
            pts = None
            with open(screenlog) as f:
                for line in f:
                    match = re.search(pattern, line)
                    if match:
                        pts = match.group(1)
            assert pts is not None

            # Tell the Screen process to monitor the PTY
            process2 = subprocess.Popen(['screen', '-S', 'silhouette-qemu-demo',
                                         '-X', 'screen', pts],
                                        cwd=os.path.join(root, 'projects', benchmark),
                                        start_new_session=True)
            if process2.wait() != 0:
                exit(1)
            if process.wait() != 0:
                exit(1)


#
# The main function.
#
def main():
    # Construct a CLI argument parser
    parser = argparse.ArgumentParser(description='Silhouette QEMU Demo')
    parser.add_argument('action', choices=['build', 'run', 'debug'], metavar='ACTION',
                        help='Action to do (build, run, debug)')
    parser.add_argument('configuration', choices=configurations, metavar='CONFIG',
                        help='Name of the configuration (' + ', '.join(configurations) + ')')
    parser.add_argument('benchmark', choices=benchmarks.keys(), metavar='BENCH',
                        help='Name of the benchmark suite (' + ', '.join(benchmarks.keys()) + ')')
    parser.add_argument('program', metavar='PROGRAM', nargs='*',
                        help='Name of the program(s) in the benchmark suite')

    # Parse CLI arguments
    args = parser.parse_args()
    action = args.action
    config = args.configuration
    benchmark = args.benchmark
    program = list(set(args.program))
    if len(program) > 0:
        for p in program:
            if p not in benchmarks[benchmark]:
                parser.error('argument PROGRAM must be chosen from:\n' +
                             ' '.join(benchmarks[benchmark]))

    # Build or run
    if action == 'build':
        build(config, benchmark, program)
    elif action == 'debug':
        debug(config, benchmark, program)
    else:
        run(config, benchmark, program)

    return


if __name__ == '__main__':
    main()
