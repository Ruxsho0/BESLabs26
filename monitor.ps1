# Find COM port specifically matching Stellaris / TI ICDI LaunchPad
$port = (Get-CimInstance Win32_PnPEntity |
    Where-Object { 
        $_.Name -match '\(COM\d+\)' -and (
            $_.DeviceID -like "*VID_1CBE*" -or 
            $_.Name -like "*Stellaris*" -or 
            $_.Manufacturer -like "*Texas Instruments*"
        )
    } |
    Select-Object -ExpandProperty Name |
    ForEach-Object { [regex]::Match($_, 'COM\d+').Value } |
    Select-Object -First 1)

if ($port) {
    Write-Host "Detected MCU on $port..."
    plink -serial $port -sercfg 115200,8,n,1,N
} else {
    Write-Host "Error: Stellaris ICDI Serial Port not found! Ensure the board is plugged in and drivers are installed."
    exit 1
}
