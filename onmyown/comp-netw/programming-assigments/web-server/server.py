#import socket module
from socket import *
import sys # In order to terminate the program
import threading

serverPort = 12000

def handleConn(conn, addr):
    try:
        message = conn.recv(1024).decode()
        filename = message.split()[1]
        f = open(filename[1:])
        outputdata = f.read()
        conn.send("HTTP/1.1 200 OK\r\n".encode())
        conn.send(f"Content-Length: {len(outputdata)}\r\n".encode())
        conn.send("\r\n".encode())
        for i in range(0, len(outputdata)):
            conn.send(outputdata[i].encode())
        conn.close()
    except IOError:
        resp = "<h1>Couldn't find that, buddy</h1>"
        conn.send("HTTP/1.1 404 Not Found\r\n".encode())
        conn.send(f"Content-Length: {len(resp)}\r\n".encode())
        conn.send("\r\n".encode())
        for i in range(0, len(resp)):
            conn.send(resp[i].encode())
        conn.close()

    print(f"Connection closed: {addr}")


serverSocket = socket(AF_INET, SOCK_STREAM)

serverSocket.bind(("", serverPort))
serverSocket.listen(1)
print('Ready to serve...')
try:
    while True:
        connectionSocket, addr = serverSocket.accept()
        thread = threading.Thread(target=handleConn, args=(connectionSocket, addr))
        thread.start()
except KeyboardInterrupt:
    print("Shutting down server...")
finally:
    serverSocket.close()
    sys.exit()
