# ---- Builder Stage ----
FROM alpine:3.23 AS builder

RUN apk add --no-cache \
    wget \
    build-base \
    cmake \
    samurai \
    pkgconf \
    openssl-dev \
    glib-dev \
    gtk4.0-dev \
    gd-dev \
    bash

WORKDIR /src
COPY . /src

RUN cmake --preset release && cmake --build --preset release && \
    chmod +x setup_test_env.sh && ./setup_test_env.sh

# ---- Runtime Stage ----
FROM alpine:3.23

ENV DISPLAY=:0
ENV RESOLUTION=1280x720
ENV VNC_PORT=5900
ENV NOVNC_PORT=6080

RUN apk add --no-cache \
    openssl \
    glib \
    gtk4.0 \
    gd \
    dbus-x11 \
    font-liberation \
    xvfb \
    x11vnc \
    xfce4 \
    xfce4-terminal \
    bash \
    novnc \
    websockify \
    gst-plugins-base \
    gst-plugins-good \
    gst-plugins-bad \
    gst-plugins-rs-gtk4 \
    hicolor-icon-theme \
    adwaita-icon-theme \
    su-exec \
    sudo \
    && ln -s /usr/share/novnc/vnc_lite.html /usr/share/novnc/index.html \
    && adduser -D -s /bin/bash -u 1000 user1 \
    && echo "user1 ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/user1 \
    && chmod 0440 /etc/sudoers.d/user1

WORKDIR /home/user1

COPY --from=builder /src/home/user1 /usr/local/share/template_user1
COPY entrypoint.sh /usr/local/bin/entrypoint.sh
RUN chmod +x /usr/local/bin/entrypoint.sh

EXPOSE 5900 6080

CMD ["bash", "/usr/local/bin/entrypoint.sh"]
