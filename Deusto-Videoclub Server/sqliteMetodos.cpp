/*
 * sqliteMetodos.cpp
 *
 *  Created on: 15 may 2024
 *      Author: Usuario
 */
extern "C" {
#include "sqlite3.h"
}
#include "Usuario.h"
#include "string.h"
#include <iostream>
using namespace std;

sqlite3* abrirDB(){

	sqlite3* db;

	int result = sqlite3_open("User.db", &db);

	if(result != SQLITE_OK){
		cout << "Error abriendo la base de datos" << endl;
	}

	return db;
}

int iniciarSesion(char* usuario, char* contrasenha, Usuario& u){
	sqlite3* db = abrirDB();

	int result;

	sqlite3_stmt *stmt;

	char*
}
