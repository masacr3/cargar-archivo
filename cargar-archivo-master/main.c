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
#include "strutil.h" //splitMEEEE!
#include "hash.h" //DOS
#include "abb.h" //Ver-visitantes
#include "lista.h" // listas

//prototype
bool cargar_archivo(char*,abb_t*);


/*
 *  wrapper para el hash
 */

void eliminar_tiempo(void*);

/*
 *	Dos
 */
 

time_t iso8601_to_time(const char*); //ok

//calcula si el tiempo esta dentro del rango DOS
bool tiempo_sospechoso(char*,char*); //ok

//cargar los tiempos al hash con sus respectivos Ips
void cargar_tiempo(hash_t*,char*,char*); //ok

//chequea si la lista tiene un ataque DOS
bool dos_attack(lista_t*);//ok

//Verifica los ips con DOS
void DOS(hash_t*); // agregar el heap


int main(){
  char* archivo = "access002.log";
  abb_t* visitantes = abb_crear(cmp_ip);
  cargar_archivo(archivo, abb);
  return 0;
}


bool cargar_archivo(char* archivo, abb_t* visitantes){
  FILE* f;

  if ( (f = fopen(archivo,"r")) == NULL ) return false;

  char *linea = NULL;
  size_t capacidad = 0;
  ssize_t read;
	
	hash_t* hash = hash_crear( eliminar_tiempo );
	
	if ( !hash ){
		fclose(f);
		return false;
	}

  
  while ( ( read = getline(&linea, &capacidad, f) ) != -1 ){		

    char** datos = split(linea,'\t');

    if(!datos){
      fclose(f); //archivo
      hash_destruir(hash);
      free(linea);
      return false;
    }

    size_t tam = strlen(datos[0]);
    char* ip_dos = malloc( sizeof(char) * ( tam + 1 ) );

    if (!ip_dos){
      fclose(f); //archivo
      free_strv(datos); //datos
      hash_destruir(hash);
      free(linea);//linea
      return false;
    }
		
		
    char* ip_visitantes = malloc( sizeof(char) * ( tam + 1 ) );

    if (!ip_visitantes){
      fclose(f); //archivo
      free_strv(datos); //datos
      free(linea);//linea
      hash_destruir(hash);
      free(ip_dos);//ip
      return false;
    }
		

    char* tiempo = malloc( sizeof(char) * ( strlen(datos[1]) + 1) );

    if (!tiempo){
      fclose(f); //archivo
      free_strv(datos); //datos
      free(linea);//linea
      hash_destruir(hash);
      free(ip_dos);//ip
      free(ip_visitantes); //ip2
      return false;
    }

    //todo OK creo copias para las siguientes estructuras
    strcpy(ip_dos, datos[0]);
    
    strcpy(ip_visitantes, datos[0]);
    
    strcpy(tiempo, datos[1]);
   
    cargar_tiempo( hash, ip_dos, tiempo );
    
    abb_guardar(visitantes, ip_visitantes, NULL);
    
    //tengo que borrar ya que el hash guarda una copia
    //y me queda colgado UU.
    free(ip_visitantes)
    free(ip_dos);    
    free_strv(datos);
  }
	
	DOS(hash);
	
	//RESULT
	fprintf(stdout,"%s","OK");
	

	fclose(f);
	free(linea);
	hash_destruir(hash);
	
  return true;
}

/*wrapper*/
void eliminar_tiempo(void* l){
	lista_destruir(l,free);
}

//DOS

void cargar_tiempo(hash_t* hash, char* ip, char* tiempo){
	
	bool esta = hash_pertenece( hash, ip );
	
	if ( esta ){
		lista_t* lista = hash_obtener( hash, ip );
		lista_insertar_ultimo( lista, tiempo );
	}
	else{
		lista_t* lista = lista_crear();
		
		if( !lista ) return;
		
		lista_insertar_ultimo( lista, tiempo );
		hash_guardar( hash, ip, lista );
	}
	
}

bool tiempo_sospechoso( char* inicio, char* fin){
	time_t i = iso8601_to_time(inicio);
	time_t f = iso8601_to_time(fin);
	return ( difftime(f,i) < 2.0 );
}


bool dos_attack( lista_t* lista_tiempo ){

	// si esta vacio o el largo de tiempos es menor a 5 no hay ataque
	if ( lista_esta_vacia(lista_tiempo) || lista_largo(lista_tiempo) < 5 ) return false;

	lista_iter_t* primero = lista_iter_crear(lista_tiempo);

	if (!primero) return false;

	lista_iter_t* ultimo = lista_iter_crear(lista_tiempo);

	if (!ultimo){
		lista_iter_destruir(primero);
		return false;
	}

	bool es_DOS = false;

	//avanzo hasta la 5ta posicion
	for (int i=0; i < 5; i++) lista_iter_avanzar(ultimo);

	while ( !lista_iter_al_final(ultimo) ){
		char* inicio = lista_iter_ver_actual(primero);
		char* fin = lista_iter_ver_actual(ultimo);

		es_DOS = tiempo_sospechoso(inicio,fin);

		if ( es_DOS ) break;
		
		lista_iter_avanzar(primero);
		lista_iter_avanzar(ultimo);	
	}

	lista_iter_destruir(primero);
	lista_iter_destruir(ultimo);

	return es_DOS;
}


void DOS (hash_t* hash){

	heap_t* heap = heap_crear(cmp_ip);
	
	if(!heap) return;
		
	
	hash_iter_t* iter = hash_iter_crear(hash);

	if (!iter) return ;
	
	//recorro todo el hash verificando el ataque
	while ( !hash_iter_al_final(iter) ){
		const char* ip = hash_iter_ver_actual(iter);
		lista_t* lista_tiempos = hash_obtener(hash,ip);
		
		//hay ataque
		if ( dos_attack(lista_tiempos) ) {
			char* copy_ip = malloc( sizeof(char) * ( strlen(ip) + 1 ) );
			
			if ( !copy_ip ){
				hash_iter_destruir(iter);
				return;
			}
			
			strcpy(copy_ip,ip);
			
			heap_encolar(heap, copy_ip);
			
		}
		hash_iter_avanzar(iter);
	}
	
	while ( !heap_esta_vacio(heap) ){
		char* ip = heap_desencolar(heap);
		fprintf(stdout,"DoS: %s\n",ip);
		free(ip);
	}
	
	heap_destruir(heap, NULL);
	hash_iter_destruir(iter);

}

time_t iso8601_to_time(const char* iso8601)
{
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
}
