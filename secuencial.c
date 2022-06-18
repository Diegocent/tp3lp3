#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX 20

struct thread_arg
{
    int cantidadThreads; //cantidad de hilos a crear para realizar la busqueda
    int key; //clave a buscar dentro del vector
};

int array[MAX];

int flag = 0; //bandera que indica si esta dentro del array o no 
int posicion=-1; //posicion en donde se encuentra la clave dentro del vector, -1 si no se encuentra
int current_thread = 0;
int nroProceso=0;

void* ThreadSearch(void* pArg) { 
   struct thread_arg *pThreadArg = (struct thread_arg *)pArg;
   int num = current_thread++;
   //printf("nroProceso actual es: %d\n", num);
   int key=pThreadArg->key;
   int cantidadThreads=pThreadArg->cantidadThreads;
   //printf("El numero de Thread actual es %d\n", num);
   //printf("EL rango es %d-%d\n",num * (MAX / cantidadThreads),((num + 1) * (MAX / cantidadThreads)) -1);
   for (int i = num * (MAX / cantidadThreads); i < ((num + 1) * (MAX / cantidadThreads)) && flag == 0 ; i++){
      if (array[i] == key)
      {
         flag = 1; //set flag if key is found
         posicion=i;
         nroProceso=num+1;
         break;
      }
   }
}

int main(int argc, char *argv[])
{

   time_t t;
   srandom(time(&t));
   // Genera semilla para no crear siempre
   // los mismos números aleatorios
   for (int i=0;i<MAX;i++)
   { 
      array[i]=random()%10; // Crea un número aleatorio entre 0 y 9
      //printf("array[%d]=%d\n",i,array[i]);
   }

   //printf("El numero de Threads es %d\n", atoi((char*)argv[2]));
   int cantidadHilos= atoi((char*)argv[2]);
   int key =atoi((char*)argv[1]);
   //printf("El numero a buscar es %d\n", atoi((char*)argv[1]));
   pthread_t thread[cantidadHilos];
   struct thread_arg ThreadArg[cantidadHilos];

   for (int i = 0; i < cantidadHilos; i++)
   { // se pasan los argumenos de la cantidad de hilos y la clave a buscar a un struct
      ThreadArg[i].cantidadThreads=cantidadHilos;
      ThreadArg[i].key=key;
      //se crean multiples hilos
      pthread_create(&thread[i], NULL, ThreadSearch, (void *)&ThreadArg[i]);
   }
   for (int i = 0; i < cantidadHilos; i++) {
      pthread_join(thread[i], NULL); //se espera que todos los procesos terminen
   }
   if (posicion != -1)
      printf("Posicion: %d , Numero de Proceso: %d\n", posicion,nroProceso);
   else
      printf("No esta en el vector\n");
}
