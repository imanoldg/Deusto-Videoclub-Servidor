/*
 * main.cpp
 *
 *  Created on: 15 may 2024
 *      Author: Imanol
 */
#include <winsock2.h>
#include <iostream>
extern "C" {
#include "sqlite3.h"
}



#include "sqliteMetodos.h"
#include "Usuario.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

using namespace std;

int main(void){
	WSADATA wsaData;
	SOCKET conn_socket;
	SOCKET comm_socket;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512];

	cout << "Iniciando libreria socket\n" << endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << " No se ha podido iniciar la libreria socket. Codigo de error: " << WSAGetLastError() << endl;
		return -1;
	}
	cout << "Libreria socket iniciada" << endl;

	//SOCKET creation
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "No se ha podido crear el socket. Codigo de error: " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}
	cout << "Socket creado correctamente" << endl;

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
		cout << "Error al vincular el socket. Codigo de error: " << WSAGetLastError() <<endl;
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}
	cout << "Bindeo realizado correctamente" << endl;

	//LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		cout << "Error al activar modo escucha. Codigo de error: " << WSAGetLastError() <<endl;
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//ACCEPT incoming connections (server keeps waiting for them)
	cout << "Esperando a conexiones del cliente..." << endl;
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);

	if (comm_socket == INVALID_SOCKET) {
		cout << "Error al aceptar conexion del cliente. Codigo de error: " << WSAGetLastError() << endl;
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	cout << "Conexion entrante por: " << inet_ntoa(client.sin_addr) << " (" << ntohs(client.sin_port) << ")" << endl;

	// Closing the listening sockets (is not going to be used anymore)
	closesocket(conn_socket);

	cout << "Esperando comandos del cliente..." << endl;

	do {
		recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
		cout << "Comando recibido: " << recvBuff << endl;

		if (strcmp(recvBuff, "SESION_INIT") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char usuario[strlen(recvBuff)];
			strcpy(usuario, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char contrasenha[strlen(recvBuff)];
			strcpy(contrasenha, recvBuff);

			cout<<usuario<<endl;
			cout<<contrasenha<<endl;
			Usuario u;

			int resultado = iniciarSesion(usuario, contrasenha, u);

			//SE HACE UN SPRINTF POR CADA PROPIEDAD DEL USUARIO

			sprintf(sendBuff, "%s", u.getDNI());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%s", u.getNombre());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%s", u.getApellido());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%s", u.getEmail());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%d", u.getTlf());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%s", u.getUser());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%s", u.getContra());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%s", u.getGenero());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%s", u.getFechaNcto());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%d", u.getNumTarjeta());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%d", u.getPuntos());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

			sprintf(sendBuff, "%d", resultado);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
		}


		if(strcmp(recvBuff, "PASS_CHANGE") == 0){
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char contrasenha[strlen(recvBuff)] = "";
			strcpy(contrasenha, recvBuff);

			if (passChange(dni, contrasenha) == 0){
				sprintf(sendBuff, "Contraseña cambiada");
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}
		}


		if(strcmp(recvBuff, "GET_NUM_ALQUILERES") == 0){
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);
			int numPelis = getNumAlquileres(dni);

			sprintf(sendBuff, "%d", numPelis);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
		}

		if(strcmp(recvBuff, "GET_ALQUILERES") == 0){
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[100];
			strcpy(dni, recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int numPelis = atoi(recvBuff);

			Pelicula* p = new Pelicula();
			listaPelis peliculas(p, numPelis);
			getAlquileres(dni, peliculas);

			sprintf(sendBuff, "%d", numPelis);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

			for (int i = 0; i < numPelis; ++i) {
				sprintf(sendBuff, "%s", peliculas.pelis[i].getNombre());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}


		}

		if(strcmp(recvBuff, "UPDATE_PUNTOS") == 0){
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int numPuntos = atoi(recvBuff);

			if(updatePuntos(dni, numPuntos) == 0){
				sprintf(sendBuff, "Puntos actualizados. ¡Disfruta de tu oferta!");
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}
		}

	} while (1);
}
