import socket
import pickle

HEADER = 64
PORT = 5050
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"
INIT_MESSAGE = "INIT"
SERVER = "192.168.0.105"
ADDR = (SERVER, PORT)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(ADDR)

def send(msg):
    message = msg.encode(FORMAT)

    msg_length = len(message) #length of the message we need to send
    send_length = str(msg_length).encode(FORMAT) #encoed len to sen it to server
    send_length += b' ' * (HEADER - len(send_length)) #pad it with spaces to be 64 bytes
    client.send(send_length) #send 1st message to server containing length of message
    
    client.send(message) #send 2nd message containing the real msg

send(input())