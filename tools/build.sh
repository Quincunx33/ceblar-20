#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")/.."
make check
make all
printf '%s\n' 'Build initialization completed.'
