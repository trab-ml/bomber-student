# Bomber Student

TESTER

```bash
$ ls 
bin  doc  include  Makefile  README.md  run.sh  src
$ chmod +x ./run.sh
$ ./run.sh

# or
$ make; ./bin/main 49151 # server
$ nc localhost 49151 # client

# simultaneous connexions
$ make; ./bin/main 49151 # server
$ ./manyclients.sh # client
```

```bash
# [SERVER]
        # For Debugging
        gdb ./bin/main
        [ENTER]
        (gdb) run 49151
        ./bin/main 49151

        # For Memory Analysis
        # -v --> verbose
        # valgrind --vgdb=yes --vgdb-error=0 ./bin/main 49151
        # valgrind --leak-check=full ./bin/main 49151
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./bin/main 49151
# [CLIENT]
        nc localhost 49151
        # ...
```

## DEPENDANCES

- cJSON <https://github.com/DaveGamble/cJSON>

nc server_addr 49151
nc localhost 49151

## PROJECT STRUCTURE

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

## COMING SOON

- Memory leaks??
- Toutes les requêtes implémentées
- Logique du jeu intégrée
- Plusieurs Serveurs <https://excalidraw.com/#room=236d49594f1cecf84346,ra7t_Z5onmRz5D7Wn3DWBA>

## CURRENT STATE OF main.c

```bash
# [SERVER]
$ make; ./bin/main 49151
gcc -Wall -Werror -g -Iinclude -c -o bin/server_utils.o src/server_utils.c
gcc -Wall -Werror -g -o bin/main bin/cJSON.o bin/client_list.o bin/error_handler.o bin/main.o bin/server_utils.o bin/socket_utils.o bin/test_cJSON.o
[SERVER] Add client to clients list 127.0.0.1:57518, his login is Mario

# [CLIENT]
$ nc localhost 49151
testman
letmetest
okeeey
looking for bomberstudent server
looking for bomberstudent servers
hello i’m a bomberstudent server
Mariiiiiiiiiiiiio
login too long!
Mario
Get maps/list
{
        "statut":       400,
        "message":      "Bad request"
}GET maps/list
{
        "action":       "maps/list",
        "statut":       "200",
        "message":      "ok",
        "nbMapsList":   1,
        "maps": [{
                        "id":   0,
                        "width":        24,
                        "height":       8,
                        "content":      "************************=----------------------==----==============----==----------****--------==------****------------==----==============----==----------------------=************************"
                }]
}
```

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
