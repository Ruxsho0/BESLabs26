#!/usr/bin/env bash
set -e

# Installation of necessary tools on
# Ubuntu/Debian-based, Fedora/RHEL-based, Arch-based Linux systems:
# - VS Code extensions
# - ARM GNU compiler toolchain
# - GNU GCC & Make (Host C compiler and glibc headers for PC unit tests)
# - OpenOCD for flashing and debugging
# - CompileDB for compile_commands.json generation
# - tio for serial terminal monitoring

# Ensure script executes in the directory where it lives
cd "$(dirname "$0")"

ARCH=$(uname -m)
if [[ "$ARCH" != "x86_64" && "$ARCH" != "aarch64" ]]; then
    echo "Error: Unsupported Linux host architecture: $ARCH"
    exit 1
fi

echo "Installing OpenOCD, Make, pipx and tio..."
if command -v apt &> /dev/null; then
    echo "Detected Debian/Ubuntu-based system (apt)."
    sudo apt update
    sudo apt install -y openocd build-essential gcc make pipx tio
elif command -v dnf &> /dev/null; then
    echo "Detected Fedora/RHEL-based system (dnf)."
    sudo dnf install -y openocd gcc make pipx tio
elif command -v pacman &> /dev/null; then
    echo "Detected Arch-based system (pacman)."
    sudo pacman -Syu --needed --noconfirm openocd base-devel gcc make python-pipx tio
else
    echo "Error: Unsupported package manager."
    exit 1
fi

if command -v code &> /dev/null; then
    echo "Installing VS Code extensions..."
    code --install-extension marus25.cortex-debug \
         --install-extension llvm-vs-code-extensions.vscode-clangd
else
    echo "Error: VS Code is not installed. Please install it: https://code.visualstudio.com/docs/setup/linux"
    if [[ -n "$WSL_DISTRO_NAME" ]]; then
        echo "WSL detected. Please ensure VS Code is installed on your Windows host with the 'ms-vscode-remote.remote-wsl' extension."
    fi
fi

ARM_VER="15.3.rel1"
PKG_NAME="arm-gnu-toolchain-${ARM_VER}-${ARCH}-arm-none-eabi"
ARM_TAR_LINK="https://gitlab.arm.com/api/v4/projects/tooling%2Fgnu-toolchains-for-arm/packages/generic/gnu-toolchain/${ARM_VER}/${PKG_NAME}.tar.xz"
ARM_DEST_DIR="/opt/arm-gnu-toolchain"

if [[ ! -d "${ARM_DEST_DIR}/bin" ]]; then
    echo "Downloading and installing ARM GNU Toolchain to ${ARM_DEST_DIR}..."
    echo "Link to tarball: ${ARM_TAR_LINK}"
    sudo mkdir -p "${ARM_DEST_DIR}"
    curl -sSL "${ARM_TAR_LINK}" | sudo tar -xJ -C "${ARM_DEST_DIR}" --strip-components=1
else
    echo "ARM GNU Toolchain already installed in ${ARM_DEST_DIR}. Skipping download."
fi

# Export so compieldb would not fail silently
export PATH="${ARM_DEST_DIR}/bin:${PATH}"

if [[ "$SHELL" == *"zsh"* ]]; then
    SHELL_CONFIG="${HOME}/.zshrc"
else
    SHELL_CONFIG="${HOME}/.bashrc"
fi

if grep -q "${ARM_DEST_DIR}/bin" "${SHELL_CONFIG}" 2> /dev/null; then
    echo "ARM Toolchain PATH entry already exists in ${SHELL_CONFIG}. Skipping."
else
    echo "Adding ARM Toolchain to PATH in ${SHELL_CONFIG}..."
    echo "export PATH=\"${ARM_DEST_DIR}/bin:\$PATH\"" >> "${SHELL_CONFIG}"
fi

echo "Installing compiledb via pipx..."
pipx ensurepath
pipx install compiledb

# Export so compiledb can be run from this session
export PATH="${HOME}/.local/bin:${PATH}"

# Move back up once to root repository
cd ..

JSON_OUT="$(pwd)/compile_commands.json"

rm -f "$JSON_OUT"

echo "Generating clangd compile_commands.json for all lab directories..."
find . -maxdepth 3 -name Makefile -exec dirname {} \; | while read -r dir; do
    echo "Processing $dir..."
    (
        cd "$dir" || exit 1
        compiledb -n -o "$JSON_OUT" make -B
    )
done

if [[ -s "$JSON_OUT" ]]; then
    echo "compile_commands.json created successfully!"
else
    echo "Error: Failed to generate compile_commands.json."
fi

echo "Installation complete! Close and reopen your terminal for PATH changes to take effect."
