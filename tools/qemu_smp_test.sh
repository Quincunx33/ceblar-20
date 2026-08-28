#!/bin/sh
set -u
cd "$(dirname "$0")/.."
for cpus in 2 4; do
  serial="/tmp/ceblar-20-qemu-smp-${cpus}-serial.log"
  log="/tmp/ceblar-20-qemu-smp-${cpus}.log"
  : > "$serial"
  : > "$log"
  timeout 8s qemu-system-i386 -smp "$cpus" -cdrom build/ceblar-20.iso -display none -serial "file:$serial" -monitor none -no-reboot >"$log" 2>&1
  rc=$?
  cat "$serial"
  cat "$log"
  if [ "$rc" -ne 124 ]; then
    echo "QEMU SMP baseline exited unexpectedly for ${cpus} CPUs: $rc" >&2
    exit 1
  fi
  grep -q 'smp: BSP online, CPUID APIC' "$serial"
  grep -q 'user: hello' "$serial"
  grep -q 'futex: timeout wake self-test passed' "$serial"
done
