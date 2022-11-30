import socket
from sys import exit

HOST = socket.gethostname()
IP = socket.gethostbyname(HOST)
PORT = 9999

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as client :
    client.connect((IP, PORT))

    while True :
        msg = input("Enter messge : ")
        client.send(msg.encode())

        if msg == "" :
            print("Connection closed")
            client.close()
            exit()
        else :
            msg_received = client.recv(1024)
            print(f"Message from server : {msg_received.decode()}")
