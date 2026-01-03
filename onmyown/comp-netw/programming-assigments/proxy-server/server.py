from socket import *
import sys
import os

if len(sys.argv) <= 1:
    print('Usage : "python3 ProxyServer.py port"')
    sys.exit(2)

# Create a server socket, bind it to a port and start listening
tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.bind(("", int(sys.argv[1])))
tcpSerSock.listen(2)

while 1:
    # Start receiving data from the client
    print('-------------------------------------------')
    print('Ready to serve...')

    tcpCliSock, addr = tcpSerSock.accept()
    print('Received a connection from:', addr)
    message = tcpCliSock.recv(1024).decode()
    print("MESSAGE:\n", message)

    # Extract the filename from the given message
    hostn = message.partition("Host: ")[2].partition("\r\n")[0].strip()
    hostn, _, port = hostn.partition(":")
    filename = ogfilename = message.split()[1].partition("http://")[2].partition("/")[2]
    print("Host: ", hostn)
    if port.strip():
        try:
            port = int(port)
            print("Port: ", port)
        except:
            port = 80
    else:
        port = 80
    if filename[-1] == '/':
        filename += "index.html"
    elif '.' not in filename:
        filename += "/index.html"
    print("File: ", filename)
    fileExist = "false"
    try:
        # Check wether the file exist in the cache
        f = open(f"./cache/{hostn}/{filename}", "rb")
        outputdata = f.read()
        fileExist = "true"
        fileExt = filename.rsplit(".", 1)[-1]

        if fileExt == "html":
            cntType = "text/html"
        elif fileExt == "css":
            cntType = "text/css"
        elif fileExt == "js":
            cntType = "application/javascript"
        else:
            cntType = ""

        if cntType:
            cntType = f"Content-Type: {cntType}\r\n"



        # ProxyServer finds a cache hit and generates a response message
        tcpCliSock.send(b"HTTP/1.1 200 OK\r\n")
        tcpCliSock.send(cntType.encode())
        tcpCliSock.send(f"Content-Length: {len(outputdata)}\r\n".encode())
        tcpCliSock.send(b"\r\n")
        tcpCliSock.send(outputdata)

        print('Read from cache')
        # Error handling for file not found in cache
    except IOError as e:
        if fileExist == "false":
            print("Forwading request")
            # Create a socket on the proxyserver
            c = socket(AF_INET, SOCK_STREAM)
            try:
                c.connect((hostn, 80))
                req = f"GET /{ogfilename} HTTP/1.1\r\nHost: {hostn}\r\n\r\n"
                c.send(req.encode())
                print("Request:\n", req)
                buffer = c.recv(4096)
                body = buffer.partition(b"\r\n\r\n")[2]

                # Also send the response in the buffer to client socket
                tcpCliSock.send(buffer)

                if buffer.split()[1] == b"200":
                    print("A 200 response, caching file")

                    path = f"./cache/{hostn}"
                    file_path = f"{path}/{filename}"

                    os.makedirs(file_path.rsplit("/", 1)[0], exist_ok=True)
                    print(file_path)

                    with open(file_path, "wb") as tmpFile:
                        tmpFile.write(body)
                else:
                    print(f"Not a 200 response({buffer.split()[1]}), skipping caching")
                c.close()
            except Exception as e:
                print(e)
                print("Illegal request")
                tcpCliSock.send(b"HTTP/1.1 404 Not Found\r\n")
                tcpCliSock.send(b"\r\n")
    # Close the client and the server sockets
    tcpCliSock.close()
# Fill in start.
# Fill in end.
