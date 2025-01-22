import socket

IP = socket.gethostbyname(socket.gethostname())
PORT = 9999

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client :
    client.connect((IP, PORT))

    full_msg = ''
    while True :
        msg = client.recv(8) #server에서 온 byte[]를 8 byte씩 끊어서 읽는다.
        if len(msg) == 0 :
            break
        print(msg.decode())
        full_msg += msg.decode()

    print(full_msg)

# console.log
# Welcome
# to the s
# erver!
