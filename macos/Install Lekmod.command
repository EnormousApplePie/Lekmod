#!/bin/sh
# Finder opens .command files in Terminal; selections use native macOS dialogs.
cd -- "$(dirname -- "$0")" || exit 1
if ! command -v python3 >/dev/null 2>&1; then
    printf '%s\n' 'Python 3.9+ is required. Install Python, then run this installer again.'
    read -r answer
    exit 1
fi
python3 -u install.py --gui
status=$?
if [ "$status" -ne 0 ]; then
    printf '%s\n' 'Press Return to close.'
    read -r answer
fi
exit "$status"
