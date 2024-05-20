
/*
 * sqliteMetodos.cpp
 *
 *  Created on: 15 may 2024
 *      Author: Imanol
 */
extern "C" {
#include "sqlite3.h"
}
#include "Usuario.h"
#include "Peliculas.h"
#include "string.h"
#include <iostream>
using namespace std;

sqlite3* abrirDB() {

	sqlite3 *db;

	int result = sqlite3_open("UserDB.db", &db);

	if (result != SQLITE_OK) {
		cout << "Error abriendo la base de datos" << endl;
	}


	cout << "BASE DE DATOS ABIERTA" << endl;

	return db;
}

int iniciarSesion(char *usuario, char *contrasenha, Usuario &u) {
	sqlite3 *db = abrirDB();

	int result;

	sqlite3_stmt *stmt;

	//COMPROBAR QUE LA CONTRASEÑA DEL USUARIO INTRODUCIDO ES CORRECTA
	char sql[] = "SELECT Password FROM usuario WHERE User = ?";

	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	sqlite3_bind_text(stmt, 1, usuario, strlen(usuario), SQLITE_STATIC);

	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) {
			//GUARDA LA CONTRASEÑA DEL USUARIO EN UNA VARIABLE
			u.setContra((char*) sqlite3_column_text(stmt, 0));
		}
	} while (result == SQLITE_ROW);

	sqlite3_finalize(stmt);

	if (strcmp(contrasenha, u.getContra()) == 0) {
		cout << "USUARIO Y CONTRASEÑA CORRECTOS" << endl;
		cout << endl;
		cout << "SESIÓN INICIADA" << endl;

		//COPIAR TODOS LOS DATOS DEL USUARIO DE LA BD AL USUARIO PASADO POR PARAMETROS
		char sql[] = "SELECT * FROM usuario WHERE User = ?";

		sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		sqlite3_bind_text(stmt, 1, usuario, strlen(usuario), SQLITE_STATIC);

		do {
			result = sqlite3_step(stmt);
			if (result == SQLITE_ROW) {
				u.setDNI((char*) sqlite3_column_text(stmt, 0));
				u.setNombre((char*) sqlite3_column_text(stmt, 1));
				u.setApellido((char*) sqlite3_column_text(stmt, 2));
				u.setEmail((char*) sqlite3_column_text(stmt, 3));
				u.setTlf((int) sqlite3_column_int(stmt, 4));
				u.setUser((char*) sqlite3_column_text(stmt, 5));
				u.setContra((char*) sqlite3_column_text(stmt, 6));

				if (sqlite3_column_int(stmt, 7) == 1) {
					u.setGenero("M");
				} else {
					u.setGenero("F");
				}

				u.setFechaNcto((char*) sqlite3_column_text(stmt, 8));
				u.setPuntos((int) sqlite3_column_int(stmt, 9));
			}
		} while (result == SQLITE_ROW);

		sqlite3_finalize(stmt);
		//CERRAR BASE DE DATOS
		sqlite3_close(db);

		return 1;
	} else {
		printf("ERROR INICIANDO SESION\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		//CERRAR BASE DE DATOS
		sqlite3_close(db);
		return 0;
	}
}

int passChange(char *dni, char *contrasenha) {
	sqlite3 *db = abrirDB();

	sqlite3_stmt *stmt;

	char sql[] = "UPDATE Usuario SET Password = ? WHERE dni = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 1, contrasenha, strlen(contrasenha),
	SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 2, dni, strlen(dni), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error updating Password usuario\n");
		return result;
	}
	result = sqlite3_finalize(stmt);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement (UPDATE)\n");
		printf("%s\n", sqlite3_errmsg(db));
		return result;
	}
	sqlite3_close(db);
	return 0;
}

Peliculas getAlquileres(char *dni) {
	sqlite3 *db = abrirDB();
	Peliculas p;
	sqlite3_stmt *stmt;

	char sql[] = "SELECT COUNT(*) FROM alquiler WHERE DNI = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);

	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}

	result = sqlite3_bind_text(stmt, 1, dni, strlen(dni),
	SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
	}

	do{
		result = sqlite3_step(stmt);
		if(result == SQLITE_ROW){
			p.setNumPeliculas(sqlite3_column_int(stmt, 0));
		}
	} while(result == SQLITE_ROW);

	result = sqlite3_finalize(stmt);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement (UPDATE)\n");
		printf("%s\n", sqlite3_errmsg(db));
	}


	char sql2[] = "SELECT TITULO_PELI FROM alquiler WHERE DNI = ?";

		int result2 = sqlite3_prepare_v2(db, sql2, strlen(sql2) + 1, &stmt, NULL);
		if (result2 != SQLITE_OK) {
			printf("Error preparing statement\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_close(db);
		}

		result2 = sqlite3_bind_text(stmt, 1, dni, strlen(dni),
		SQLITE_STATIC);
		if (result2 != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
		}

		do{
			result2 = sqlite3_step(stmt);
			if(result2 == SQLITE_ROW){
				for (int i = 0; i < p.getNumPeliculas() - 1; ++i) {
					p.setNombre((char*)sqlite3_column_text(stmt, 0), i);
				}
			}
		} while(result2 == SQLITE_ROW);

		result2 = sqlite3_finalize(stmt);
		if (result2 != SQLITE_OK) {
			printf("Error finalizing statement (UPDATE)\n");
			printf("%s\n", sqlite3_errmsg(db));
		}

	sqlite3_close(db);
	return p;
}
