#!/usr/bin/env bash

# Find first available serial device and run tio serial monitor

PORT=$(ls /dev/ttyACM* /dev/ttyUSB* /dev/cu.usbmodem* /dev/cu.usbserial* 2> /dev/null | head -n 1)

if [[ -n "$PORT" ]]; then
    echo "Detected MCU on $PORT..."
    if grep -qi "wsl2" /proc/sys/kernel/osrelease 2> /dev/null; then
        stty -F "$PORT" 115200 hup -clocal # Configure speed and enable terminal hangup/DTR assertion
    fi
    tio "$PORT"
else
    echo "Error: No serial device found! Ensure the board is plugged in."
    exit 1
fi
