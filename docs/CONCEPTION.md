# Conception

[Conception de l'Architecture en C]
    |
    +-- Serveur de Jeu (Local)
    |       |
    |       +-- Gestion des connexions (sockets)
    |       |
    |       +-- Logique du Jeu
    |       |       |
    |       |       +-- Gestion de la carte du jeu
    |       |       |
    |       |       +-- Gestion des joueurs et des bombes
    |       |
    |       +-- Synchronisation des joueurs
    |
    +-- Clients (Côté Joueur)
            |
            +-- Interface Utilisateur (simpliste)
            |       |
            |       +-- Affichage de la carte et des joueurs
            |       |
            |       +-- Indicateurs de score et de temps
            |
            +-- Communication avec le serveur (sockets)
            |
            +-- Gestion des Actions du Joueur
                    |
                    +-- Déplacement du personnage
                    |
                    +-- Placement des bombes
                    |
                    +-- Gestion des événements de jeu

(À visualiser dans vscode)

## Spécifications Techniques

Le serveur de jeu local est écrit en langage C et gère la logique du jeu "Bomber Student".

### Gestion de la Logique du Jeu

Le serveur gère la carte du jeu, y compris les obstacles, les objets et les emplacements des joueurs.
Il gère les joueurs, leurs positions, les bombes qu'ils posent, et les interactions entre les joueurs (éliminations, bonus, etc.).
Synchronisation des Joueurs :

Le serveur assure la synchronisation des actions des joueurs, veillant à ce que tous les clients disposent d'une vue cohérente du jeu.
Interface Utilisateur (simpliste) :

Les clients disposent d'une interface utilisateur minimale qui affiche la carte du jeu, les joueurs, les scores et le temps restant.
L'interface graphique peut être simpliste, mettant l'accent sur la clarté des informations de jeu.
Gestion des Actions du Joueur :

Les clients permettent aux joueurs de se déplacer sur la carte, de placer des bombes et d'interagir avec le jeu.
Ils envoient ces actions au serveur via des sockets pour que le serveur les propage aux autres joueurs.
