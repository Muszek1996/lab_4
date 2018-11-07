#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include<pthread.h>
#include <unistd.h>
#include "pomiar_czasu/pomiar_czasu.h"


void * watek_klient (void * arg);


int l_kf,l_kr;
pthread_mutex_t mutexKufle;
pthread_mutex_t mutexKran;

void main(){


  pthread_t *tab_klient;
  int *tab_klient_id;

  int l_kl , i;

  pthread_mutex_init(&mutexKufle, NULL);
  pthread_mutex_init(&mutexKran, NULL);

  printf("Liczba klientow: "); scanf("%d", &l_kl);

  printf("Liczba kufli: "); scanf("%d", &l_kf);

  //printf("\nLiczba kranow: "); scanf("%d", &kran);
  l_kr = 1;

  tab_klient = (pthread_t *) malloc(l_kl*sizeof(pthread_t));
  tab_klient_id = (int *) malloc(l_kl*sizeof(int));
  for(i=0;i<l_kl;i++) tab_klient_id[i]=i;



  inicjuj_czas();
  printf("Otwieramy pub!\n");
  printf("Liczba wolnych kufli %d\n", l_kf);

  for(i=0;i<l_kl;i++){

    pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]);
  }
  for(i=0;i<l_kl;i++){
    pthread_join( tab_klient[i], NULL);
  }
  printf("Zamykamy pub!\n");
  drukuj_czas();

}


void * watek_klient (void * arg_wsk){

  int moj_id = *((int *)arg_wsk);

  int i, j, kufel, result;
  int ile_musze_wypic = 2;

  printf("Klient %d, wchodze do pubu\n", moj_id);
    
  for(i=0; i<ile_musze_wypic; i++){


  int flag = 0;

    while(!flag){
      pthread_mutex_lock(&mutexKufle);


      if(l_kf>0){

          printf("Klient %d, pobieram kufel nr:%d \n", moj_id, l_kf--);
          flag=1;
        }
      else{
        printf("Klient %d,brak kufli, czekam\n", moj_id);
        usleep(1000000);
      }

      pthread_mutex_unlock(&mutexKufle);
    }






    pthread_mutex_lock(&mutexKran);
    printf("Klient %d, nalewam z kranu moj %d kufel \n", moj_id, i+1);
    usleep(3000);
    pthread_mutex_unlock(&mutexKran);


    printf("Klient %d, pije\n", moj_id);
    usleep(1000000);


    pthread_mutex_lock(&mutexKufle);
    printf("Klient %d, odkladam kufel na stole teraz jest: %d + 1 oddawany\n", moj_id, l_kf);
    ++l_kf;
    pthread_mutex_unlock(&mutexKufle);

    
  }

  printf("Klient %d, wychodze z pubu\n", moj_id);
    
  return(NULL);
} 


