# BOMBER STUDENT

Ce projet a pour but de créer un serveur de jeu proposant à ses utilisateurs de jouer au jeu
"Bomber Man". Le projet consiste à développer d’une part le serveur de jeu lui-même et d’autre
part le client permettant à des utilisateurs de se connecter au serveur afin de jouer les uns contre
les autres.

## A Faire

- Gérer en intégralité [les requêtes des joueurs](./src/query_handler.c), notamment en passant la main à [la logique du jeu](./src/game_main.c) qui est entièrement fonctionnelle.

## TESTER

```bash
$ ls 
bin  doc  include  Makefile  manyclients.sh  python_client  README.md  run.sh  src
$ chmod +x ./run.sh
$ ./run.sh

# or
$ make; ./bin/main 49151 # server
$ python3 ./python_client/client.py # client
```

## STRUCTURE DU PROJET

bomber-student/
|-- src/
|   |-- cJSON.c
|   |-- server.c
|   |-- testcJSON.c
|   |-- ...
|-- include/
|   | ... les .h ...
|-- bin/
|   | ... les .exe ...
|-- doc/
|   | ... doc + rscr ...
|-- Makefile
|-- README.md
|-- python_client
|   | ... les .py ...

## ARCHITECTURE

<https://excalidraw.com/#room=236d49594f1cecf84346,ra7t_Z5onmRz5D7Wn3DWBA>

ou sur [https://excalidraw.com/](doc/multipleClients.excalidraw), 'ouvrir' puis 'charger depuis un fichier'

[plus](./doc/ARCHITECTURE.md)

## DÉPENDANCES

- cJSON <https://github.com/DaveGamble/cJSON>
- Python <https://www.python.org/>

## RESSOURCES

- <https://users.ece.cmu.edu/~eno/coding/CCodingStandard.html>
- <https://sourceware.org/gdb/current/onlinedocs/gdb#Running>
- <https://valgrind.org/docs/manual/manual-core-adv.html#manual-core-adv.gdbserver-simple>

- <https://www.youtube.com/watch?v=LtXEMwSG5-8>
- <https://www.youtube.com/watch?v=Pg_4Jz8ZIH4>
- <https://github.com/karimbeji/bombermanmmgr/tree/master>
- <https://oldschoolprg.x10.mx/projets.php#bomberman>
- <https://codes-sources.commentcamarche.net/source/17477-bomberman-code-r-avec-sdl-simple-directmedia-layer>
- <https://youtu.be/77vYKsXC4IE?si=CTjuEt7pcNMZnZAL>
- <https://youtube.com/playlist?list=PLGVRGFq_teMPG5MPmZrDaVh3VlsGuC97k&si=b4EJtzW9W7dDLUxd>
- <https://github.com/DaveGamble/cJSON>
