#!/usr/bin/env bash

[ -d result ] && mv "result" "result-$(date +%s%3N)"

mkdir -p result
echo '*' > result/.gitignore

cd result
../../can-logger-monitor/read-can.sh /dev/ttyUSB0 115200 | python3 ../group.py
