#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
tools/make_test_disk.sh build/test-virtio.img >/dev/null
: > /tmp/ceblar-20-virtio-serial.log
: > /tmp/ceblar-20-virtio-test.log
set +e
timeout 8s qemu-system-i386 -boot d -cdrom build/ceblar-20.iso -drive id=vd0,file=build/test-virtio.img,format=raw,if=none -device virtio-blk-pci,drive=vd0,disable-modern=on -display none -serial file:/tmp/ceblar-20-virtio-serial.log -monitor none -no-reboot >/tmp/ceblar-20-virtio-test.log 2>&1
rc=$?
set -e
if [ "$rc" -ne 124 ]; then
  echo "QEMU exited unexpectedly: $rc" >&2
  cat /tmp/ceblar-20-virtio-serial.log
  cat /tmp/ceblar-20-virtio-test.log
  exit 1
fi
grep -q 'virtio-blk: legacy queue ready' /tmp/ceblar-20-virtio-serial.log
grep -q 'userspace: launching ring3 init' /tmp/ceblar-20-virtio-serial.log
if grep -qE 'KERNEL PANIC|exception:' /tmp/ceblar-20-virtio-serial.log; then
  cat /tmp/ceblar-20-virtio-serial.log
  exit 1
fi
cat /tmp/ceblar-20-virtio-serial.log
