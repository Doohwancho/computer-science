import socket

HEADERSIZE = 10
IP = socket.gethostbyname(socket.gethostname())
PORT = 1233

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client :
    client.connect((IP, PORT))

    full_msg = ''
    new_msg = True

    while True :
        msg = client.recv(8).decode()
        if new_msg :
            print(f"new message length: {msg[:HEADERSIZE]}")
            msglen = int(msg[:HEADERSIZE])
            new_msg = False

        full_msg += msg

        if len(full_msg) - HEADERSIZE == msglen: #헤더에서 payload사이즈 적어주면, 딱 고까지 client가 parse해서 decode() 한다.
            print("Full message recovered")
            print(full_msg)
            print(full_msg[HEADERSIZE:])
            new_msg = True
            break

# console.log

# new message length: 21
# Full message recovered
# 21        Welcome to the server
# Welcome to the server

