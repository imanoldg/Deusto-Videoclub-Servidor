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
#include "Pelicula.h"
#include "string.h"
#include <iostream>
using namespace std;

sqlite3* abrirDB() {

	sqlite3 *db;

	int result = sqlite3_open("UserDB.db", &db);

	if (result != SQLITE_OK) {
		cout << "Error abriendo la base de datos" << endl;
	}

	return db;
}

int iniciarSesion(char usuario[], char contrasenha[], Usuario &u) {
	sqlite3 *db = abrirDB();
	int result;
	sqlite3_stmt *stmt;

	//COMPROBAR QUE LA CONTRASEÑA DEL USUARIO INTRODUCIDO ES CORRECTA
	char sql[] = "SELECT * FROM USUARIO WHERE User = ?";

	result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 1, usuario, strlen(usuario),
	SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);
	if (result == SQLITE_ROW) {
		if (strcmp(usuario, (char*) sqlite3_column_text(stmt, 5)) == 0
				&& strcmp(contrasenha, (char*) sqlite3_column_text(stmt, 6))
						== 0) {
			cout << "USUARIO Y CONTRASEÑA CORRECTOS" << endl;
			cout << endl;
			cout << "SESIÓN INICIADA" << endl;

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
			u.setNumTarjeta((int) sqlite3_column_int(stmt, 9));
			u.setPuntos((int) sqlite3_column_int(stmt, 10));
			return 1;

			sqlite3_finalize(stmt);
			sqlite3_close(db);

		}

	} else if (result != SQLITE_DONE) {
		printf("ERROR INICIANDO SESION\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		//CERRAR BASE DE DATOS
		sqlite3_close(db);
		return 0;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;

}

int passChange(char dni[], char contrasenha[]) {
	sqlite3 *db = abrirDB();

	sqlite3_stmt *stmt;

	char sql[] = "UPDATE usuario SET Password = ? WHERE DNI = ?";

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

int getNumAlquileres(char dni[]) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;

	int numPelis = 0;

	char sql[] = "SELECT COUNT(*) FROM alquiler WHERE DNI = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
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

	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) {
			numPelis = sqlite3_column_int(stmt, 0);
		}
	} while (result == SQLITE_ROW);

	result = sqlite3_finalize(stmt);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement (UPDATE)\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
	}

	sqlite3_close(db);
	return numPelis;
}

int getAlquileres(char dni[], listaPelis &p) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;

	char sql2[] = "SELECT TITULO_PELI FROM alquiler WHERE DNI = ?";

	cout << "Seleccionando peliculas..." << endl;

	int result = sqlite3_prepare_v2(db, sql2, strlen(sql2) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
	}

	result = sqlite3_bind_text(stmt, 1, dni, strlen(dni) + 1,
	SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
	}

	int contador = 0;
	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) {
			cout << (char*) sqlite3_column_text(stmt, 0) << endl;
			p.pelis[contador].setNombre((char*) sqlite3_column_text(stmt, 0));
			cout << p.pelis[contador].getNombre() << endl;
			contador++;
		} else{
			printf("Error\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
		}
	} while (result == SQLITE_ROW);

	result = sqlite3_finalize(stmt);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement SELECT\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);

	}

	sqlite3_close(db);
	return 0;
}

int updatePuntos(char dni[], int numPuntos) {
	sqlite3 *db = abrirDB();

	sqlite3_stmt *stmt;

	char sql[] = "UPDATE usuario SET PUNTOS = ? WHERE DNI = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_int(stmt, 1, numPuntos);
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
		printf("Error updating Puntos usuario\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}
	result = sqlite3_finalize(stmt);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement (UPDATE)\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}
	sqlite3_close(db);
	return 0;
}

