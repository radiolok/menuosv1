#!/usr/bin/env bash
# Install TI MSP430-GCC (msp430-elf-gcc) into a prefix.
#
#   tools/ci/install-msp430-gcc.sh [PREFIX] [VERSION]
#
# TI ships the Linux toolchain as a .7z archive with a ready-to-use tree
# (bin/, msp430-elf/, include/ with device headers and linker scripts).
# The script unpacks it and moves that tree to PREFIX, so afterwards
#   PREFIX/bin/msp430-elf-gcc
#   PREFIX/include/msp430g2553.h, msp430g2553.ld, ...
# exist no matter how TI named the top-level directory.
set -euo pipefail

PREFIX=${1:-/opt/msp430-gcc}
VERSION=${2:-9.3.1.2}
URL="https://dr-download.ti.com/software-development/ide-configuration-compiler-or-debugger/MD-LlCjWuAbzH/${VERSION}/msp430-gcc-full-linux-x64-installer-${VERSION}.7z"

command -v 7z >/dev/null || { echo "7z not found (apt install p7zip-full)" >&2; exit 1; }

work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT

echo "Downloading $URL"
curl -fsSL --retry 3 -o "$work/gcc.7z" "$URL"
7z x -y -bd -o"$work/x" "$work/gcc.7z" >/dev/null

gcc=$(find "$work/x" -type f -name msp430-elf-gcc -path '*/bin/*' | head -1)
if [ -z "$gcc" ]; then
	# Older packages are a self-extracting .run installer inside the archive.
	run=$(find "$work/x" -type f -name '*.run' | head -1)
	if [ -n "$run" ]; then
		chmod +x "$run"
		"$run" --mode unattended --prefix "$PREFIX"
		gcc="$PREFIX/bin/msp430-elf-gcc"
	else
		echo "msp430-elf-gcc not found in the archive:" >&2
		find "$work/x" -maxdepth 3 >&2
		exit 1
	fi
else
	root=$(dirname "$(dirname "$gcc")")
	mkdir -p "$(dirname "$PREFIX")"
	rm -rf "$PREFIX"
	mv "$root" "$PREFIX"
fi

# Device headers and linker scripts must end up in PREFIX/include,
# that is where the Makefile looks for them (MSP430_GCC_ROOT/include).
if [ ! -f "$PREFIX/include/msp430g2553.ld" ]; then
	ld=$(find "$PREFIX" -name msp430g2553.ld | head -1)
	if [ -z "$ld" ]; then
		echo "device support files (msp430g2553.ld) not found" >&2
		exit 1
	fi
	mkdir -p "$PREFIX/include"
	cp -a "$(dirname "$ld")"/. "$PREFIX/include/"
fi

"$PREFIX/bin/msp430-elf-gcc" --version | head -1
