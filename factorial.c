#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t     mutex = PTHREAD_MUTEX_INITIALIZER;

struct thread_arg
{
    int index;
    int limit; //cantidad de hilos maxima
    int numero; //numero para calcular el factorial
    pthread_cond_t *waitCond;
    pthread_cond_t *signalCond;
};

void* threadFun(void* pArg)
{
    struct thread_arg *pThreadArg = (struct thread_arg *)pArg;
    int i=pThreadArg->index, rValue;
    int threadLimit=pThreadArg->limit;
    long long int resultado= 1;
    int n=pThreadArg->numero;
    int num=0;
    for (num= 2; num<= n; num++) 
    {
        resultado= resultado* num;
    }
    for(;i< threadLimit;i += threadLimit)
    {
        rValue = pthread_mutex_lock(&mutex);
       
        printf("Factorial de %d: %lld\n",n,resultado);

        rValue = pthread_cond_signal(pThreadArg->signalCond);
        //espera su turno
        while( pthread_cond_wait(pThreadArg->waitCond, &mutex) != 0 )
        {}
        rValue = pthread_mutex_unlock(&mutex);
    }

    rValue = pthread_cond_signal(pThreadArg->signalCond);
    return NULL;    
}


int main(int argc, char *argv[])
{
    int cantThread=argc-1;
    //printf("Cantidad Threads es %d\n", argc-1);
    pthread_t ThreadId[cantThread];
    struct thread_arg ThreadArg[cantThread];

    pthread_cond_t  cond[cantThread];//  = PTHREAD_COND_INITIALIZER;

    for (int i =0; i< cantThread ; i++)
        pthread_cond_init(&cond[i], NULL);
    
    for (int i =0; i< cantThread ; i++)
    {
        ThreadArg[i].limit = cantThread;
        ThreadArg[i].index = i;
        ThreadArg[i].numero= atoi((char*)argv[i+1]);
        ThreadArg[i].waitCond = &cond[i];
        if (i == cantThread-1)
            ThreadArg[i].signalCond = &cond[0];
        else
            ThreadArg[i].signalCond = &cond[i+1];

        pthread_create(&ThreadId[i], NULL, &threadFun,(void*)&ThreadArg[i]);
        usleep(500);
    }
    for (int i =0; i< cantThread ; i++)
        pthread_join(ThreadId[i],NULL);

    return 0;
}
