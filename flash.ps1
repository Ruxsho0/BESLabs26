# Flashes firmware onto device board using TM4C123 configuration file.
# Works on Windows 10/11.
# Guarantees flash succeeded without corruption, reboots microcontroller 
# so code executes immediately and closes OpenOCD process.

$Firmware = "lab.axf"
$BoardCfg = "board\ti_ek-tm4c123gxl.cfg"

if (-not (Test-Path -Path $Firmware)) {
    Write-Host "Error: Firmware file '$Firmware' not found!"
    Write-Host "Please build your project first."
    exit 1
}

Write-Host "Flashing $Firmware to TM4C123 Launchpad..."
openocd -d0 -f $BoardCfg -c "program $Firmware verify reset exit" *> $null

if ($LASTEXITCODE -eq 0) {
    Write-Host "Flashing and verification successful!"
} else {
    Write-Host "Error: Flashing failed! Check if the TM4C123 board is plugged in and ready."
    exit 1
}
