# C++ Build System Generator

This C++ build system generator is a simple utility that can help you create a Makefile for your C++ projects. It analyzes the source files in the current directory and generates a Makefile with dependencies and build rules. You can use this generated Makefile to build your C++ project with just one command.

## Prerequisites

- C++ Compiler with C++20 support (e.g., g++)

## Usage

1. Compile the program:
   
   ```bash
    make
    make install
   ```

2. Run the program to generate a Makefile:

   - If you provide an `output_file` argument, the generated Makefile will be saved to that file.
   ```bash
    $ mmpp
   ```

   - If you don't provide an `output_file` argument, the Makefile will be printed to the console.

   ```bash
    $ mmpp makefile
   ```


3. Use the generated Makefile to build your C++ project:

   ```bash
   make
   ```

4. Clean the generated object files:

   ```bash
   make clean
   ```

## Features

- Generates Makefile rules for all `.cpp` source files in the current directory.
- Automatically detects source file dependencies and includes them in the Makefile.
- Provides build, run, clean, install, and uninstall targets in the generated Makefile.

## Example

Suppose you have the following C++ source files in your project directory:

- `main.cpp`
- `utils.cpp`
- `utils.hpp`

Running the `mmpp` program will generate a Makefile that includes rules for building your project, such as:

```make
# Generated with github.com/da0x/mmpp
# Binary:
my_program: .obj utils.o main.o
    g++ -std=c++20 .obj/utils.o .obj/main.o -o my_program

.obj:
    mkdir .obj

.obj/main.o: main.cpp utils.hpp
    g++ -std=c++20 -o .obj/main.o -c main.cpp

.obj/utils.o: utils.cpp utils.hpp
    g++ -std=c++20 -o .obj/utils.o -c utils.cpp

run: my_program
    ./my_program

clean:
    rm -rfv .obj

install:
    sudo cp -v my_program /usr/local/bin/

uninstall:
    sudo rm -v /usr/local/bin/my_program
```

