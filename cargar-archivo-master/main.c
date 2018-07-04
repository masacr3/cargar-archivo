#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
//catedra consept
#include "strutil.h" //splitMEEEE!
#include "hash.h" //DOS
//#include "abb.h" //Ver-visitantes
#include "lista.h" // listas

//prototype
bool prueba(char*);


/*
 *  wrapper para el hash
 */

void eliminar_tiempo(void*);

/*
 *	Dos
 */


void cargar_tiempo(hash_t*,char*,char*);


//bool es_dos(const char*, const char*);

//principal function
//void DOS(const hash_t*);



int main(){
  char* archivo = "access001.log";
  prueba(archivo);
  return 0;
}

bool prueba(char* archivo){
  FILE* f;

  if ( (f = fopen(archivo,"r")) == NULL ) return false;

  char *linea = NULL;
  size_t capacidad = 0;
  ssize_t read;
	size_t leer = 0;
	
	hash_t* hash = hash_crear( eliminar_tiempo );
	
	if ( !hash ){
		fclose(f);
		return false;
	}

  
  while ( ( read = getline(&linea, &capacidad, f) ) != -1 && leer < 5){
    
		leer++;		

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
		
		/*
    char* ip_visitantes = malloc( sizeof(char) * ( tam + 1 ) );

    if (!ip_visitantes){
      fclose(f); //archivo
      free_strv(datos); //datos
      free(linea);//linea
      hash_destruir(hash);
      free(ip_dos);//ip
      return false;
    }
		*/

    char* tiempo = malloc( sizeof(char) * ( strlen(datos[1]) + 1) );

    if (!tiempo){
      fclose(f); //archivo
      free_strv(datos); //datos
      free(linea);//linea
      hash_destruir(hash);
      free(ip_dos);//ip
      //free(ip_visitantes); //ip2
      return false;
    }

    //todo OK creo copias para las siguientes estructuras
    strcpy(ip_dos, datos[0]);
    //strcpy(ip_visitantes, datos[0]);
    strcpy(tiempo, datos[1]);
    
    //fprintf(stdout, "dos: %s | visitantes: %s | tiempo: %s \n", ip_dos, ip_visitantes, tiempo);
    
    cargar_tiempo( hash, ip_dos, tiempo );
    
    //tengo que borrar ya que el hash guarda una copia
    //y me queda colgado UU.
    free(ip_dos);    
    free_strv(datos);
  }
	
	hash_iter_t *it = hash_iter_crear(hash);
	
	while( !hash_iter_al_final(it) ){
		const char* c = hash_iter_ver_actual(it);
		lista_t* l = hash_obtener(hash,c);
		lista_iter_t* it_l = lista_iter_crear(l);
		
		fprintf(stdout,"\n\nip: %s tiempo:", c);
		
		while ( !lista_iter_al_final(it_l) ){
			fprintf(stdout," %s,",(char*)lista_iter_ver_actual(it_l));
			lista_iter_avanzar(it_l);
		}
		
		lista_iter_destruir(it_l);
		hash_iter_avanzar(it);
	}
	hash_iter_destruir(it);
	
	
		

	fprintf(stdout,"\n\n%d\n",(int)leer);	
	
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



