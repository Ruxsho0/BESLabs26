#!/usr/bin/env bash

# Attaches device to WSL2 using usbipd-win

TARGET_VID="1cbe:00fd"

DEV_LINE=$(usbipd.exe list | tr -d '\r' | awk '/Connected:/{flag=1; next} /Persisted:/{flag=0} flag' | grep -i "$TARGET_VID" | head -n 1)
if [[ -n "$DEV_LINE" ]]; then
    echo "Found target device ($TARGET_VID). Attaching to WSL..."
    usbipd.exe attach -w -i "$TARGET_VID"
else
    echo "Error: Target board not detected. Ensure it is plugged in."
    exit 1
fi
