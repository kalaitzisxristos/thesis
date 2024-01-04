#!/usr/bin/env bash

mkdir -p result
echo '*' > result/.gitignore

cd result
../../can-logger-monitor/read-can.sh /dev/ttyUSB0 115200 | tee ../can-log.txt | python3 ../group.py
