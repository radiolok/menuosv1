#!/usr/bin/env bash
# Install TI MSP430-GCC (msp430-elf-gcc) and its device support files.
#
#   tools/ci/install-msp430-gcc.sh [PREFIX]
#
# Uses the plain toolchain tarball and the support-files zip that TI publishes
# next to the GUI installer, so nothing interactive runs. Afterwards:
#   PREFIX/bin/msp430-elf-gcc
#   PREFIX/include/msp430g2553.h, msp430g2553.ld, ...
#
# Versions can be overridden with MSP430_GCC_RELEASE, MSP430_GCC_VERSION and
# MSP430_SUPPORT_VERSION.
set -euo pipefail

# In GitHub Actions report problems as annotations, so they show up on the run page.
fail(){
	if [ -n "${GITHUB_ACTIONS:-}" ]; then echo "::error title=install-msp430-gcc::$*"; fi
	echo "$*" >&2
	exit 1
}
trap 'fail "line $LINENO: \"$BASH_COMMAND\" exited with $?"' ERR

PREFIX=${1:-/opt/msp430-gcc}
RELEASE=${MSP430_GCC_RELEASE:-9.3.1.2}          # TI download page version
GCC=${MSP430_GCC_VERSION:-9.3.1.11}             # toolchain inside that release
SUPPORT=${MSP430_SUPPORT_VERSION:-1.212}        # device headers + linker scripts
BASE="https://dr-download.ti.com/software-development/ide-configuration-compiler-or-debugger/MD-LlCjWuAbzH/${RELEASE}"

command -v unzip >/dev/null || fail "unzip not found"

work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT

fetch(){
	echo "Downloading $BASE/$1"
	curl -fsSL --retry 3 -o "$work/$1" "$BASE/$1" || fail "download failed: $BASE/$1"
}

fetch "msp430-gcc-${GCC}_linux64.tar.bz2"
fetch "msp430-gcc-support-files-${SUPPORT}.zip"

mkdir -p "$work/gcc" "$work/support"
tar -xjf "$work/msp430-gcc-${GCC}_linux64.tar.bz2" -C "$work/gcc"
unzip -q "$work/msp430-gcc-support-files-${SUPPORT}.zip" -d "$work/support"

gcc=$(find "$work/gcc" -type f -name msp430-elf-gcc -path '*/bin/*' | head -1)
[ -n "$gcc" ] || fail "msp430-elf-gcc not in the tarball: $(ls "$work/gcc" | tr '\n' ' ')"
ld=$(find "$work/support" -type f -name msp430g2553.ld | head -1)
[ -n "$ld" ] || fail "msp430g2553.ld not in the support files: $(ls "$work/support" | tr '\n' ' ')"

rm -rf "$PREFIX"
mkdir -p "$(dirname "$PREFIX")"
mv "$(dirname "$(dirname "$gcc")")" "$PREFIX"
mkdir -p "$PREFIX/include"
cp -a "$(dirname "$ld")"/. "$PREFIX/include/"

"$PREFIX/bin/msp430-elf-gcc" --version | head -1
