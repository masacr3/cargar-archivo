#define _GNU_SOURCE
#define _XOPEN_SOURCE
#define TIME_FORMAT "%FT%T%z"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
//catedra consept
#include "strutil.h" 
#include "hash.h" 
#include "abb.h" // modificado
#include "heap.h"
#include "lista.h" 

//prototype

//pre el archivo tiene que estar ordenados segun el regimen de la catedra
bool cargar_archivo(char*,abb_t*);

/*
 * comparacion
 *
 */

int cmp_numeros_i(const int a, const int b);
int cmp_ip_i(const void* ip1,const void* ip2);
int cmp_numeros(const int a, const int b);
int cmp_ip(const char* ip1,const char* ip2);


/*
 *  wrapper para el hash
 */

void eliminar_tiempo(void*);

/*
 *	Dos
 */
 

time_t iso8601_to_time(const char*);

//calcula si el tiempo esta dentro del rango DOS
bool tiempo_sospechoso(char*,char*);

//cargar los tiempos al hash con sus respectivos Ips
void cargar_tiempo(hash_t*,char*,char*); 

//chequea si la lista tiene un ataque DOS
bool dos_attack(lista_t*);

//Verifica los ips con DOS
void DOS(hash_t*); 

/*
 *	Ver visitantes
 *
 */

void VER_VISITANTES(abb_t*, const char*, const char*);
