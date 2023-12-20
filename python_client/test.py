import socket
import time

# Constantes
MAX_BUFFER_SIZE = 2048
SERVER_ADDRESS = "127.0.0.1"
SERVER_PORT = 49151

# Classe principale pour la gestion du réseau
class Network:
    def __init__(self):
        self.client = None
        self.buffer = ''

    def connect_to_server(self):
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client.connect((SERVER_ADDRESS, SERVER_PORT))
        print(f"Connexion réussie sur {SERVER_ADDRESS}:{SERVER_PORT}")

    def send_to_server(self, data):
        self.client.send(data.encode())

    def receive_from_server(self):
        self.buffer = self.client.recv(MAX_BUFFER_SIZE).decode()
        print(f"Réponse du serveur: {self.buffer}")

    def cleanup(self):
        self.client.close()

# Fonction principale
def main():
    net = Network()
    net.connect_to_server()

    # Étape 1 : Recherche de serveurs
    net.send_to_server("looking for bomberstudent servers")
    net.receive_from_server()

    print(f"reponse:{net.buffer}")
    if net.buffer == "hello i’m a bomberstudent server":
        # Étape 2 : Envoi du pseudo
        pseudo = input("Entrez votre pseudo : ")
        net.send_to_server(pseudo)

        while True:
            # Étape 3 : Interaction avec le serveur en fonction de la lettre choisie
            user_input = input("Choisissez une action (C: Créer partie, M: Afficher cartes, D: Se déplacer, Q: Quitter) : ")
            net.send_to_server(user_input)
            net.receive_from_server()

            if user_input.upper() == "Q":
                print("Déconnexion...")
                break

            time.sleep(1)
    else:
        print("Serveur non reconnu.")

    net.cleanup()

if __name__ == "__main__":
    main()


