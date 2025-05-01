#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from scapy.all import *

# Configuration parameters
TAP_IFACE = "tap0"  # TAP device name
TAP_MAC = "5a:06:72:6d:98:51"  # Local MAC address for tap0

def process_packet(packet):
    """
    Process each captured packet and print its detailed information.
    """
    print("\n=== Captured Packet ===")
    if packet.haslayer(Ether):
        print("Ethernet Header:")
        print(f"  Source MAC: {packet[Ether].src}")
        print(f"  Destination MAC: {packet[Ether].dst}")
    if packet.haslayer(IP):
        print("IP Header:")
        print(f"  Source IP: {packet[IP].src}")
        print(f"  Destination IP: {packet[IP].dst}")
        print(f"  Protocol: {packet[IP].proto}")
    if packet.haslayer(TCP):
        print("TCP Header:")
        print(f"  Source Port: {packet[TCP].sport}")
        print(f"  Destination Port: {packet[TCP].dport}")
    if packet.haslayer(UDP):
        print("UDP Header:")
        print(f"  Source Port: {packet[UDP].sport}")
        print(f"  Destination Port: {packet[UDP].dport}")
    if packet.haslayer(ICMP):
        print("ICMP Header:")
        print(f"  Type: {packet[ICMP].type}")
        print(f"  Code: {packet[ICMP].code}")
    if packet.haslayer(Raw):
        print("Raw Data:")
        print(packet[Raw].load.hex())
    print(packet.summary())  # Print a brief summary of the packet

# Start sniffing and processing packets
if __name__ == "__main__":
    print(f"Listening on {TAP_IFACE} interface for traffic...")
    sniff(iface=TAP_IFACE, prn=process_packet, store=0)

