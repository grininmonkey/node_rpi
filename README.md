# soon™


Raspberry Pi tcp fixes to help prevent Influx traffic from blocking after a period of time,
accumulating large number of TIME_WAIT sockets with Influx.

Add these lines to `/etc/sysctl.conf`:

```bash
# Allow reuse of sockets in TIME_WAIT state for new connections
net.ipv4.tcp_tw_reuse = 1

# Enable fast recycling of TIME_WAIT sockets (deprecated in newer kernels, but try it)
net.ipv4.tcp_tw_recycle = 0

# Reduce how long TIME_WAIT connections are kept (default: 60 seconds)
net.ipv4.tcp_fin_timeout = 15

# Expand ephemeral port range
net.ipv4.ip_local_port_range = 1024 65535
```

Apply:

```bash
sudo sysctl -p
```
