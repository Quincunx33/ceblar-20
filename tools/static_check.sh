#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
grep -q -- '-Wall' Makefile
grep -q -- '-Wextra' Makefile
grep -q -- '-Werror' Makefile
git diff --check
if git ls-files | grep -E '(^|/)(build|\.git)(/|$)|\.(iso|img|log)$' >/tmp/ceblar-20-static-forbidden.log; then
  cat /tmp/ceblar-20-static-forbidden.log >&2
  exit 1
fi
printf '%s\n' 'static: strict flags, whitespace, and tracked-artifact checks passed'
