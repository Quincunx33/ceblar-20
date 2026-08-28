#!/usr/bin/env bash
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
ITERATIONS="${1:-10}"
if ! [[ "$ITERATIONS" =~ ^[0-9]+$ ]] || [ "$ITERATIONS" -lt 1 ]; then echo "usage: $0 [iterations]" >&2; exit 2; fi
cd "$ROOT"
make -j2 >/tmp/ceblar_stress_build.log 2>&1 || { cat /tmp/ceblar_stress_build.log; exit 1; }
for n in $(seq 1 "$ITERATIONS"); do
  out="/tmp/ceblar_stress_${n}.serial"
  timeout 8s qemu-system-i386 -cdrom build/ceblar-20.iso -display none -serial "file:$out" -monitor none -no-reboot >/dev/null 2>&1 || true
  grep -q 'user: hello' "$out" || { echo "iteration $n: missing user marker"; exit 1; }
  grep -q 'timer: IRQ0 active' "$out" || { echo "iteration $n: missing timer marker"; exit 1; }
  grep -q 'panic\|exception' "$out" && { echo "iteration $n: kernel failure"; cat "$out"; exit 1; } || true
  echo "iteration $n: PASS"
done
make static-check
