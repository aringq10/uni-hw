from socket import *
import threading
serverName = '127.0.0.1'
serverPort = 12000
connections = 100

def spawnConn(i):
    clientSocket = socket(AF_INET, SOCK_STREAM)
    clientSocket.connect((serverName,serverPort))
    req = f"GET /{i + 1} HTTP/1.1\r\n\r\n"
    clientSocket.send(req.encode())
    clientSocket.close()

for i in range(0, connections):
    thread = threading.Thread(target=spawnConn, args=(i,))
    thread.start()
