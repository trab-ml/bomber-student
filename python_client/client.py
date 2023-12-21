
import socket
from network import Network
import re
import json
import random
import threading
import time

def rejoindre_partie(client_socket, game_name):
    """
    Sends a join game request to the server.

    Args:
        client_socket (socket): The client socket object.
        game_name (str): The name of the game to join.

    Returns:
        None
    """
    join_game_request = f"POST game/join {game_name}"
    client_socket.send(join_game_request.encode())

def recuperer_infos_joueur():
    net.send_to_server("Get infos")

net = Network()
net.init()

def mouvement(move):
    net.buffer = b'' 
    net.send_to_server(move)

def afficher_carte(carte):
    for ligne in carte:
        print(" ".join(ligne))
    print()
    
j=0
net.connect_to_server()

while True:
    user_input = input("\nVoulez-vous choisir un pseudo (I), créer une partie (C) ou rejoindre une partie (J) "
                        "ou afficher la liste des map (M)? (Q pour quitter) ")
    
    if user_input.upper() == "I":
        player_name = input("Veuillez entrer un nom de joueur : ")
        net.send_to_server(player_name)
        net.receive_from_server()
        print(net.buffer)
            
    elif user_input.upper() == "C":
        net.send_to_server("POST game/create")
        net.receive_from_server()
        print(net.buffer)
            
    elif user_input.upper() == "J":
        game_name = input("Veuillez entrer le nom de la partie à rejoindre : ")
        net.send_to_server("POST game/join")
        net.receive_from_server()
        print(net.buffer)
               
    elif user_input.upper() == "Q":
        print("Déconnexion")
        net.send_to_server("POST exit")
        net.client.close()
        break
        
    elif user_input.upper() == "L":
        net.send_to_server("GET game/list")
        net.receive_from_server()
        print(net.buffer)
            
    elif user_input.upper() == "M":
        net.send_to_server("GET maps/list")
        net.receive_from_server()
        print(net.buffer)
        
    else:
        print("Commande invalide. Veuillez entrer C pour créer une partie, J pour rejoindre, ou Q pour quitter.")

    time.sleep(1)