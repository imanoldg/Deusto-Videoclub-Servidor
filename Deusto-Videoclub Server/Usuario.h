/*
 * Usuario.h
 *
 *  Created on: 15 may 2024
 *      Author: Usuario
 */

#ifndef USUARIO_H_
#define USUARIO_H_

class Usuario{
private:
	char dni[10];
	char nombre[40];
	char apellido[60];
	char email[60];
	int telefono;
	char user[40];
	char contra[16];
	char genero[1];
	char* fechaNcto;
	int numTarjeta;
	int puntos;
	
public:
	Usuario(char* dni, char* nombre, char* apellido, char* email, int telefono, char* user, char* contra, char* genero, char* fechaNcto, int numTarjeta, int puntos);
	Usuario(Usuario &u);
	~Usuario();

	char* getDNI();
	char* getNombre();
	char* getApellido();
	char* getEmail();
	int getTlf();
	char* getUser();
	char* getContra();
	char* getGenero();
	char* getFechaNcto();
	int getNumTarjeta();
	int getPuntos();
	void setPuntos(int puntos);
};

#endif /* USUARIO_H_ */
