#!/usr/bin/env zsh
set -e

# Installation of necessary tools on macOS systems:
# - Apple Command Line Tools (Host C compiler and SDK headers for PC unit tests)
# - ARM GNU compiler toolchain
# - GNU Make for build scripting (configured in PATH over legacy Apple Make)
# - OpenOCD for flashing and debugging
# - CompileDB for compile_commands.json generation
# - tio for serial terminal monitoring

# Ensure script executes in the directory where it lives
cd "$(dirname "$0")"

ARCH=$(uname -m)
if [[ "$ARCH" != "arm64" && "$ARCH" != "x86_64" ]]; then
    echo "Error: Unsupported Mac host architecture: $ARCH"
    exit 1
fi

# Ensure macOS Command Line Tools (Host C Compiler / SDK headers) are installed
xcode-select -p > /dev/null 2>&1 || xcode-select --install

if command -v brew &> /dev/null; then
    echo "Installing OpenOCD, Make, compiledb and tio via Homebrew..."
    brew install open-ocd make compiledb tio
else 
    echo "Error: Homebrew is required on macOS. Please install it: https://brew.sh/"
    exit 1
fi

GNU_MAKE_PATH="$(brew --prefix)/opt/make/libexec/gnubin"
export PATH="${GNU_MAKE_PATH}:${PATH}"

if [[ "$ARCH" == "arm64" ]]; then
    ARM_VER="15.3.rel1"
    PKG_NAME="arm-gnu-toolchain-${ARM_VER}-darwin-${ARCH}-arm-none-eabi"
else
    # x86_64 Intel Macs use 14.2.rel1 (last release supporting x86_64 host)
    ARM_VER="14.2.rel1"
    PKG_NAME="arm-gnu-toolchain-${ARM_VER}-darwin-x86_64-arm-none-eabi"
fi

ARM_TAR_LINK="https://gitlab.arm.com/api/v4/projects/tooling%2Fgnu-toolchains-for-arm/packages/generic/gnu-toolchain/${ARM_VER}/${PKG_NAME}.tar.xz"
ARM_DEST_DIR="/opt/arm-gnu-toolchain"

if [[ ! -d "${ARM_DEST_DIR}/bin" ]]; then
    echo "Downloading and installing ARM GNU Toolchain to ${ARM_DEST_DIR}..."
    echo "Link to tarball: ${ARM_TAR_LINK}"
    sudo mkdir -p "${ARM_DEST_DIR}"
    curl -sSL "${ARM_TAR_LINK}" | sudo tar -xJ -C "${ARM_DEST_DIR}" --strip-components=1

    # Remove macOS Gatekeeper quarantine attributes to allow binary execution
    sudo xattr -r -d com.apple.quarantine "${ARM_DEST_DIR}" 2> /dev/null
else
    echo "ARM GNU Toolchain already installed in ${ARM_DEST_DIR}. Skipping download."
fi

# Export so compiledb would not fail silently
export PATH="${ARM_DEST_DIR}/bin:${PATH}"

SHELL_CONFIG="${HOME}/.zshrc"

if grep -q "${ARM_DEST_DIR}/bin" "${SHELL_CONFIG}" 2> /dev/null; then
    echo "ARM Toolchain PATH entry already exists in ${SHELL_CONFIG}. Skipping."
else
    echo "Adding ARM Toolchain to PATH in ${SHELL_CONFIG}..."
    echo "export PATH=\"${ARM_DEST_DIR}/bin:\$PATH\"" >> "${SHELL_CONFIG}"
fi

if grep -q "${GNU_MAKE_PATH}" "${SHELL_CONFIG}" 2> /dev/null; then
    echo "GNU Make PATH entry already exists in ${SHELL_CONFIG}. Skipping."
else
    echo "Adding GNU Make to PATH in ${SHELL_CONFIG}..."
    echo "export PATH=\"${GNU_MAKE_PATH}:\$PATH\"" >> "${SHELL_CONFIG}"
fi

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
