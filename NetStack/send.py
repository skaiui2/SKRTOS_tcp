#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from scapy.all import *

# 配置参数（需根据实际情况修改）
TAP_IFACE = "tap0"                  # 你的 TAP 设备名
TAP_MAC    = "5a:06:72:6d:98:51"    # tap0 的 MAC（源 MAC）
DST_MAC    = "00:0c:29:e5:98:c9"
DST_IP     = "192.168.1.2"          # 目标 IP（Ping 请求方的 IP）
SRC_IP     = "192.168.91.139"          # 伪造的源 IP（需与请求包匹配）

# 构造 ICMP Echo Reply
reply = Ether(dst=DST_MAC, src=TAP_MAC) / \
         IP(src=SRC_IP, dst=DST_IP) / \
         ICMP(type=0, id=123, seq=1)  # type=0 是 Echo Reply

# 发送到 TAP 设备
sendp(reply, iface=TAP_IFACE, verbose=True)
print("Sent ICMP Echo Reply to {} via {}".format(DST_IP, TAP_IFACE))
