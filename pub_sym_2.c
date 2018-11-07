#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include<pthread.h>
#include <unistd.h>

struct struktura_t { 
  int *kufel;
  int l_kf;
  int l_kr;
  pthread_mutex_t *tab_kuf; 
  int *kran;
  pthread_mutex_t *tab_kran;  
};
struct struktura_t pub_wsk;


void * watek_klient (void * arg);

main(){
  pthread_t *tab_klient;
  int *tab_klient_id;

  int l_kl, l_kf, l_kr, i;

  printf("\nLiczba klientow: "); scanf("%d", &l_kl);

  printf("\nLiczba kufli: "); scanf("%d", &l_kf);
  pub_wsk.kufel = l_kf;


  printf("\nLiczba kranow: "); scanf("%d", &l_kr);
  pub_wsk.kran = l_kr;

  tab_klient = (pthread_t *) malloc(l_kl*sizeof(pthread_t));
  tab_klient_id = (int *) malloc(l_kl*sizeof(int));
  pub_wsk.kufel = (int *) malloc(l_kf*sizeof(int));
  pub_wsk.kran = (int *) malloc(l_kr*sizeof(int));

  for(i=0;i<l_kl;i++) tab_klient_id[i]=i;

  pub_wsk.tab_kuf = (pthread_mutex_t *) malloc(l_kf*sizeof(pthread_mutex_t));
  for(i=0;i<l_kf;i++) {
    pthread_mutex_init(&pub_wsk.tab_kuf[i], NULL);
    pub_wsk.kufel[i] = 1;
  }

  pub_wsk.tab_kran = (pthread_mutex_t *) malloc(l_kr*sizeof(pthread_mutex_t));
  for(i=0;i<l_kr;i++) {
    pthread_mutex_init(&pub_wsk.tab_kran[i], NULL);
    pub_wsk.kran[i]= 1;
  }

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

  int moj_id = * ((int *)arg_wsk);

  int i, kran, kufel, result;

  int ile_musze_wypic = 2;

  printf("\nKlient %d, wchodzę do pubu\n", moj_id); 
    
  for(i=0; i<ile_musze_wypic; i++){

    printf("\nKlient %d, wybieram kufel\n", moj_id);

    int j = 0;

    int flag = 0;

    while(!flag){
      for(int i=0;i<pub_wsk.l_kf;i++)
      if(!pthread_mutex_trylock(&pub_wsk.tab_kuf[i])){
        printf("\nKlient %d, wybieram kufel nr:%d\n", moj_id,i);
        pub_wsk.kufel[i] = 0;
        pthread_mutex_unlock(&pub_wsk.tab_kuf[i]);
        for(int j=0;j<pub_wsk.l_kr;j++){
          if(!pthread_mutex_trylock(&pub_wsk.tab_kran[j])){
            printf("\nKlient %d, nalewam do kufla\n", moj_id);
            pub_wsk.kran[j]=0;
            pthread_mutex_unlock(&pub_wsk.tab_kran[j]);
            printf("\nKlient %d, pije\n", moj_id);

          }else{
            printf("\nKlient %d, brak wolnych kranów\n", moj_id);
            usleep(10000);
          }

        }

      }else{
        printf("\nKlient %d, brak wolnych kufli\n", moj_id);
        usleep(10000);
      }

    }

    // printf("\nKlient %d, wybrałem kufel %d\n", moj_id, kufel); ?????

    printf("\nKlient %d, wybieram kran\n", moj_id); 

    // ????????????
    
    printf("\nKlient %d, nalewam z kranu %d\n", moj_id, kran); 
    sleep(1);

    // ????????????
    if(kran==0)  printf("\nKlient %d, pije piwo Guinness\n", moj_id); 
    else if(kran==1)  printf("\nKlient %d, pije piwo Żywiec\n", moj_id); 
    else if(kran==2)  printf("\nKlient %d, pije piwo Heineken\n", moj_id); 
    else if(kran==3)  printf("\nKlient %d, pije piwo Okocim\n", moj_id); 
    else if(kran==4)  printf("\nKlient %d, pije piwo Karlsberg\n", moj_id); 
    // itd., itp. 

    sleep(1);

    // ????????????

    printf("\nKlient %d, odkladam kufel %d\n", moj_id, kufel); 

  }

  return(NULL);
} 
