import socket
import json
import time

def send_request(request_data, socket):
    socket.sendall(request_data.encode())
    response_data = socket.recv(1024).decode()
    print(response_data)

def main():
    host = "localhost"
    port = 49151

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))

        # Envoie de la première requête
        initial_request = "looking for bomberstudent servers"
        send_request(initial_request, s)

        # Réception de la réponse
        server_response = s.recv(1024).decode()
        print(server_response)

        # Envoie du login
        login_request = "Mariooo"
        send_request(login_request, s)

        # Envoie de la requête GET maps/list
        # get_maps_request = json.dumps({
        #     "action": "maps/list"
        # })
        # send_request(get_maps_request, s)
        
        time.sleep(1)
        print("Envoie de la requête GET maps/list")
        
        send_request("GET maps/list", s)
        server_response = s.recv(1024).decode()
        print(server_response)
        
        print("fin traitement GET maps/list")
        
        time.sleep(1)

if __name__ == "__main__":
    main()
