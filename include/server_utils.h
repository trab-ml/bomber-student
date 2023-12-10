#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#define SERVER_MAX_SIZE_LOGIN 10
#define WELCOME_MESSAGE "Dites bonjour à "
#define INACTIVITY_TIMEOUT_SECONDS 60 * 10 // Temps d'inactivité autorisé (en secondes)

void handleError(const char *message);
void sendMessage(int client_socket, const char *message);
void runServer(int server_socket, clientList *clients);
void handleNewClient(int client_socket, struct sockaddr_in *client_address, clientList *clients);
void processClientMessage(int client_socket, const char *buffer, clientList *clients);
void handleInactiveClients(clientList *clients);

#endif