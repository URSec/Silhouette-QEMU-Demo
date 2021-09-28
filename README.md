# Run Silhouette on QEMU

This repository contains and organizes code for people who want to try out
Silhouette on a QEMU emulator (i.e., without a real ARM development board).
For general information about the Silhouette project, please refer to the
[Silhouette repository](https://github.com/URSec/Silhouette).  If you happen to
have an STM32F469 Discovery board with you, you can also replicate our
evaluation of Silhouette on this board in
[this repository](https://github.com/URSec/Silhouette-Evaluation).

## Using Docker Image

We have set up a Docker image that contains a pre-compiled version of the
Silhouette compiler, binaries of the libraries (i.e., Newlib and compiler-rt),
and source code of programs to be built and run under Silhouette.  The image is
based on a minimal Ubuntu system on x86_64 and is provided so that one can skip
building the Silhouette compiler and the libraries to save time.  If you choose
to run Silhouette using our Docker image, simply run
```shell
docker run -it --rm ursec/silhouette-qemu-demo
```
and a shell in a container will be spawned for you.  You can then skip the
[setup phase](#set-up-the-evaluation-environment) and go directly to the
[build-and-run phase](#build-debug-and-run-programs).  This image also includes
useful tools (such as `arm-none-eabi-objdump`) to examine generated binaries.

## Everything from Scratch

Now we are building everything from scratch.  Before we start, we list here a
few assumptions and dependencies of the environment:

- We assume the host machine is x86 and the host operating system is Linux.
  Combinations of other architectures and operating systems may work but were
  not tested.
- We use CMake, Ninja, and Clang to build the LLVM-based Silhouette compiler,
  so `cmake`, `ninja`, and `clang` of appropriate versions must be found in
  `PATH`.
- We use the Silhouette compiler to build Newlib and compiler-rt, so make sure
  that common development tools needed to build Newlib and compiler-rt (such as
  `make`) are there in `PATH`.  In particular, one of our build scripts uses
  `arm-none-eabi-gcc` to find out where a bare-metal ARM `libgcc` is installed.
- We use SCons to build programs, so make sure that `scons` of an appropriate
  version is there in `PATH`.
- We use QEMU ARM emulator to run ELF binaries built for the Luminary Micro
  Stellaris LM3S6965EVB board, so `qemu-system-arm` of an appropriate version
  must be found in `PATH`.
- We use GDB to debug ELF binaries and have debugging support included in one
  of our scripts.  If you would like to use our script for debugging, make sure
  either `gdb-multiarch` or `arm-none-eabi-gdb` is there in `PATH`.  In
  addition, the script uses GNU Screen to simultaneously run QEMU and GDB, so
  `screen` of an appropriate version must also be found in `PATH`.

Again, our [Docker image](#using-docker-image) meets every assumption and has
every dependency pre-installed.  It is the recommended way to try out
Silhouette on QEMU.

Following is the directory hierarchy of this repository:

```shell
Silhouette-QEMU-Demo
|-- build                    # Directory for building LLVM, Newlib, and compiler-rt
|   |-- build.llvm.sh        # Script to build LLVM
|   |-- build.newlib.sh      # Script to build Newlib
|   |-- build.compiler.rt.sh # Script to build compiler-rt
|
|-- llvm-project             # A submodule of URSec/Silhouette-Compiler containing
|                            # source code of LLVM and Silhouette passes
|
|-- newlib-cygwin            # A submodule of URSec/newlib-cygwin containing source
|                            # code of Newlib
|
|-- projects                 # Directory containing source code of programs
|   |-- beebs                # Source code of BEEBS benchmarks
|   |-- coremark             # Source code of CoreMark benchmark
|   |-- tests                # Source code of test programs
|
|-- demo.py                  # Script to build, debug, and run programs
|
|-- README.md                # This README file
```

### Set up the Evaluation Environment

1. Clone this repository.
   ```shell
   git clone --recurse-submodules https://github.com/URSec/Silhouette-QEMU-Demo
   ```
2. Build the Silhouette compiler.
   ```shell
   cd Silhouette-QEMU-Demo && ./build/build.llvm.sh
   ```
   Note that all our scripts (`demo.py` and those in the `build` directory) are
   CWD-agnostic; each of them can be run from any working directory and would
   have the same outcome.  After `./build/build.llvm.sh` finishes, the
   Silhouette compiler will be installed in `build/llvm/install`.
3. Build Newlib and compiler-rt.
   ```shell
   ./build/build.newlib.sh && ./build/build.compiler.rt.sh
   ```
   Note that the above two scripts will build the libraries using two
   configurations:
   - **Baseline**: Compile without any of our passes, denoted as `baseline`;
   - **Silhouette**: Turn on the shadow stack, store hardening, and CFI passes,
     denoted as `silhouette`.

   After the two scripts finish, Newlib will be installed in
   `build/newlib-baseline/install` and `build/newlib-silhouette/install` and
   compiler-rt will be installed in `build/compiler-rt-baseline/install` and
   `build/compiler-rt-silhouette/install`.

### Build, Debug, and Run Programs

We have a script `demo.py` that can compile, debug, and run two benchmark
suites ([BEEBS](https://beebs.mageec.org) and
[CoreMark](https://www.eembc.org/coremark)) and a few test programs we wrote to
demonstrate Silhouette's protections (explained
[here](#what-do-test-programs-do)).  The script supports the following
command-line argument format
```shell
./demo.py <ACTION> <CONFIG> <BENCH> [PROGRAM [PROGRAM]...]
```
where
- `ACTION` can be `build`, `debug`, or `run`,
- `CONFIG` is the name of a configuration (either `baseline` or `silhouette`,
  see above),
- `BENCH` is the name of the benchmark/program suite (`beebs`, `coremark`, or
  `tests`), and
- `PROGRAM` is the name of a program in the corresponding benchmark/program
  suite.

If no `PROGRAM` is specified, all the programs in the corresponding
benchmark/program suite will be compiled/run.  For example, running
`./demo.py build baseline beebs bs ns` will compile two programs (named `bs`
and `ns`, respectively) in BEEBS using the Baseline configuration (and link
Newlib and compiler-rt of the Baseline configuration as well), and running
`./demo.py run silhouette tests` will run all the test programs we wrote that
were compiled using the Silhouette configuration.

For `build`, the generated binaries, as well as intermediate object files, will
be placed in the `projects/<BENCH>/build-<CONFIG>` directory.

For `run`, the script will spawn a QEMU process for running each of the
specified binaries.  You can quit QEMU at any point by
<kbd>Ctrl</kbd>-<kbd>A</kbd> + <kbd>C</kbd> (which opens QEMU's monitor
console) and then typing `quit`/`q`.

For `debug`, what the script does is simply creating a debugging session of two
side-by-side windows in GNU Screen (again, for each of the specified binaries),
where the window on the left side runs QEMU and the window on the right side
runs GDB.  You can switch between the two windows by
<kbd>Ctrl</kbd>-<kbd>A</kbd> + <kbd>Tab</kbd>.  To open QEMU's monitor console,
now you need to press <kbd>Ctrl</kbd>-<kbd>A</kbd> + <kbd>A</kbd> + <kbd>C</kbd>
when the QEMU window has the input, as both QEMU and GNU Screen use
<kbd>Ctrl</kbd>-<kbd>A</kbd> as their control character.

## What Do Test Programs Do

Our test program suite contains four test programs that characterize different
memory safety and control-flow hijacking attacks: `forward`, `backward`,
`overflow`, and `rop`.  You can
build them using either the baseline or Silhouette configuration and run them
to examine their execution results with and without Silhouette's protections.

`forward` demonstrates how a forward-edge control flow transfer (i.e., an
indirect function call) is hijacked to jump to the middle of a function.  When
built with the baseline configuration, it will jump to code that prints out a
message indicating a successful attack.  When it is built with the Silhouette
configuration, you will find that Silhouette's CFI instrumentation detects a
forward-edge corruption and clears all bits in the corrupted function pointer;
jumping to the address `0` then triggers a `UsageFault` exception, whose
handler will print out a message indicating a failed attack.

`backward` demonstrates how a backward-edge control flow transfer (i.e., a
function return) is hijacked to jump to the beginning of a function that does
not appear in the regular control flow graph.  In the baseline configuration,
it corrupts a return address saved on the stack to point to a function which
prints out a message indicating a successful attack.  The same attack under the
Silhouette configuration will no longer work, and you will see a message
printed out to indicate a failure, which is part of the original benign control
flow.

`overflow` demonstrates that a stack-based buffer overflow vulnerability may
write past the stack region and overwrite other memory regions.  In the
Silhouette configuration, it will try to corrupt the shadow stack region, which
is placed next to the stack region, and it will be captured as the shadow stack
is protected from writes initiated by unprivileged stores.

`rop` simulates a Return-Oriented Programming (ROP) attack, in which an
attacker hijacks the control flow by corrupting a return address on the stack
and executing a chain of reusable code gadgets.  In our program, we fabricated
a buffer overflow vulnerability and crafted an attack payload to print out a
"hello world"-styled message to the serial console.  The payload is hard-coded
and is highly dependent on the generated code layout; therefore it is different
across the baseline and Silhouette configurations.  Similar to that of
`backward`, such a ROP attack under the Silhouette configuration will not work,
and `rop`'s original control flow will be executed.

## How to Extend

Internally, `demo.py` invokes `scons` to build programs and invokes
`qemu-system-arm` to run programs.  Each benchmark/program suite in the
`projects` directory is pre-configured to be built using SCons.  If you want to
add new programs, you can see how existing programs are configured in there.
Specifically:
- `projects/<BENCH>/SConstruct` is the entry point of SCons and defines the
  build environment (such as `CFLAGS` and `LDFLAGS`) of each configuration,
  when building programs in `BENCH`.
- `projects/<BENCH>/src/programs.scons` defines program-specific settings and
  will be loaded by the above `SConstruct` script.
- If you want to debug or run a newly added program via `demo.py`, simply add
  the program name to the `benchmarks` dictionary in the script.

The biggest constraint for supporting larger programs (such as
[CoreMark-Pro](https://www.eembc.org/coremark-pro)) is the limited memory on
the LM3S6965EVB board.  The board only has 256 KB of flash memory and 64 KB of
SRAM.  You can find how we lay out memory regions in the linker scripts,
`LinkerScript-baseline.ld` and `LinkerScript-silhouette.ld`, of each
benchmark/program suite.  In particular, we place the shadow stack above the
regular stack with a constant offset `0x2000` (`0x1ffc` for shadow stack
instrumentations), which limits the stack size to 8 KB.  If your program to add
consumes more memory than available, it will not work as expected.  However,
one could switch to a larger stack (and shadow stack) size as follows:
- Increase the `_StackSize` variable in `LinkerScript-baseline.ld` and
  `LinkerScript-silhouette.ld` to a larger value; depending on the `_StackSize`
  value, the value of the `_HeapSize` variable might have to decrease in order
  to fit in the SRAM.
- Change occurrences of `-DSILHOUETTE_SS_OFFSET=0x1ffc` and
  `-mllvm -arm-silhouette-shadowstack-offset=0x1ffc` in both
  `build/build.newlib.sh` and `build/build.compiler.rt.sh` accordingly, and
  then rebuild Newlib and compiler-rt.
- Change occurrences of `SILHOUETTE_SS_OFFSET=0x1ffc` and
  `-Wl,-mllvm,-arm-silhouette-shadowstack-offset=0x1ffc` in the corresponding
  `SConstruct` script accordingly, and then (re)build the program(s).
