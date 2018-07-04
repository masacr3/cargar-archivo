#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
//catedra consept
#include "strutil.h" //splitMEEEE!
#include "hash.h" //DOS
#include "abb.h" //Ver-visitantes
#include "lista.h" // listas_ip

#define CANT_LINE 20

//prototype
bool prueba(char*);

/*
 *  wrapper para el hash
 */

void eliminar_tiempo(lista_t*);

/* DOS functions */
ve
bool es_dos(const char*, const char*);

//principal function
void DOS(const hash_t*);



int main(){
  char* archivo = "access001.log";
  prueba(archivo);
  return 0;
}

bool prueba(char* archivo){
  FILE* f;

  if ( (f = fopen(archivo),"r") == NULL ) return false;

  char *linea = NULL;
  size_t capacidad = 0;
  ssize_t read;
  size_t leer = 0;
  
  while ( ( read = getline(&linea, &capacidad, f) ) != -1 && leer < CANT_LINE ){
    //off passed
    leer++;
    
    char** datos = split(linea,'\t');

    if(!datos){
      fclose(f); //archivo
      free(linea);
      return false;
    }

    size_t tam = strlen(datos[0]);
    char* ip_dos = malloc( sizeof(char) * ( tam + 1 ) );

    if (!ip_dos){
      fclose(f); //archivo
      free_strv(datos); //datos
      free(linea);//linea
      return false;
    }

    char* ip_visitantes = malloc( sizeof(char) * ( tam + 1 ) );

    if (!ip_visitantes){
      fclose(f); //archivo
      free_strv(datos); //datos
      free(linea);//linea
      free(ip_dos);//ip
      return false;
    }

    char* tiempo = malloc( sizeof(char) * ( strlen(datos[1]) + 1) );

    if (!tiempo){
      fclose(f); //archivo
      free_strv(datos); //datos
      free(linea);//linea
      free(ip_dos);//ip
      free(ip_visitantes); //ip2
      return false;
    }

    //todo OK creo copias para las siguientes estructuras
    strcpy(ip_dos, datos[0]);
    strcpy(ip_visitantes, datos[0]);
    strcpy(tiempo, datos[1]);
    
    fprintf(stdout, "dos: %s | visitantes: %s | tiempo: %s \n", ip_dos, ip_visitantes, tiempo);
    
    //off passed
    free(ip_dos);
    free(ip_visitantes);
    free(tiempo)
    
    free_strv(datos);
  }
  return true;
}


void eliminar_tiempo(lista_t* lista){
    lista_destruir(lista,free);
}
