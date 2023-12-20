#!/bin/bash
clear
make

# Detect OS
OS=$(uname)

# Open terminal based on OS
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

# Open terminal and execute main
open_terminal ./bin/main 49151

# In another terminal
open_terminal python3 ./python_client/client.py
