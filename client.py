
import socket
from network import Network
import re
import json
import game
import random
import threading
import time

# Fonction pour afficher les map disponibles
def affiche_map(client_socket):
    message2 = "GET maps/list"
    client_socket.send(message2.encode())



def print_formatted_map(map_content):
    # Divise le contenu du map en lignes
    map_lines = [map_content[i:i+24] for i in range(0, len(map_content), 24)]

    # Affiche chaque ligne avec un retour à la ligne
    for line in map_lines:
        print(line)


def formatted_map(map_content):
    if map_content is None:
        print("Erreur: map_content est None.")
        return []
    else:
        # Divise le contenu du map en lignes
        map_lines = [map_content[i:i+24] for i in range(0, len(map_content), 24)]
        return map_lines



    
    
# Fonction pour recevoir la réponse du serveur, extraire le premier map et le retourner
def recevoir_et_extraire_map(client_socket):
    # Envoyer la demande pour afficher les maps
    affiche_map(client_socket)

    # Recevoir la réponse du serveur
    receveur = client_socket.recv(4096).decode()  # Ajuster la taille du tampon en fonction de vos besoins

    # Afficher la réponse (vous pouvez commenter cette ligne si vous ne voulez pas afficher la réponse)
    print(f"Message reçu du serveur: {receveur}")

    # Extraire le contenu du premier map
    map_content = extraire_map(receveur)

    # if map_content:
    #     print(f"Contenu du premier map:\n{map_content}")
    #     ajouter_client_position(map_content)
    # else:
    #     print("Impossible d'extraire le contenu du map.")

    # Retourner le contenu du premier map
    return map_content


    
def extraire_map(response):
    try:
        # Charger la réponse JSON
        donnees_json = json.loads(response)

        # Vérifier si la clé 'maps' existe dans la réponse
        if 'maps' in donnees_json:
            # Récupérer le premier élément de la liste 'maps'
            map_info = donnees_json['maps'][0]

            # Vérifier si la clé 'content' existe dans la carte
            if 'content' in map_info:
                # Renvoyer le contenu de la carte
                return map_info['content']
    except json.JSONDecodeError:
        pass

    return None



    
    
class Joueur:
    def __init__(self, nom, position_x, position_y, score,life):
        self.nom = nom
        self.caractere=""
        self.position_x = position_x
        self.position_y = position_y
        self.score = score
        self.life = life

joueurs = [] #tableau des joueurs

def ajouterJoueur(data):
    for joueur_data in data['players']:
        nom = joueur_data['name']
        pos = joueur_data['pos']
        position_x = pos['x']
        position_y = pos['y']
        # Créer une instance de la classe Joueur
        joueur = Joueur(nom, position_x, position_y,0,100)
         # Ajouter le joueur au tableau
        joueurs.append(joueur)
    


def affiche_joueurs(joueurs):
    for joueur in joueurs:
        print(f"Nom du joueur : {joueur.nom}")
        print(f"Position du joueur : ({joueur.position_x}, {joueur.position_y})")
        print(f"Score du joueur : {joueur.score}")
        print(f"Caractère du joueur : {joueur.caractere}")
        print()

    
   
  
# # Fonction pour rejoindre une partie de jeu
# def rejoindre_partie(client_socket, game_name):
#     join_game_request = f"POST /game/join {game_name}"
#     client_socket.send(join_game_request.encode())

def recuperer_infos_joueur():
    net.send_to_server("Get infos")

#tentative de connexion au serveur 
net = Network()
net.init()


def recuperer_nom_joueur():
    net.buffer = b'' 
    net.send_to_server("name")
    net.receive_from_server()
    return net.buffer

def generer_position_aleatoire():
    largeur_carte = 24
    hauteur_carte = 8
    position_x = random.randint(0, largeur_carte - 1)
    position_y = random.randint(0, hauteur_carte - 1)
    return position_x, position_y 

def refreshMap():
    net.buffer = b'' 
    net.send_to_server("GET maps/list")
    net.receive_from_server()
    map =formatted_map(extraire_map(net.buffer)) 
    return map

def mouvement(move):
    net.buffer = b'' 
    net.send_to_server(move)
    # net.receive_from_server()
    # return net.buffer


    

def afficher_carte(carte):
    
    for ligne in carte:
        print(" ".join(ligne))
    print()
    

def update():
    threading.Timer(0.1, afficher_carte, args=[refreshMap()]).start()
    
j=0
net.connect_to_server()

while True:
    user_input = input("Voulez-vous créer une partie (C) ou rejoindre une partie (J) "
                        "ou afficher la liste des jeux (L) ou liste des map (M)? (Q pour quitter) ")
    
    if user_input.upper() == "C":
            
            net.send_to_server("POST /game/create")
            net.receive_from_server()
            print(net.buffer)
            
    elif user_input.upper() == "J":
            game_name = input("Veuillez entrer le nom de la partie à rejoindre : ")
            net.send_to_server(f"POST /game/join {game_name}")
            net.receive_from_server()
            print(net.buffer)
            
            # Charger la chaîne JSON
            # data = json.loads(net.buffer)
            # ajouterJoueur(data)
           
            # affiche_joueurs(joueurs)
           
           
            # game.player_position[recuperer_nom_joueur()]=list(generer_position_aleatoire())
            # afficher_carte(refreshMap())
            # net.buffer = b'' 
            # net.send_to_server("GET maps/list")
            # net.receive_from_server()
            # map =formatted_map(extraire_map(net.buffer)) 
            print(recuperer_nom_joueur())
            # map=refreshMap()
            # print(map)
            # map =mouvement("D")
            # mov= input("Choisissez un mouvement D G B H")
            # if mov=="D":
            #     map=mouvement("D")
            game.ajouter_joueur()
            
            #boucle du jeu 
            while True:
                # game.tableau =refreshMap()
                # game.player_position[recuperer_nom_joueur()]=list(generer_position_aleatoire())
                # game.start_game(recuperer_nom_joueur())
                update()
                time.sleep(1)
                
                print("\n")
                
                name = input("Choisissez un movement D G H B  et K pour quitter le jeu : ")
                if name == "K":
                    break
                elif name == "D":
                    # if map_content is None:
                    #     print("Erreur: map_content est None. Veuillez initialiser la carte correctement.")
                    #     continue  # Retourne au début de la boucle pour demander à nouveau une entrée
                    mouvement(name)

                       
                    
                    
                    
                    
                    
                     
                
                    
                    
                
            # game.tableau = map
            # game.start_game(recuperer_nom_joueur())
            # j=j+1
            
            # while True:
            #     rejoindre(net.client,1)
              
            #     receveur = net.client.recv(1024).decode()
                
            #     print_formatted_map(receveur)
            #     break
                    # ajouter_client_position(map_content)
               
           
             
    # elif user_input.upper() =="N":
    #         # rejoindre(client_socket,1)
               
               
    elif user_input.upper() == "Q":
            print("Déconnexion")
            net.send_to_server("POST deconnect")
            
            net.client.close()
            break
    elif user_input.upper() == "L":
            net.send_to_server("GET game/list")
            net.receive_from_server()
            print(net.buffer)
            
    elif user_input.upper() == "M":
            net.send_to_server("GET maps/list")
            net.receive_from_server()
            # print(receive_map_from_server('127.0.0.1',43589))
            print_formatted_map(extraire_map(net.buffer))
           
    else:
            print("Commande invalide. Veuillez entrer C pour créer une partie, J pour rejoindre, ou Q pour quitter.")

    
    

    

# # # Affichage de la réponse

# print("Réponse du serveur: {net.buffer}")

# Nettoyag