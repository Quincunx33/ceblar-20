#!/usr/bin/env python3
import re
import sys
from pathlib import Path

source = Path(sys.argv[1] if len(sys.argv) > 1 else "userspace/user_image.c")
target = Path(sys.argv[2] if len(sys.argv) > 2 else "build/HELLO.ELF")
text = source.read_text()
blob = bytes(int(value, 16) for value in re.findall(r"0x([0-9a-fA-F]{2})", text))
blob = blob.replace(b"user: hello\n", b"disk: hello\n")
target.parent.mkdir(parents=True, exist_ok=True)
target.write_bytes(blob)
print(f"ELF image: {target} ({len(blob)} bytes)")
