#!/bin/bash
set -e

cp -r /usr/local/share/template_user1/* /home/user1/ 2>/dev/null || true
mkdir -p /home/user1/images
chown -R user1:user1 /home/user1
chmod 775 /home/user1
chmod 570 /home/user1/* 2>/dev/null || true
chmod 775 /home/user1/images
chmod 664 /home/user1/images/* 2>/dev/null || true

exec su-exec user1 bash -c '
    export DISPLAY=:0
    Xvfb :0 -screen 0 1280x720x24 -dpi 96 &
    sleep 2

    export GDK_BACKEND=x11
    export LIBGL_ALWAYS_SOFTWARE=1
    export GSK_RENDERER=cairo

    eval $(dbus-launch --sh-syntax)

    x11vnc -display :0 -nopw -listen localhost -xkb -forever -shared -geometry 1280x720 &

    websockify --web /usr/share/novnc/ 6080 localhost:5900 &

    ./mediaplayer &

    exec xfce4-session
'
