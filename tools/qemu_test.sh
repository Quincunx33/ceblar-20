#!/usr/bin/env bash
set -u
cd "$(dirname "$0")/.."
log=/tmp/ceblar-20-qemu-test.log
serial=/tmp/ceblar-20-qemu-serial.log
: > "$log"
: > "$serial"
timeout 8s qemu-system-i386 -cdrom build/ceblar-20.iso -display none -serial "file:$serial" -monitor none -no-reboot >"$log" 2>&1
rc=$?
cat "$serial"
cat "$log"
if [ "$rc" -ne 124 ]; then
  echo "QEMU exited unexpectedly: $rc" >&2
  exit 1
fi
grep -Eq '(pmm|vmm|module|subsystem|timer|syscall)' "$serial"
