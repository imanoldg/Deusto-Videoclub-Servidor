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
#include "string.h"
#include <iostream>
using namespace std;

sqlite3* abrirDB() {

	sqlite3 *db;

	int result = sqlite3_open("User.db", &db);

	if (result != SQLITE_OK) {
		cout << "Error abriendo la base de datos" << endl;
	}

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

				if(sqlite3_column_int(stmt, 7) == 1){
					u.setGenero('M');
				} else{
					u.setGenero('F');
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
		return 0;
	}



}
