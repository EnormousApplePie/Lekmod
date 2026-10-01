#!/bin/sh
cd -- "$(dirname -- "$0")" || exit 1
if ! command -v python3 >/dev/null 2>&1; then
    printf '%s\n' 'Python 3.9+ and Xcode Command Line Tools are required.'
    read -r answer
    exit 1
fi
python3 build_launcher.py --open
result=$?
if [ "$result" -ne 0 ]; then
    printf '%s\n' 'Press Return to close.'
    read -r answer
fi
exit "$result"
