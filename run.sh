#!/bin/bash
clear
make

# Détecte le système d'exploitation
OS=$(uname)

# Sélectionne la commande appropriée
open_terminal() {
    case "$OS" in
        "Linux")
            if command -v gnome-terminal &> /dev/null; then
                gnome-terminal -- "$@"
            elif command -v xterm &> /dev/null; then
                xterm -e "$@"
            else
                echo "Terminal non pris en charge sur Linux."
            fi
            ;;
        "Darwin")
            open -a Terminal "$@"
            ;;
        "Windows_NT")
            start "$@"
            ;;
        *)
            echo "Système d'exploitation non pris en charge."
            ;;
    esac
}

# Ouvre un nouveau terminal et exécute le binaire main
open_terminal ./bin/main 49151

# Ouvre un autre terminal et exécute nc sur localhost 49151
open_terminal nc localhost 49151
