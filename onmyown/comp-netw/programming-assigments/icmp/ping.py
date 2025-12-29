from socket import *
import os
import sys
import struct
import time
import select
# import binascii

ICMP_ECHO_REQUEST = 8
DEF_TIMEOUT = 1
# RTT statistics
max = 0
min = 0
avg = 0

def checksum(data: bytes) -> int:
    csum = 0
    n = len(data)
    i = 0

    while i + 1 < n:
        csum += (data[i] << 8) | data[i + 1]
        csum = (csum & 0xffff) + (csum >> 16)
        i += 2

    if i < n:
        csum += data[i] << 8
        csum = (csum & 0xffff) + (csum >> 16)

    csum = (csum & 0xffff) + (csum >> 16)
    return (~csum) & 0xffff

def receiveOnePing(mySocket, ID, timeout, destAddr):
    timeLeft = timeout
    while 1:
        startedSelect = time.time()
        whatReady = select.select([mySocket], [], [], timeLeft)
        howLongInSelect = (time.time() - startedSelect)

        if whatReady[0] == []:
            return "Request timed out."

        timeReceived = time.time()
        recPacket, addr = mySocket.recvfrom(1024)
        if addr[0] != destAddr:
            timeLeft -= howLongInSelect
            if timeLeft <= 0:
                return "Request timed out."
            continue

        #Fetch the ICMP header from the IP packet
        ipLen = (recPacket[0] & 0x0f) * 4
        if ipLen >= 20 and len(recPacket) >= ipLen + 8:
            icmpPacket = recPacket[ipLen:]
            icmp_type, icmp_code, _cksum, pkt_id, pkt_seq = struct.unpack("!BBHHH", icmpPacket[:8])
            if len(icmpPacket) == 16 and icmp_type == 0 and icmp_code == 0 and pkt_id == ID:
                timeSent = struct.unpack("d", icmpPacket[8:16])[0]
                return f"{((timeReceived - timeSent) * 1000):.3f} ms"

        timeLeft = timeLeft - howLongInSelect
        if timeLeft <= 0:
            return "Request timed out."

def sendOnePing(mySocket, destAddr, ID):
    # Header is type (8), code (8), checksum (16), id (16), sequence (16)
    myChecksum = 0

    header = struct.pack("!BBHHH", ICMP_ECHO_REQUEST, 0, myChecksum, ID, 1)
    data = struct.pack("d", time.time())

    myChecksum = checksum(header + data)

    header = struct.pack("!BBHHH", ICMP_ECHO_REQUEST, 0, myChecksum, ID, 1)
    packet = header + data

    mySocket.sendto(packet, (destAddr, 1))

def doOnePing(destAddr, timeout, mySocket):
    myID = os.getpid() & 0xFFFF # Return the current process id

    sendOnePing(mySocket, destAddr, myID)
    delay = receiveOnePing(mySocket, myID, timeout, destAddr)
    return delay

def ping(host, timeout=1):
    dest = gethostbyname(host)
    print("Pinging " + dest + " using Python:")
    print("")

    icmp = getprotobyname("icmp")
    # SOCK_RAW is a powerful socket type. For more details: http://sock-raw.org/papers/sock_raw
    mySocket = socket(AF_INET, SOCK_RAW, icmp)

    try:
        while 1 :
            delay = doOnePing(dest, timeout, mySocket)
            print(delay)
            time.sleep(1)
    except KeyboardInterrupt:
        print("\nStopping")
    finally:
        mySocket.close()
        min = min * 1000
        print(f"RTT min/max/avg: {min:.3f}/{max:.3f}/{avg:.3f} ms")


def main():
    args = sys.argv[1:]

    if len(args) < 1:
        print(f"Usage: {sys.argv[0]} <host> [timeout]")
        raise SystemExit(2)

    timeout = int(args[1]) if len(args) >= 2 else DEF_TIMEOUT
    host = args[0]
    ping(host, timeout)

if __name__ == "__main__":
    main()
