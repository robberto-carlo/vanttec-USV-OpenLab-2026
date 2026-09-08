#!/bin/bash

sudo docker run --rm -it \
  --device=/dev/video0:/dev/video0 \
  -e QT_QPA_PLATFORM=wayland \
  -e XDG_RUNTIME_DIR=/tmp/runtime \
  -e WAYLAND_DISPLAY=wayland-0 \
  -v /run/user/1000/wayland-0:/tmp/runtime/wayland-0 \
  vanttec-usv
