#!/usr/bin/env bash
set -u
cd "$(dirname "$0")/.."
serial=/tmp/ceblar-20-qemu-smp-serial.log
log=/tmp/ceblar-20-qemu-smp.log
: > "$serial"
: > "$log"
timeout 8s qemu-system-i386 -smp 2 -cdrom build/ceblar-20.iso -display none -serial "file:$serial" -monitor none -no-reboot >"$log" 2>&1
rc=$?
cat "$serial"
cat "$log"
if [ "$rc" -ne 124 ]; then
  echo "QEMU SMP baseline exited unexpectedly: $rc" >&2
  exit 1
fi
grep -q 'smp: BSP online, CPUID APIC' "$serial"
grep -q 'user: hello' "$serial"
