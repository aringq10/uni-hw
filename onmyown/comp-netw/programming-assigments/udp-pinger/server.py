import random
from socket import *
import time
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(('', 12000))
while True:
    rand = random.randint(0, 10)
    message, address = serverSocket.recvfrom(1024)
    message = message.upper()
    if rand < 0:
        continue
    # rand = random.randint(0, 999)
    # time.sleep(rand / 1000)
    serverSocket.sendto(message, address)
