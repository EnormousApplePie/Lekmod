#!/bin/sh
# Keep the original Finder entry point working.
exec /bin/sh "$(dirname -- "$0")/Lekmod Launcher.command"
