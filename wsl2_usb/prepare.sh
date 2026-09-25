#!/usr/bin/env bash
set -e

# Prepares WSL2 for USB device passthrough via usbipd-win after PC boot

echo "Initializing USBIPD drivers for WSL2..."
sudo modprobe vhci-hcd # To fix usbipd error: WSL kernel is not USBIP capable
echo "WSL2 is now ready for 'usbipd attach'!"
