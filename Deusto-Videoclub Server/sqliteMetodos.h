/*
 * sqliteMetodos.h
 *
 *  Created on: 15 may 2024
 *      Author: Imanol
 */

#ifndef SQLITEMETODOS_H_
#define SQLITEMETODOS_H_

extern "C" {
#include "sqlite3.h"
}
#include "Usuario.h"
#include "Pelicula.h"

sqlite3* abrirDB();
int iniciarSesion(char usuario[], char contrasenha[], Usuario &u);
int passChange(char dni[], char contrasenha[]);
int getNumAlquileres(char dni[]);
int getAlquileres(char dni[], listaPelis &p);
#endif /* SQLITEMETODOS_H_ */
