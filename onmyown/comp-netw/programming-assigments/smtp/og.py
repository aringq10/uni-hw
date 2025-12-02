from socket import *
import uuid
from datetime import datetime

# Choose a mail server (e.g. Google mail server) and call it mailserver
port = 1025
mailserver = "localhost"
sender = "<aringas.civilka@mif.stud.vu.lt>"
rcpt = "<aringas.benas.civilka@gmail.com>"

from_addr = "aringas.civilka@mif.stud.vu.lt"
to_addr = "aringas.benas.civilka@gmail.com"
subject = "Test Email"
msg_id = f"<{uuid.uuid4().hex}@example.com>"
date = datetime.utcnow().strftime('%a, %d %b %Y %H:%M:%S +0000')

headers = f"""From: {from_addr}
To: {to_addr}
Subject: {subject}
Message-ID: {msg_id}
Date: {date}
MIME-Version: 1.0
Content-Type: text/plain; charset="utf-8"
Content-Transfer-Encoding: 7bit

"""

body = "Hello, this is a test email.\n"
message = headers + body

# Create socket called clientSocket and establish a TCP connection with mailserver
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((mailserver, port))

print("----------------------------------")
print("Connected")

recv = clientSocket.recv(1024).decode()
print(recv)
if recv[:3] != '220':
    print('220 reply not received from server.')

# Send HELO command and print server response.
heloCommand = 'HELO Alice\r\n'
clientSocket.send(heloCommand.encode())
recv1 = clientSocket.recv(1024).decode()
print(recv1)
if recv1[:3] != '250':
    print('250 reply not received from server.')

# Send MAIL FROM command and print server response.
mailFromCommand = f'MAIL FROM: {sender}\r\n'
clientSocket.send(mailFromCommand.encode())
recv2 = clientSocket.recv(1024).decode()
print(recv2)
if recv2[:3] != '250':
    print('250 reply not received from server.')

# Send RCPT TO command and print server response.
mailToCommand = f'RCPT TO: {rcpt}\r\n'
clientSocket.send(mailToCommand.encode())
recv3 = clientSocket.recv(1024).decode()
print(recv3)
if recv3[:3] != '250':
    print('250 reply not received from server.')

# Send DATA command and print server response.
clientSocket.send(b"DATA\r\n")
recv4 = clientSocket.recv(1024).decode()
print(recv4)
if recv4[:3] != '354':
    print('354 reply not received from server.')

# Message ends with a single period.
clientSocket.sendall(message.encode() + b"\r\n.\r\n")
recv5 = clientSocket.recv(1024).decode()
print(recv5)
if recv5[:3] != '250':
    print('250 reply not received from server.')

# Send QUIT command and get server response.
clientSocket.send(b"QUIT\r\n")
recv6 = clientSocket.recv(1024).decode()
print(recv6)
if recv6[:3] != '221':
    print('221 reply not received from server.')
