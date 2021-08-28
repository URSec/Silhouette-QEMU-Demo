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
import shutil
import subprocess
import sys


#
# Path to the root directory of the repository.
#
root = os.path.abspath(os.path.dirname(sys.argv[0]))

#
# Path to the build directory where we place the compiler and libraries.
#
build_dir = root + '/build'

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
        'forward',
        'backward',
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
    if not os.path.exists(build_dir + '/llvm/install/bin/clang'):
        print('The Silhouette compiler cannot be found')
        print('Run \'./build/build.llvm.sh\' to build it')
        exit(1)
    if not os.path.exists(build_dir + '/newlib-' + config + '/install/arm-none-eabi/lib/libc.a'):
        print('Newlib for ' + config + ' cannot be found')
        print('Run \'./build/build.newlib.sh\' to build it')
        exit(1)
    if not os.path.exists(build_dir + '/compiler-rt-' + config + '/install/lib/baremetal/libclang_rt.builtins-arm.a'):
        print('Compiler-rt for ' + config + ' cannot be found')
        print('Run \'./build/build.compiler.rt.sh\' to build it')
        exit(1)

    # Make sure SCons is installed
    if not shutil.which('scons'):
        print('SCons cannot be found')
        exit(1)

    targets = []
    for p in programs:
        targets += ['build-' + config + '/' + config + '-' + p + '.elf']
    if len(targets) == 0:
        targets += ['build-' + config]

    process = subprocess.Popen(['scons'] + targets, cwd='projects/' + benchmark,
                               stdout=sys.stdout, stderr=subprocess.PIPE,
                               text=True)
    if process.wait() != 0:
        print('****************************************************************')
        print('Build failure')
        print('Error output:')
        print(process.stderr.read())
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
        targets += ['build-' + config + '/' + config + '-' + p + '.elf']

    # Make sure the programs to run have been built
    for t in targets:
        elf = 'projects/' + benchmark + '/' + t
        if not os.path.exists(elf):
            print(elf + ' cannot be found')
            print('Build it by \'' + ' '.join([sys.argv[0], 'build', config, benchmark]) + '\'')
            exit(1)

    qemu_args = [
        'qemu-system-arm', '-M', 'lm3s6965evb', '-serial', 'mon:stdio',
        '-append', 'console=ttyS0', '-nographic', '-kernel',
    ]
    print('Quit QEMU by Ctrl-A + C and then \'quit\'')
    for t in targets:
        print('================================================================')
        print('Running ' + t)
        print()
        process = subprocess.Popen(qemu_args + [t], cwd='projects/' + benchmark,
                                   stdin=sys.stdin, stdout=sys.stdout,
                                   stderr=subprocess.PIPE, text=True)
        if process.wait() != 0:
            print('****************************************************************')
            print('Run failure')
            print('Error output:')
            print(process.stderr.read())
            exit(1)


#
# The main function.
#
def main():
    # Construct a CLI argument parser
    parser = argparse.ArgumentParser(description='Silhouette QEMU Demo')
    parser.add_argument('action', choices=['build', 'run'], metavar='ACTION',
                        help='Action to do (build, run)')
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
    else:
        run(config, benchmark, program)

    return


if __name__ == '__main__':
    main()
