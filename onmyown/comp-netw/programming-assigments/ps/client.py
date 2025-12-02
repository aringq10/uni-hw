from socket import *
import time

serverName = '127.0.0.1'
serverPort = 12000
serverAddress = (serverName, serverPort)
pings = 10
timeoutLimit = 1.0

mn = timeoutLimit * 1000
mx = 0.0
avg = 0.0
lossCount = 0

for i in range(0, pings):
    sock = socket(AF_INET, SOCK_DGRAM)
    sock.settimeout(timeoutLimit)
    message = f"Ping {i + 1}".encode()

    try:
        sock.sendto(message, serverAddress)
        start_time = time.time()
        data, addr = sock.recvfrom(1024)
        end_time = time.time()
        elapsed = (end_time - start_time) * 1000
        mn = min(mn, elapsed)
        mx = max(mx, elapsed)
        avg += elapsed
        print(data.decode(), f" {elapsed:.5f} ms")

    except TimeoutError:
        print("Request timed out")
        lossCount += 1

if pings - lossCount != 0:
    avg /= pings - lossCount

print(f"avg: {avg:.5f} ms, max: {mx:.5f} ms, min: {mn:.5f} ms")
print(f"Packet loss rate: {(lossCount / pings * 100):.2f}%")
