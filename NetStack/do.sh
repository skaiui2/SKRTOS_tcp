sudo ip tuntap add dev tap0 mode tap user $(whoami)
sudo ip link set tap0 up
ip addr show tap0

