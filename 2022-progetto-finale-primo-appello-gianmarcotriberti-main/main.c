#include <stdio.h>
#include "gamelib.h"

int main(){
  system("clear");
  printf("Benvenuto in...\n\n\n");
  printf("88888888888 8888888b.         d8888  .d8888b.  8888888  .d8888b.\n");  
  printf("    888     888   Y88b       d88888 d88P  Y88b   888   d88P  Y88b\n");
  printf("    888     888    888      d88P888 888    888   888   888    888\n");
  printf("    888     888   d88P     d88P 888 888          888   888       \n");
  printf("    888     8888888P'     d88P  888 888  88888   888   888       \n");
  printf("    888     888 T88b     d88P   888 888    888   888   888    888\n");
  printf("    888     888  T88b   d8888888888 Y88b  d88P   888   Y88b  d88P\n");
  printf("    888     888   T88b d88P     888  'Y8888P88 8888888  'Y8888P' \n");
                                                                 

  int scelta = 0; //variabile per la scelta
  do{
    printf("\nMENÙ:\n"); //MENU
    printf("Inserisci 1 per impostare il gioco\n");
    printf("Inserisci 2 per combattere\n");
    printf("Inserisci 3 per terminare la partita\n");
    printf("Inserisci 4 per visualizzare le regole del gioco\n");
    scelta = 0;
    printf("La tua scelta: ");
    scanf("%d", &scelta);
    while(getchar()!= '\n'); //Pulisco buffer
    switch (scelta){
      case 1:{
        imposta_gioco();
        break;
      }
      case 2:{
        combatti();
        break;
      }
      case 3:{
        termina_gioco();
        break;
      }
      case 4:{
        stampa_regole();
        break;
      }
      default:{
        printf("Input invalido.\n");
      }
    }printf("------------------------------------------------------");
  }while(scelta != 3);

}
