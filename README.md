# SKRTOS_tcp

### Introduction

**SKRTOS_tcp** is a lightweight and efficient TCP/IP stack designed as a part of the SKRTOS operating system.But for now it is being developed as a separat part.It wil be integrated in the future.

Its implementation draws inspiration from the renowned "TCP/IP Illustrated, Volume 1-3" .

This stack provides essential networking features while emphasizing simplicity and scalability. Designed to integrate seamlessly with SKRTOS, it ensures reliable network functionality with minimal overhead.

Due to the complexity of developing the protocol stack directly on network card hardware, it is currently implemented as a Linux user-space protocol stack.

### USER MANUAL

none.

Currently, the stack is under development and supports basic **ARP** and **IP** functionalities. It doesn't have a lot of features.

### **How to Clone and Set Up the Stack**

Start by cloning the repository:

```
git clone git@github.com:skaiui2/SKRTOS_tcp.git
```

Next, create a TAP interface and set the required file permissions:

```
cd NetStack
./do.sh
```

After that, run the following command:

```
./k.sh
```

This will output:

```
MAC Address: 9e:4d:9e:e3:48:9f
IP Address: 192.168.1.200
```

### **Supported Features**

#### **Testing ARP**

You can test ARP by running the following command:

```
arping -I tap0 192.168.1.200
```

```
skaiuijing@ubuntu:~/NetStack$ arping -I tap0 192.168.1.200
ARPING 192.168.1.200 from 192.168.1.100 tap0
Unicast reply from 192.168.1.200 [9E:4D:9E:E3:48:9F]  0.621ms
Unicast reply from 192.168.1.200 [9E:4D:9E:E3:48:9F]  0.713ms
Unicast reply from 192.168.1.200 [9E:4D:9E:E3:48:9F]  0.769ms
```

#### **Testing IP (Ping)**

You can also test IP functionality with Ping:

```
ping 192.168.1.200
```

Expected output:

```
skaiuijing@ubuntu:~/NetStack$ ping 192.168.1.200
PING 192.168.1.200 (192.168.1.200) 56(84) bytes of data.
64 bytes from 192.168.1.200: icmp_seq=1 ttl=64 time=0.314 ms
64 bytes from 192.168.1.200: icmp_seq=2 ttl=64 time=0.461 ms
64 bytes from 192.168.1.200: icmp_seq=3 ttl=64 time=0.391 ms
64 bytes from 192.168.1.200: icmp_seq=4 ttl=64 time=0.264 ms
```

use python script to capture network traffic  :

```
skaiuijing@ubuntu:~/NetStack$ sudo python3 capture.py
```

this will output:

```
=== Captured Packet ===
Ethernet Header:
  Source MAC: 9e:4d:9e:e3:48:9f
  Destination MAC: 96:ef:2a:0e:b8:61
Ether / ARP is at 9e:4d:9e:e3:48:9f says 192.168.1.200

=== Captured Packet ===
Ethernet Header:
  Source MAC: 96:ef:2a:0e:b8:61
  Destination MAC: 9e:4d:9e:e3:48:9f
IP Header:
  Source IP: 192.168.1.100
  Destination IP: 192.168.1.200
  Protocol: 1
ICMP Header:
  Type: 8
  Code: 0
Raw Data:
e7bdf46700000000bbdf060000000000101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f3031323334353637
Ether / IP / ICMP 192.168.1.100 > 192.168.1.200 echo-request 0 / Raw

```

### Testing UDP 

**scan Port**

You can also test UDP:

```
skaiuijing@ubuntu:~/Documents$ sudo nmap -sU -p 1234 192.168.1.200

Starting Nmap 7.60 ( https://nmap.org ) at 2025-04-19 20:53 PDT
Nmap scan report for 192.168.1.200
Host is up (0.00014s latency).

PORT     STATE SERVICE
1234/udp open  search-agent
MAC Address: 9E:4D:9E:E3:48:9F (Unknown)

Nmap done: 1 IP address (1 host up) scanned in 0.55 seconds

```

**test Server**

compile  UDP-client code:

```
$: cd testdoc
$: gcc udp.c -o udp
$: ./udp
```

The UDP-client will send message "Lx" to SKRTOS_tcp.

then output:

```
Sent: Lx
Received: Lx
```

If output this, show the SKRTOS_tcp has received message "Lx", then send it to UDP-client.



