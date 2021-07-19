#!/bin/sh
echo Dont forget:
echo cmake -DCMAKE_BUILD_TYPE=Debug ..
gdb-multiarch -ix gdb-init.cmd
