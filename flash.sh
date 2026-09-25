#!/usr/bin/env bash
set -e

# Flashes firmware onto device board using TM4C123 configuration file.
# Works on Linux and macOS.
# Guarantees flash succeeded without corruption, reboots microcontroller 
# so code executes immediately and closes OpenOCD process.

FIRMWARE="lab.axf"
BOARD_CFG="board/ti_ek-tm4c123gxl.cfg"

if [[ ! -f "$FIRMWARE" ]]; then
    echo "Error: Firmware file '$FIRMWARE' not found!"
    echo "Please build your project first."
    exit 1
fi

echo "Flashing ${FIRMWARE} to TM4C123 Launchpad..."
if openocd -d0 -f ${BOARD_CFG} -c "program ${FIRMWARE} verify reset exit" &> /dev/null; then
    echo "Flashing and verification successful!"
else
    echo "Error: Flashing failed! Check if the TM4C123 board is plugged in and ready."
    exit 1
fi
