#!/bin/sh
sudo dfu-util -a 0 -i 0 -s 0x08000000:leave -D build/stm32.bin
