#!/bin/sh

set -e

COPTS="--disable-cxx-bindings"

# Prepare OSX env for build
mkdir -p ~/Library/LaunchAgents
ln -sfv /usr/local/opt/d-bus/*.plist ~/Library/LaunchAgents
launchctl load ~/Library/LaunchAgents/org.freedesktop.dbus-session.plist
export PATH="$(brew --prefix gettext)/bin:$PATH"

export CFLAGS="-I/usr/local/opt/openssl/include $CFLAGS"
export LDFLAGS="-L/usr/local/opt/openssl/lib $LDFLAGS"

# Normal build test of all targets
./autogen.sh $COPTS $@
make
make examples
#make benchmark
