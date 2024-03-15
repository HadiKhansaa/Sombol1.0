import socket
import threading
import pickle

HEADER = 64
PORT = 5050
SERVER = socket.gethostbyname(socket.gethostname()) #192.168.0.105
ADDR = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"
room_id = 0
INITIAL_BOARD = [
                [0, 0, 0, 0, 0, 0, 0, 0],
                [1, 1, 1, 1, 1, 1, 1, 1],
                [1, 1, 1, 1, 1, 1, 1, 1],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [2, 2, 2, 2, 2, 2, 2, 2],
                [2, 2, 2, 2, 2, 2, 2, 2],
                [0, 0, 0, 0, 0, 0, 0, 0]
               ]

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(ADDR)

def handle_client(conn, addr):
    global room_id
    while True:
        serialized_msg = conn.recv(100000)
        if serialized_msg.decode() == "INIT":
            #wait for another player
            #send to 2nd player
            conn2, addr2 = server.accept()
            serialized_msg2 = conn2.recv(100000)
            
            init_msg = {"color":1, "board_layout":INITIAL_BOARD, "room_id":room_id, "opponent_addr":addr}
            to_send_msg = pickle.dumps(init_msg)
            conn2.send(to_send_msg)

            #send to first player
            init_msg2 = {"color":2, "board_layout":INITIAL_BOARD, "room_id":room_id, "opponent_addr": addr2}
            to_send_msg = pickle.dumps(init_msg2)
            conn.send(to_send_msg)

            room_id+=1
        else:
            print("hooooooo")
            msg = pickle.loads(serialized_msg)
            client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client.bind(msg["opponent_addr"])
            msg2 = {"board_layout": msg.board_layout }
            client.send(pickle.dumps(msg2))

def start():
    global room_id
    server.listen()
    print(f"[LISTENING] server is listening in {SERVER}")
    while True:
        conn, addr = server.accept()
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()
        # handle_client(conn, addr)
        print(f"[ACTIVE CONNECTIONS] {threading.active_count() - 1}")


print("[STARTING] server is starting...")
start()