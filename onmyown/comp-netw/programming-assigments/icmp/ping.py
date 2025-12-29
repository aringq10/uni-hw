from socket import *
import os
import sys
import struct
import time
import select

ICMP_ECHO_REQUEST = 8
DEF_TIMEOUT = 1.0
DEF_COUNT = -1

# RTT statistics
stats = {
    "max": 0,
    "min": float("inf"),
    "avg": 0,
    "total": 0,
    "lost": 0,
}


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

def receiveOnePing(mySocket, ID, timeout, destAddr, iteration):
    timeLeft = timeout
    while 1:
        startedSelect = time.time()
        whatReady = select.select([mySocket], [], [], timeLeft)
        howLongInSelect = (time.time() - startedSelect)

        if whatReady[0] == []:
            return -1

        timeReceived = time.time()
        recPacket, addr = mySocket.recvfrom(1024)
        if addr[0] != destAddr:
            timeLeft -= howLongInSelect
            if timeLeft <= 0:
                return -1
            continue

        #Fetch the ICMP header from the IP packet
        ipLen = (recPacket[0] & 0x0f) * 4
        if ipLen >= 20 and len(recPacket) >= ipLen + 8:
            icmpPacket = recPacket[ipLen:]
            icmp_type, icmp_code, _cksum, pkt_id, pkt_seq = struct.unpack("!BBHHH", icmpPacket[:8])
            if len(icmpPacket) == 16 and icmp_type == 0 and icmp_code == 0 and pkt_id == ID and (iteration & 0xffff) == pkt_seq:
                timeSent = struct.unpack("d", icmpPacket[8:16])[0]
                return (timeReceived - timeSent) * 1000

        timeLeft = timeLeft - howLongInSelect
        if timeLeft <= 0:
            return -1

def sendOnePing(mySocket, destAddr, ID, iteration):
    # Header is type (8), code (8), checksum (16), id (16), sequence (16)
    myChecksum = 0

    header = struct.pack("!BBHHH", ICMP_ECHO_REQUEST, 0, myChecksum, ID, iteration)
    data = struct.pack("d", time.time())

    myChecksum = checksum(header + data)

    header = struct.pack("!BBHHH", ICMP_ECHO_REQUEST, 0, myChecksum, ID, iteration)
    packet = header + data

    mySocket.sendto(packet, (destAddr, 1))

def doOnePing(destAddr, timeout, mySocket, iteration):
    myID = os.getpid() & 0xFFFF # Return the current process id

    sendOnePing(mySocket, destAddr, myID, iteration)
    delay = receiveOnePing(mySocket, myID, timeout, destAddr, iteration)
    return delay

def ping(host, timeout=1.0, count=-1):
    dest = gethostbyname(host)
    print("Pinging " + dest + " using Python:")
    print("")

    icmp = getprotobyname("icmp")
    # SOCK_RAW is a powerful socket type. For more details: http://sock-raw.org/papers/sock_raw
    mySocket = socket(AF_INET, SOCK_RAW, icmp)

    iteration = 0

    try:
        while iteration != count:
            iteration += 1
            delay = doOnePing(dest, timeout, mySocket, iteration)
            if delay == -1:
                print("Request timed out")
                stats["lost"] += 1
            else:
                print(f"{iteration}. {delay:.3f} ms")
                stats["min"] = min(stats["min"], delay)
                stats["max"] = max(stats["max"], delay)
                stats["avg"] += delay
            stats["total"] += 1
            time.sleep(1)
    except KeyboardInterrupt:
        print("\nStopping")
    finally:
        mySocket.close()
        if stats["total"] == stats["lost"]:
            lostPrc = 100
        else:
            stats["avg"] /= stats["total"] - stats["lost"]
        if stats["total"] == 0:
            lostPrc = 0
        else:
            lostPrc = stats["lost"] / stats["total"] * 100
        print("")
        print(f"Packet loss: {lostPrc:.3f}% {stats["lost"]}/{stats["total"]}")
        print(f"RTT min/max/avg: {stats["min"]:.3f}/{stats["max"]:.3f}/{stats["avg"]:.3f} ms")


def main():
    args = sys.argv[1:]

    if len(args) < 1:
        print(f"Usage: {sys.argv[0]} <host> [timeout] [count]")
        raise SystemExit(2)

    timeout = float(args[1]) if len(args) >= 2 else DEF_TIMEOUT
    count = int(args[2]) if len(args) >= 3 else DEF_COUNT
    host = args[0]
    ping(host, timeout, count)

if __name__ == "__main__":
    main()
