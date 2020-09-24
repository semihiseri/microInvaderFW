import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(bytes("50;-50", "utf-8"), ("192.168.1.105", 3000))
