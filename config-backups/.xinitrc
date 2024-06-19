#!/bin/bash

dunst &
slstatus &
# enpass &
setxkbmap us -variant intl &
~/.repos/bin/lock_screen.sh &

exec dwm
