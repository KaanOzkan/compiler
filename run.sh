#!/bin/bash
# build and run from the cli

cmake -Bbuild -H.
cmake --build build --target all
./build/compiler
