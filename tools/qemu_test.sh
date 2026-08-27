#!/usr/bin/env bash
set -u
cd "$(dirname "$0")/.."
run_qemu() {
  local image="$1" serial="$2" log="$3"
  : > "$serial"
  : > "$log"
  if [ -n "$image" ]; then
    timeout 8s qemu-system-i386 -boot d -cdrom build/ceblar-20.iso -drive "file=$image,format=raw,if=ide" -display none -serial "file:$serial" -monitor none -no-reboot >"$log" 2>&1
  else
    timeout 8s qemu-system-i386 -cdrom build/ceblar-20.iso -display none -serial "file:$serial" -monitor none -no-reboot >"$log" 2>&1
  fi
  local rc=$?
  cat "$serial"
  cat "$log"
  if [ "$rc" -ne 124 ]; then
    echo "QEMU exited unexpectedly: $rc" >&2
    exit 1
  fi
}
assert_serial() {
  local serial="$1"
  grep -q 'vmm: clone self-test passed' "$serial"
  grep -q 'module: example loaded' "$serial"
  grep -q 'userspace: launching ring3 init' "$serial"
  grep -q 'user: hello' "$serial"
  grep -q 'worker pid=1' "$serial"
  grep -q 'worker pid=2' "$serial"
  grep -q 'worker: sleep cycle complete' "$serial"
}
run_qemu "" /tmp/ceblar-20-qemu-serial.log /tmp/ceblar-20-qemu-test.log
assert_serial /tmp/ceblar-20-qemu-serial.log
if command -v mkfs.fat >/dev/null 2>&1 && command -v mcopy >/dev/null 2>&1; then
  tools/make_test_disk.sh build/test-fat32.img >/dev/null
  run_qemu build/test-fat32.img /tmp/ceblar-20-qemu-disk-serial.log /tmp/ceblar-20-qemu-disk-test.log
  grep -q 'ata: primary master ready' /tmp/ceblar-20-qemu-disk-serial.log
  grep -q 'fat32: mounted' /tmp/ceblar-20-qemu-disk-serial.log
  grep -q 'fat32: HELLO.TXT read ok' /tmp/ceblar-20-qemu-disk-serial.log
  grep -q 'user: hello' /tmp/ceblar-20-qemu-disk-serial.log
fi
