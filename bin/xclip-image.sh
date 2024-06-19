#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <image-file>"
  exit 1
fi

xclip -selection clipboard -t image/png -i "$1"

