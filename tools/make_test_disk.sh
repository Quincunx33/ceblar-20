#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
IMAGE=${1:-"$ROOT/build/test-fat32.img"}
mkdir -p "$(dirname -- "$IMAGE")"
truncate -s 64M "$IMAGE"
mkfs.fat -F 32 -n CEBLARTEST "$IMAGE" >/dev/null
TMP=$(mktemp)
trap 'rm -f "$TMP"' EXIT
printf 'ceblar-20 FAT32 storage smoke test\n' > "$TMP"
mcopy -i "$IMAGE" "$TMP" ::HELLO.TXT
printf 'FAT32 image: %s\n' "$IMAGE"
