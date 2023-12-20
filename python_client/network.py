import socket
import re
MAX_BUFFER_SIZE = 2048
port = 49151


def extraire_map(response):
    match = re.search(r'"content":"([^"]+)"', response)
    if match:
        return match.group(1)
    else:
        return None
    
    
    
class Network:
    def __init__(self):
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server = "127.0.0.1"
        self.port = port
        self.addr = (self.server, self.port)
        self.buffer = bytearray(MAX_BUFFER_SIZE)

    def init(self):
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.addr = (self.server, self.port)

    def cleanup(self):
        self.client.close()

    def connect_to_server(self):
        try:
            self.client.connect(self.addr)
            print(f"connexion reussie sur le port :{port}")
            data="looking for bomberstudent servers"
            self.client.send(data.encode())
            
        except socket.error as e:
            print(f"Error connecting to the server: {e}")
            exit()

        # Receive initial message from the server
        self.buffer = self.client.recv(MAX_BUFFER_SIZE).decode()
        print(f"reponse:{self.buffer}")

    def send_to_server(self, data):
        self.client.send(data.encode())

        # # Receive response from the server
        # self.buffer = self.client.recv(MAX_BUFFER_SIZE)

    def receive_from_server(self):
        # Receive response from the server
        bytes_received = self.client.recv(MAX_BUFFER_SIZE - 1).decode()
        if not bytes_received:
            print("Connexion refusé par le serveur")
            exit()
        self.buffer = bytes_received
        
        

