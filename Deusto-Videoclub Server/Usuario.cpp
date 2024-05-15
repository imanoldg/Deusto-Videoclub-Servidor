/*
 * Usuario.cpp
 *
 *  Created on: 15 may 2024
 *      Author: Usuario
 */
#include "Usuario.h"
#include <iostream>
#include "string.h"
using namespace std;


Usuario::Usuario(char* dni, char* nombre, char* apellido, char* email, int telefono, char* user, char* contra, char* genero, char* fechaNcto, int numTarjeta, int puntos){
	strcpy(this->dni, dni);
	strcpy(this->nombre, nombre);
	strcpy(this->apellido, apellido);
	strcpy(this->email, email);
	this->telefono = telefono;
	strcpy(this->user, user);
	strcpy(this->contra, contra);
	strcpy(this->genero, genero);
	this->fechaNcto = new char(strlen(fechaNcto) + 1);
	strcpy(this->fechaNcto, fechaNcto);
	this->numTarjeta = numTarjeta;
	this->puntos = puntos;
}

Usuario::Usuario(Usuario &otro){
	strcpy(this->dni, otro.dni);
	strcpy(this->nombre, otro.nombre);
	strcpy(this->apellido, otro.apellido);
	strcpy(this->email, otro.email);
	this->telefono = otro.telefono;
	strcpy(this->user, otro.user);
	strcpy(this->contra, otro.contra);
	strcpy(this->genero, otro.genero);
	this->fechaNcto = new char(strlen(otro.fechaNcto) + 1);
	strcpy(this->fechaNcto, otro.fechaNcto);
	this->numTarjeta = otro.numTarjeta;
	this->puntos = otro.puntos;
}

Usuario::~Usuario(){
	delete[] dni;
	delete[] nombre;
	delete[] apellido;
	delete[] email;
	delete[] user;
	delete[] contra;
	delete[] genero;
	delete[] fechaNcto;
}


char* Usuario::getDNI(){
	return this->dni;
}

char* Usuario::getNombre(){
	return this->nombre;
}

char* Usuario::getApellido(){
	return this->apellido;
}

char* Usuario::getEmail(){
	return this->email;
}

char* Usuario::getUser(){
	return this->user;
}

int Usuario::getTlf(){
	return this->telefono;
}

char* Usuario::getContra(){
	return this->contra;
}

char* Usuario::getGenero(){
	return this->genero;
}

char* Usuario::getFechaNcto(){
	return this->fechaNcto;
}

int Usuario::getNumTarjeta(){
	return this->numTarjeta;
}

int Usuario::getPuntos(){
	return this->puntos;
}

void Usuario::setPuntos(int puntos){
	this->puntos = puntos;
}
