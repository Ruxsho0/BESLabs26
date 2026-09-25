#Requires -RunAsAdministrator
$ErrorActionPreference = "Stop"

# Installation of necessary tools on Windows 10/11 systems:
# - VS Code and extensions
# - ARM GNU compiler toolchain
# - WinLibs GCC & Make (Host C compiler for PC unit tests with make.exe alias)
# - OpenOCD for flashing and debugging
# - CompileDB for compile_commands.json generation
# - PuTTY for serial terminal monitoring
# - Zadig for WinUSB driver setup

# Ensure script executes in the directory where it lives
if ($PSScriptRoot) { Set-Location $PSScriptRoot }

Write-Host "Running setup with administrator privileges..."

# Suppress the progress bar to make downloads significantly faster
$ProgressPreference = "SilentlyContinue"

Write-Host "Installing OpenOCD, MinGW-w64, Python, VS Code and PuTTY via Winget..."
winget install --id xpack-dev-tools.openocd-xpack -e --accept-source-agreements --accept-package-agreements -s winget
winget install --id BrechtSanders.WinLibs.POSIX.UCRT -e --accept-source-agreements --accept-package-agreements -s winget
winget install --id Python.Python.3.12 -e --accept-source-agreements --accept-package-agreements -s winget
winget install --id Microsoft.VisualStudioCode -e --accept-source-agreements --accept-package-agreements -s winget
winget install --id PuTTY.PuTTY -e --accept-source-agreements --accept-package-agreements -s winget

# Pull fresh PATH straight from registry
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")

$makeCmd = Get-Command mingw32-make.exe -ErrorAction SilentlyContinue
if ($makeCmd) {
    $makeExePath = $makeCmd.Source
    $targetPath = Join-Path (Split-Path $makeExePath) "make.exe"
    if (!(Test-Path $targetPath)) {
        Copy-Item $makeExePath $targetPath
        Write-Host "Successfully created make.exe alias."
    }
}

Write-Host "Installing VS Code extensions..."
code --install-extension marus25.cortex-debug
code --install-extension llvm-vs-code-extensions.vscode-clangd

$ArmVer = "15.3.rel1"
$ArmZip = "arm-gnu-toolchain-$ArmVer-mingw-w64-x86_64-arm-none-eabi.zip"
$ArmZipUrl = "https://gitlab.arm.com/api/v4/projects/tooling%2Fgnu-toolchains-for-arm/packages/generic/gnu-toolchain/$ArmVer/$ArmZip"
$ArmDestDir = "C:\arm-gnu-toolchain"

$ArmBinDir = "$ArmDestDir\bin"

if (-not (Test-Path $ArmBinDir)) {
    Write-Host "Downloading and installing ARM GNU toolchain..."
    Write-Host "Link to zip: $ArmZipUrl"
    New-Item -ItemType Directory -Path $ArmDestDir -Force | Out-Null
    curl.exe -sSL -o $ArmZip $ArmZipUrl
    tar -xf $ArmZip -C $ArmDestDir
    Remove-Item -Path $ArmZip -Force
} else {
    Write-Host "ARM GNU Toolchain already installed in $ArmDestDir. Skipping download."
}

# Add ARM to User PATH without duplicates
$UserPath = [Environment]::GetEnvironmentVariable("Path", "User")
if (($UserPath -split ';') -notcontains $ArmBinDir) {
    Write-Host "Adding $ArmBinDir to User PATH..."
    [Environment]::SetEnvironmentVariable("Path", "$ArmBinDir;$UserPath", "User")
} else {
    Write-Host "$ArmBinDir User PATH entry already exists. Skipping."
}

# Export so compieldb would not fail silently
$env:Path = "$ArmBinDir;$env:Path"

$VenvDir = "$ArmDestDir\.venv"
$VenvScripts = "$VenvDir\Scripts"

if (-not (Test-Path "$VenvScripts\compiledb.exe")) {
    Write-Host "Creating isolated Python environment for compiledb in $VenvDir..."
    python -m venv $VenvDir

    & "$VenvScripts\python.exe" -m pip install --upgrade pip
    & "$VenvScripts\python.exe" -m pip install compiledb
} else {
    Write-Host "compiledb virtual environment already exists."
}

$env:Path = "$VenvScripts;$env:Path"

# Move back up once to root repository
cd ..

$JSONOut = Join-Path (Get-Location) "compile_commands.json"
if (Test-Path $JSONOut) { Remove-Item $JSONOut -Force }

Write-Host "Generating clangd compile_commands.json for all lab directories..."

Get-ChildItem -Path . -Filter "Makefile" -Recurse -Depth 3 | ForEach-Object {
    $dir = $_.DirectoryName
    Write-Host "Processing $dir..."
    Push-Location $dir
    try { compiledb -n -o $JSONOut make -B } finally { Pop-Location }
}

if ((Test-Path $JSONOut) -and ((Get-Item $JSONOut).Length -gt 0)) {
    Write-Host "compile_commands.json created successfully!"
} else {
    Write-Error "Error: Failed to generate compile_commands.json."
}

$Zadig = "zadig-2.9.exe"
$ZadigUrl = "https://github.com/pbatard/libwdi/releases/download/v1.5.1/$Zadig"

Write-Host "Downloading Zadig for USB drivers..."
curl.exe -sSL -o $Zadig $ZadigUrl
Write-Host "`n-------------------------------------------------------"
Write-Host "1. PLUG IN your TM4C123 board now."
Write-Host "2. In Zadig: 'Options' -> check 'List All Devices'."
Write-Host "3. Select Stellaris (Interface 0)."
Write-Host "4. Select 'WinUSB' and click 'Replace Driver' (or 'Install Driver')."
Write-Host "5. Repeat steps 1-4 for the Saleae board."
Write-Host "-------------------------------------------------------`n"
Read-Host -Prompt "Press Enter once the board is plugged in to launch Zadig."

Write-Host "Launching Zadig..."
Start-Process -FilePath $Zadig -Wait
if (Test-Path $Zadig) { Remove-Item -Path $Zadig -Force }

Write-Host "Installation complete! Close and reopen your terminal for PATH changes to take effect."
