#!/bin/bash
set -e
SRC="${1:?usage: apply_overlay.sh <kernel common dir>}"
OV="$(cd "$(dirname "$0")" && pwd)"
test -f "$SRC/drivers/misc/Makefile"
cp "$OV/kdev.c" "$OV/kdev_ioctl.h" "$SRC/drivers/misc/"
grep -q 'kdev\.o' "$SRC/drivers/misc/Makefile" || printf '\nobj-y += kdev.o\n' >> "$SRC/drivers/misc/Makefile"
tail -3 "$SRC/drivers/misc/Makefile"
ls -l "$SRC/drivers/misc/kdev.c" "$SRC/drivers/misc/kdev_ioctl.h"
