#!/bin/sh

brew update
brew unlink python
brew install gettext check bullet dbus fontconfig freetype fribidi gst-plugins-good gstreamer luajit openssl webp libsndfile glib libspectre libraw librsvg poppler lz4 pulseaudio
