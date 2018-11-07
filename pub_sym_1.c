#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include<pthread.h>
#include <unistd.h>


void * watek_klient (void * arg);


int l_kf,l_kr;
pthread_mutex_t mutexKufle;
pthread_mutex_t mutexKran;

main(){


  pthread_t *tab_klient;
  int *tab_klient_id;

  int l_kl , i;

  pthread_mutex_init(&mutexKufle, NULL);
  pthread_mutex_init(&mutexKran, NULL);

  printf("\nLiczba klientow: "); scanf("%d", &l_kl);

  printf("\nLiczba kufli: "); scanf("%d", &l_kf);

  //printf("\nLiczba kranow: "); scanf("%d", &kran);
  l_kr = 1;

  tab_klient = (pthread_t *) malloc(l_kl*sizeof(pthread_t));
  tab_klient_id = (int *) malloc(l_kl*sizeof(int));
  for(i=0;i<l_kl;i++) tab_klient_id[i]=i;


  printf("\nOtwieramy pub (simple)!\n");
  printf("\nLiczba wolnych kufli %d\n", l_kf); 

  for(i=0;i<l_kl;i++){

    pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]);
  }
  for(i=0;i<l_kl;i++){
    pthread_join( tab_klient[i], NULL);
  }
  printf("\nZamykamy pub!\n");


}


void * watek_klient (void * arg_wsk){

  int moj_id = *((int *)arg_wsk);

  int i, j, kufel, result;
  int ile_musze_wypic = 2;

  printf("\nKlient %d, wchodzę do pubu\n", moj_id); 
    
  for(i=0; i<ile_musze_wypic; i++){


  int flag = 0;

    while(!flag){
      pthread_mutex_lock(&mutexKufle);


      if(l_kf>0){
          --l_kf;
          printf("\nKlient %d, pobieram kufel:%d \n", moj_id, l_kf);
          flag=1;
        }
      else{
        printf("\nKlient %d,brak kf, czekam\n", moj_id);
        usleep(10000);
      }

      pthread_mutex_unlock(&mutexKufle);
    }






    pthread_mutex_lock(&mutexKran);
    printf("\nKlient %d, nalewam z kranu mój kufel nr:%d z kranu : %d\n", moj_id, i+1, l_kr);
    usleep(300);
    pthread_mutex_unlock(&mutexKran);


    printf("\nKlient %d, pije\n", moj_id); 
    nanosleep((struct timespec[]){{0, 500000000L}}, NULL);


    pthread_mutex_lock(&mutexKufle);
    printf("\nKlient %d, odkladam kufel na stole: %d + 1 oddawany", moj_id, l_kf);
    ++l_kf;
    pthread_mutex_unlock(&mutexKufle);
    
  }

  printf("\nKlient %d, wychodzę z pubu\n", moj_id); 
    
  return(NULL);
} 


