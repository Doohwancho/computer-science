import socket

HOST = socket.gethostname()
IP = socket.gethostbyname(socket.gethostname())
PORT = 8888

while True:
    msg = input("Type message to send to the server : ")
    if msg == "":
        print("Closing connection")

    #AF_INET for IPv4, AF_INET6 for IPv6
    #SOCK_STREAM for TCP, SOCK_DGRAM for UDP
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client:
        client.connect((IP, PORT)) #establish TCP connection with server on port 8888
        client.sendall(msg.encode()) #msg.encode() == string to byte[]
        if not msg == "" :
            reply = client.recv(1024).decode() #1024 is buffer size. 2^10. .decode() for byte[] -> string

    if msg == "":
        break
    else:
        print(f"Message received : {reply}")
