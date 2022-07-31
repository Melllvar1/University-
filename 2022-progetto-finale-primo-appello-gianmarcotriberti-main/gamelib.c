#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "gamelib.h"

//VARIABILI STATICHE
static int lunghezza_mazzo;
static struct Mago mago_1;
static struct Mago mago_2;
static int conta_imposta_gioco = 0;  //PERMETTE DI VERIFICARE SE SI È IMPOSTATO IL GIOCO ALMENO UNA VOLTA
static int vittoria = 0; //NEL CASO 1, INTERROMPE IL PASSAGGIO DEL TURNO E TERMINA IL COMBATTIMENTO
static int cantatore_mazzo_1 = 0; //QUESTE TRE VARIABILI CONTATORE SONO UTILIZZATE PER GENERARE GLI HP DELLE CREATURE IN BASE ALLA LUNGHEZZA DEL MAZZO
static int cantatore_mazzo_2 = 0;
static int cantatore_mani = 0;
static int static_pesca = 0;  //QUESTE TRE VARIABILI STATIC VENGONO UTILIZZATE PER IMPEDIRE CHE IL GIOCATORE EFFETTUI PIÙ VOLTE LA STESSA AZIONE
static int static_gioca = 0;
static int static_attacca = 0;
static int inizio_game = 0;  //QUESTA VARIABILE VIENE UTILIZZATA NELLA FUNZIONE PASSA PER GESTIRE IL PASSAGGIO DEL TURNO (SE == A 0--> MAGO1; SE == 1--> MAGO2 )


// DICHIARAZIONI FUNZIONI STATICHE
static void inserisci_carta_coda(struct Mago *pmago);   //RIGA 237
static int genera_tipo(void); //RIGA 255
static int scelta_classe_mago(void); //RIGA 208
static void stampa_mazzo(struct Mago* pmago); //RIGA 403
static int  genera_hp(); //RIGA 273
static int genera_hp_classe(struct Mago* pmago, struct Carta* pcarta); //RIGA 342
static void crea_mano(struct Mago*pmago); //RIGA 355
static void stampa_mano(struct Mago* pmago); //RIGA 366
static void stampa_campo(struct Mago*); //RIGA 388
static void crea_campo(struct Mago* pmago); //RIGA 381
static const char* converti_in_tipo(int temp); //RIGA 424
const char* converti_in_classe(int temp); // RIGA 526
static void switch_gioca(struct Mago* pmago, struct Mago* pmago_2); //RIGA 436
static void pesca(struct Mago*); //RIGA 537
static void rimuovi_in_coda(struct Mago*); //RIGA 556
static void gioca_carta(struct Mago*, struct Mago*); //RIGA 581
static void riordina_mano(struct Mago*); //RIGA 853
static void attacca(struct Mago*, struct Mago*); //RIGA 756
static void controlla_vittoria(struct Mago*, struct Mago*); //RIGA 873
static void free_mazzo(struct Mago*); //RIGA 897
static void passa(); //RIGA 906
static int conta_carte_rimaste(struct Mago*); //RIGA 917



void imposta_gioco(){
  time_t t;
  srand((unsigned)time(&t));   //PER LA CREAZIONE DI NUMERI CASUALI
  do{
    if(conta_imposta_gioco == 0){  //
      conta_imposta_gioco++; 
      printf("\nInserisci il nome del mago 1\n");
      fgets(mago_1.nome_mago, 256, stdin);       //FGETS PER PRENDERE L'INPUT DEL NOME
      mago_1.classe = scelta_classe_mago();      // CHIAMO LA FUNZIONE RIGA 208
      printf("------------------------------------------------------");
      printf("\nInserisci il nome del mago 2\n");
      fgets(mago_2.nome_mago, 256, stdin);       //FGETS PER PRENDERE L'INPUT DEL NOME
      mago_2.classe = scelta_classe_mago();      // CHIAMO LA FUNZIONE RIGA 208
      mago_1.punti_vita = 20;  //INIZIALIZZO PV A 20
      mago_2.punti_vita = 20;
      mago_1.inizio_mazzo = NULL;  //INIZIALIZZO INIZIO E FINE MAZZO A NULL
      mago_1.fine_mazzo = NULL;
      mago_2.inizio_mazzo = NULL;
      mago_2.fine_mazzo = NULL;
      printf("------------------------------------------------------");
      printf("\nScegli il numero di carte per il mazzo:\n");
      int controllo = 0;  //VARIABILE UTILIZZATA PER IL LOOP 
      do{
        printf("La tua scelta: ");
        scanf("%d", &lunghezza_mazzo);
        while(getchar() != '\n');
        if(lunghezza_mazzo == 0){   //SE LA LUNGHEZZA DEL MAZZO È 0 OPPURE VENGONO INSERITI CARATTERI NON NUMERI
          printf("Impossibile creare mazzo. Inserisci una lunghezza valida.\n");
        }else{
        printf("------------------------------------------------------");

        for(int i = 0; i < lunghezza_mazzo; i++){     //FOR CHE RIPETE IN RELAZIONE ALLA LUNGHEZZA DEL MAZZO
        inserisci_carta_coda(&mago_1);  //CREA IL MAZZO   RIGA 237
        }

        for(int i = 0; i < lunghezza_mazzo; i++){    //FOR CHE RIPETE IN RELAZIONE ALLA LUNGHEZZA DEL MAZZO
        inserisci_carta_coda(&mago_2);  //CREA MAZZO    RIGA 237
        }

        printf("\nStampo mazzo 1:\n");
        stampa_mazzo(&mago_1); //RIGA 403
        printf("\nStampo mazzo 2:\n");
        stampa_mazzo(&mago_2); 
        crea_mano(&mago_1); //CREO MANO RIGA 355
        crea_mano(&mago_2); 
        printf("\nStampo mano %s", mago_1.nome_mago);
        stampa_mano(&mago_1); //RIGA 366
        printf("\nStampo mano %s", mago_2.nome_mago);
        stampa_mano(&mago_2); 
        printf("\n");
        crea_campo(&mago_1); //RIGA 381
        crea_campo(&mago_2); 
        controllo = 1;
        }
      }while(controllo == 0);
    }
    else{ //LIBERA LA MEMORIA DINAMICA
      free_mazzo(&mago_1);   //FREE DI ENTRAMBI I MAZZI
      free_mazzo(&mago_2);

      for(int i = 0; i <6; i++){ //FREE DI ENTRAMBE LE MANI
        free(mago_1.mano[i]);
        mago_1.mano[i] = NULL;
      }

      for(int i = 0; i <6; i++){
        free(mago_2.mano[i]);
        mago_2.mano[i] = NULL;
      }

      for(int i = 0; i <4; i++){  //FREE DI ENTRAMBI I CAMPI
        free(mago_1.campo[i]);
        mago_1.campo[i] = NULL;
      }

      for(int i = 0; i <4; i++){
        free(mago_2.campo[i]);
        mago_2.campo[i] = NULL;
      }
      
      conta_imposta_gioco = 0;
      cantatore_mazzo_1 = 0;    //AZZERAMENTO VARIABILI GLOBALI
      cantatore_mazzo_2 = 0;
      cantatore_mani = 0;
      vittoria = 0;
      static_attacca = 0;
      static_gioca = 0;
      static_pesca = 0;
    }
  }while(conta_imposta_gioco == 0);  //IL LOOP DOPO AVER LIBERATO LA MEMORIA INIZIA L'IMPOSTAZIONE DEL GIOCO
}


void combatti(){
  system("clear");  
  if(conta_imposta_gioco != 0 && conta_imposta_gioco != 2){  //CONTROLLO SE È STATO IMPOSTATO IL GIOCO 
    inizio_game = rand() % 2; //GENERO NUMERO RANDOM DA 0 A 1 PER SCEGLIERE IL PRIMO GIOCATORE
    while(vittoria == 0){  //LOOP FINO A CHE LA VARIABILE GLOBALE VITTORIA DIVENTA 1
      if (inizio_game == 0){   //GIOCATORE 1
        printf("------------------------------------------------------\n");
        switch_gioca(&mago_1, &mago_2); //RIGA 436
      }else{ //GIOCATORE 2
        printf("------------------------------------------------------\n");
        switch_gioca(&mago_2, &mago_1);
      }
    }
  conta_imposta_gioco = 2;  //VARIABILE GLOBALE CHE VERIFICA SE SI HA GIOCATO
  }else{
    printf("Impossibile cominciare a giocare senza aver impostato il gioco.\n"); //CASO IN CUI LA VARIABILE CONTA_IMPOSTA GIOCO == 0
    return;
  }
}


void termina_gioco(){
 //LIBERO MEMORIA DINAMICA  
 free_mazzo(&mago_1);   //FREE DI ENTRAMBI I MAZZI
 free_mazzo(&mago_2);
 for(int i = 0; i <6; i++){ //FREE DI ENTRAMBE LE MANI
   free(mago_1.mano[i]);
   mago_1.mano[i] = NULL;
 }

 for(int i = 0; i <6; i++){
   free(mago_2.mano[i]);
   mago_2.mano[i] = NULL;
  }

 for(int i = 0; i <4; i++){  //FREE DI ENTRAMBI I CAMPI
  free(mago_1.campo[i]);
  mago_1.campo[i] = NULL;
 }
      
 for(int i = 0; i <4; i++){
  free(mago_2.campo[i]);
  mago_2.campo[i] = NULL;
 }

 conta_imposta_gioco--;
 cantatore_mazzo_1 = 0;    //AZZERAMENTO VARIABILI GLOBALI
 cantatore_mazzo_2 = 0;
 cantatore_mani = 0;
 vittoria = 0;
 
 system("clear");
 printf("\n");
 printf("███▀▀▀██┼███▀▀▀███┼███▀█▄█▀███┼██▀▀▀\n");
 printf("██┼┼┼┼██┼██┼┼┼┼┼██┼██┼┼┼█┼┼┼██┼██┼┼┼\n");
 printf("██┼┼┼▄▄▄┼██▄▄▄▄▄██┼██┼┼┼▀┼┼┼██┼██▀▀▀\n");
 printf("██┼┼┼┼██┼██┼┼┼┼┼██┼██┼┼┼┼┼┼┼██┼██┼┼┼\n");
 printf("███▄▄▄██┼██┼┼┼┼┼██┼██┼┼┼┼┼┼┼██┼██▄▄▄\n");
 printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
 printf("███▀▀▀███┼▀███┼┼██▀┼██▀▀▀┼██▀▀▀▀██▄┼\n");
 printf("██┼┼┼┼┼██┼┼┼██┼┼██┼┼██┼┼┼┼██┼┼┼┼┼██┼\n");
 printf("██┼┼┼┼┼██┼┼┼██┼┼██┼┼██▀▀▀┼██▄▄▄▄▄▀▀┼\n");
 printf("██┼┼┼┼┼██┼┼┼██┼┼█▀┼┼██┼┼┼┼██┼┼┼┼┼██┼\n");
 printf("███▄▄▄███┼┼┼─▀█▀┼┼─┼██▄▄▄┼██┼┼┼┼┼██▄\n");
 printf("\nAlla prossima\n");
}

int scelta_classe_mago(){   //FUNZIONE CHE PERMETTE DI SCEGLIERE LA CLASSE 
  int appoggio; //VARIABILE D'APPOGGIO
  do{
    printf("Scegli la classe:\n");
    printf("Premi 1 per utilizzare un mago tenebre\n");
    printf("Premi 2 per utilizzare un mago vita\n");
    printf("Premi 3 per utilizzare un mago luce\n");
    printf("La tua scelta: ");
    scanf("%d", &appoggio);
    while(getchar() != '\n');
    switch(appoggio){              //SWITCH PER LA SCELTA
      case 1:{
        return tenebre;
      }
      case 2:{
        return vita;
      }
      case 3:{
        return luce;
      }
      default:{
        printf("Input invalido\n");   //RISTAMPO IL MENÙ SOLO SE INPUT INVALIDO
      }
    }
  }while(appoggio != 1 && appoggio != 2 && appoggio != 3);
 return 0; 
}


void inserisci_carta_coda(struct Mago *pmago){  //FUNZIONE UTILIZZATA PER CREARE IL MAZZO
    struct Carta* new_carta =(struct Carta*)(malloc(sizeof(struct Carta*))); //VADO A CREARE UNA STRUCT ALLOCANDO MEMORIA DINAMICA
    new_carta->tipo  = genera_tipo();   // RIGA 255
    new_carta->punti_vita_inflitti = genera_hp_classe(pmago, new_carta); //RIGA 342
    new_carta->pNext = NULL;

    if(pmago->inizio_mazzo == NULL){  //SE INIZIO MAZZO È PUNTATO A NULL LA CARTA DIVENTA SIA L'INIZIO CHE LA FINE DEL MAZZO
      pmago->inizio_mazzo = new_carta;   
      pmago->fine_mazzo = new_carta;
    }
    else
    {
      pmago->fine_mazzo->pNext = new_carta;  //ALTRIMENTI DIVENTA LA FINE DEL MAZZO
      pmago->fine_mazzo = new_carta;
  }
}


int genera_tipo(void){ //GENERA IL TIPO DELLA CARTA IN BASE A DELLE PROBABILITÀ
  int t;
  t = rand()%100;              //GENERO NUMERO DA 0 A 99
  if((t >= 0) && (t <= 39)){                    //PROBABILITÀ TIPO DELLA CARTA:
    return creatura;                            //SE ESCE UN NUMERO DA 0 A 39 (40%)--->CREATURA
  }                                             //DA 40 A 64 (25%)--->RIMUOVI CREATURA
  else if((t>= 40) && (t<= 64)){                //DA 65 A 84 (20%)--->INFLIGGI DANNI
    return rimuovi_carta;                       //DA 85 A 99 (15%)---> GUARISCI DANNI
  }
  else if((t>= 65) && (t<= 84)){
    return infliggi_danno;
  }
  else{
    return guarisci_danno;
  }
}


int genera_hp(struct Carta* pcarta){          //GENERA IL NUMERO DI HP RANDOMICAMENTE. UTILIZZA 2 VARIABILI GLOBALI PER CREARE IL MAZZO, OVVERO OGNI VOLTA CHE SI CHIAMA LA FUNZIONE VIENE AUMENTATO IL CONTATORE DI UNO E SI COMPARA CON LA LUNGHEZZA DEL MAZZO.

  while(cantatore_mazzo_1!=lunghezza_mazzo){
  if((pcarta->tipo == creatura) && (cantatore_mazzo_1 < (lunghezza_mazzo/4))){                                                           //LE PRIME CREATURE INSERITE (FINO AD 1/4 DEL MAZZO) HANNO PUNTI VITA COMPRESI TRA 7 E 8
    cantatore_mazzo_1++;                                                                                                                 //LE CREATURE DA UN 1/4 FINO A METÀ DEL MAZZO HANNO PUNTI VITA TRA 5 E 6
    return rand()% 2 + 7;                                                                                                                //LE CREATURE DALLA METÀ FINO 3/4 HANNO PUNTI VITA TRA 3 E 4
  }else if((pcarta->tipo == creatura) && (cantatore_mazzo_1 >= (lunghezza_mazzo/4)) && (cantatore_mazzo_1 < (lunghezza_mazzo/2))){       //LE CREATURE DA 3/4 ALLA FINE HANNO PUNTI VITA TRA 1 E 2
    cantatore_mazzo_1++;
    return rand()% 2 + 5;
  }else if((pcarta->tipo == creatura) && (cantatore_mazzo_1 >= (lunghezza_mazzo/2)) && (cantatore_mazzo_1 < (lunghezza_mazzo/4*3))){
    cantatore_mazzo_1++;
    return rand()% 2 + 3;
  }else if((pcarta->tipo == creatura) && (cantatore_mazzo_1 >= (lunghezza_mazzo/4*3)) && (cantatore_mazzo_1 < lunghezza_mazzo)){
    cantatore_mazzo_1++;
    return rand()% 2 + 1;
  }else if(pcarta->tipo == rimuovi_carta){  //SE LE CARTE SONO RIMUOVI CREATURA RETURNA 0 PV 
    cantatore_mazzo_1++;
    return 0;
  }else if(pcarta->tipo == infliggi_danno){ // SE LE CARTE SONO INFLIGGI DANNO RETURNA UN VALORE DA 2 A 4
    cantatore_mazzo_1++;
    return ((rand()% 3) + 2);
  }else{                      // SE LE CARTE SONO GUARISCI DANNO RETURNA UN VALORE DA 2 A 5
    cantatore_mazzo_1++;
    return ((rand()% 4) + 2);
  }}

  while(cantatore_mazzo_2!=lunghezza_mazzo){
  if((pcarta->tipo == creatura) && (cantatore_mazzo_2 < (lunghezza_mazzo/4))){                                                           //LE PRIME CREATURE INSERITE (FINO AD 1/4 DEL MAZZO) HANNO PUNTI VITA COMPRESI TRA 7 E 8
    cantatore_mazzo_2++;                                                                                                          //LE CREATURE DA UN 1/4 FINO A METÀ DEL MAZZO HANNO PUNTI VITA TRA 5 E 6
    return rand()% 2 + 7;                                                                                                         //LE CREATURE DALLA METÀ FINO 3/4 HANNO PUNTI VITA TRA 3 E 4
  }else if((pcarta->tipo == creatura) && (cantatore_mazzo_2 >= (lunghezza_mazzo/4)) && (cantatore_mazzo_2 < (lunghezza_mazzo/2))){       //LE CREATURE DA 3/4 ALLA FINE HANNO PUNTI VITA TRA 1 E 2
    cantatore_mazzo_2++;
    return rand()% 2 + 5;
  }else if((pcarta->tipo == creatura) && (cantatore_mazzo_2 >= (lunghezza_mazzo/2)) && (cantatore_mazzo_2 < (lunghezza_mazzo/4*3))){
    cantatore_mazzo_2++;
    return rand()% 2 + 3;
  }else if((pcarta->tipo == creatura) && (cantatore_mazzo_2 >= (lunghezza_mazzo/4*3)) && (cantatore_mazzo_2 < lunghezza_mazzo)){
    cantatore_mazzo_2++;
    return rand()% 2 + 1;
  }else if(pcarta->tipo == rimuovi_carta){
    cantatore_mazzo_2++;
    return 0;
  }else if(pcarta->tipo == infliggi_danno){
    cantatore_mazzo_2++;
    return ((rand()% 3) + 2);
  }else{
    cantatore_mazzo_2++;
    return ((rand()% 4) + 2);
  }}

  while(cantatore_mani != 10){        //PER CRARE GLI HP DELLE CARTE NELLE MANI
    if(pcarta->tipo == creatura){            //LE CREATURE HANNO HP DA 1 A 2
    cantatore_mani++;
    return rand()% 2 +1;
    }else if(pcarta->tipo == rimuovi_carta){      //LE CARTE RIMUOVI CREATURA NON HANNO HP. 
      cantatore_mani++;
      return 0;
    }else if(pcarta->tipo == infliggi_danno){      //LE CARTE INFLIGGGI DANNO HANNO HP DA 2 A 4
      cantatore_mani++;
      return ((rand()% 3) + 2);
    }else if(pcarta->tipo == guarisci_danno){      //LE CARTE GUARISCI DANNO HANNO HP DA 2  A 5
      cantatore_mani++;
      return ((rand()% 4) + 2);
    }
  }
 return 0; 
}


int genera_hp_classe(struct Mago* pmago, struct Carta* pcarta){                       //FUNZIONE PER CAMBIARE IL NUMERO DI CARTE IN RELAZIONE ALLA CLASSE DEL MAGO: QUESTA FUNZIONE VIENE CHIAMATA SIA NELLA CREAZIONE DEI MAZZI CHE DELLE MANI
  if(pmago->classe == tenebre && pcarta->tipo == infliggi_danno){                                       //SE CLASSE MAGO TENEBRE(0)--->>> INLIGGI DANNO(2) HP*2
    return (genera_hp(pcarta)*2);  //GENERA IL NUMERO RANDOM CHIAMANDO LA FUNZIONE 
  }else if(pmago->classe == vita && pcarta->tipo == creatura){                                  //SE CLASSE MAGO VITA(1)--->>> CREATURE(0) HP*1,5
    return (genera_hp(pcarta)*3/2); 
  }else if(pmago->classe == luce && pcarta->tipo == guarisci_danno){                                  //SE CLASSE MAGO LUCE(2)--->>> GUARISCI DANNI(3) HP*2,5  (ORIGINARIAMENTE ERA 3,5 MA MI SEMBRAVA TROPPO SQUILIBRATO)
    return (genera_hp(pcarta)*5/2); 
  }else { //CASO 
    return genera_hp(pcarta);
  }
}


void crea_mano(struct Mago* pmago){                                                             //CREO LE PRIME 5 CARTE IN MANO
  for(int i = 0; i< 5; i++){
  pmago->mano[i] = (struct Carta*) malloc(sizeof(struct Carta*));
  pmago->mano[i]-> tipo = genera_tipo();
  pmago->mano[i]-> punti_vita_inflitti = genera_hp_classe(pmago, pmago->mano[i]);
  pmago->mano[i]->pNext = NULL;
  }
  pmago->mano[5] = NULL;  //INIZZIALIZZO L'ULTIMA A NULL
}


void stampa_mano(struct Mago* pmago){
  for(int temp = 0; temp < 6; temp++){             //CICLO CHE STAMPA LA MANO
    if(pmago->mano[temp] == NULL){
      printf("Slot libero\n");  //SE LA POSIZIONE NELLA MANO È NULL STAMPA SLOT LIBERO
    }else{
      if(pmago->mano[temp]->tipo == rimuovi_carta){
        printf("Carta %d. Tipo: %s\n", (temp+1), converti_in_tipo(pmago->mano[temp]->tipo)); //LE CARTE RIMUOVI CREATURE VENGONO STAMPATE SENZA HP
      }else{
        printf("Carta %d. Tipo: %s. HP: %d\n", (temp+1), converti_in_tipo(pmago->mano[temp]->tipo), pmago->mano[temp]-> punti_vita_inflitti);
      }  //LA FUNZIONE CONVERTI IN TIPO RESTITUISCE LA STRINGA RELATIVA AI TIPO DELLE CARTE //FUNZIONE RIGA 424
    }
  }
}


void crea_campo(struct Mago* pmago){                      //INZIALIZZO I PUNTATORE DEI DUE CAMPI A NULL
  for(int i = 0; i < 4; i++){
    pmago->campo[i] =NULL;
  }
}


void stampa_campo(struct Mago* pmago){
  for(int temp = 0; temp < 4; temp++){             //CICLO CHE STAMPA IL CAMPO
    if(pmago->campo[temp] == NULL){
      printf("%d. Slot libero\n", (temp+1)); //SE LA POSIZIONE NEL CAMPO È NULL STAMPA SLOT LIBERO
    }else{
      if(pmago->campo[temp]->tipo == rimuovi_carta){
        printf("Carta %d. Tipo: %s\n", (temp+1), converti_in_tipo(pmago->campo[temp]->tipo));  //LE CARTE RIMUOVI CREATURE VENGONO STAMPATE SENZA HP
      }else{
        printf("Carta %d. Tipo: %s. HP: %d\n", (temp+1), converti_in_tipo(pmago->campo[temp]->tipo), pmago->campo[temp]-> punti_vita_inflitti);
      }   //LA FUNZIONE CONVERTI IN TIPO RESTITUISCE LA STRINGA RELATIVA AI TIPO DELLE CARTE //FUNZIONE RIGA 424
    }
  }
}


void stampa_mazzo(struct Mago* pmago){
  if(pmago->inizio_mazzo == NULL){ //SE MAZZO VUOTO
    printf("Il mazzo è vuoto\n");
  }
  else
  {
    struct Carta* pScan= pmago->inizio_mazzo; //PSCAN "SCANSIONE PER IL MAZZO E PER OGNI CARTA STAMPA I RISPETTIVI VALORI"
    do{
      if(pScan->tipo == rimuovi_carta){ 
        printf("Carta Tipo %s\n", converti_in_tipo(pScan->tipo));  //LE CARTE RIMUOVI CREATURE VENGONO STAMPATE SENZA HP
        pScan = pScan->pNext; //PSCAN SI SPOSTA ALLA CARTA SUCCESSIVA
      }else{
        printf("Carta Tipo %s: %dPV\n", converti_in_tipo(pScan->tipo), pScan->punti_vita_inflitti);
        pScan = pScan->pNext;
      }
    }while(pScan != NULL); //FINCHÉ NON SI RAGGIUNGE LA FINE DEL MAZZO
  }
  return;
}


const char* converti_in_tipo(int temp){                              //FUNZIONE PER CONVERTIRE GLI ELEMENTI DI ENUM IN STRINGHE S
   switch (temp)
   {
      case 0: return "Creatura";
      case 1: return "Rimuovi creatura";
      case 2: return "Infliggi danno";
      case 3: return "Guarisci danno";
      default: return "Impossibile almeno toglie il warning";
   }
}


void switch_gioca(struct Mago* pmago, struct Mago* pmago_2){  //FUNZIONE CHIAMATA IN COMBATTI PER EVITARE DI RISCRIVERE IL MENÙ E LO SWITCH DUE VOLTE
  printf("Tocca a te %s", pmago->nome_mago);  //PRENDE IL NOME DEL MAGO
  printf("Scegli cosa fare: \n");
  printf("Premi 1 per pescare una carta\n");
  printf("Premi 2 giocare una carta\n");
  printf("Premi 3 attaccare\n");
  printf("Premi 4 per passare il turno\n");
  printf("Premi 5 per stampare la tua mano\n");
  printf("Premi 6 per stampare entrambi i campi\n");
  printf("Premi 7 per visualizzare lo stato del tuo mago e di quello avversario\n");
  printf("Premi 8 per Visualizzare le carte rimanenti nel mazzo\n");
  printf("Premi 9 per arrenderti\n\n");
  int scelta_turno = 0;
  printf("La tua scelta: ");
  scanf("%d", &scelta_turno);
  while(getchar() != '\n');
  switch(scelta_turno){
    case 1:{
      if(static_pesca == 0){ //SE STATIC_PESCA== 0 SIGNIFICA CHE È LA PRIMA VOLTA CHE SI PESCA NEL TURNO
        static_pesca++;
        pesca(pmago); //RIGA 537
        controlla_vittoria(pmago, pmago_2); //RIGA 867 CONTROLLA SE VENGONO RISPETTATE LE CONDIZIONI DI VITTORIA E IN CASO TERMINA IL LOOP E LA PARTITA
        break;
      }else{  // SE STATIC_PESCA != O ALLORA HAI GIÀ PESCATO
        printf("Puoi pescare una sola volta\n");
        break;
      }
    }
    case 2:{
      if(static_gioca == 0){
        static_gioca++;
        gioca_carta(pmago, pmago_2); // RIGA 581
        controlla_vittoria(pmago, pmago_2); 
        break;
      }else{
        printf("Puoi giocare una sola carta alla volta\n");
        break;
      }
    }
    case 3:{
      if(static_attacca == 0){
        static_attacca++;
        attacca(pmago, pmago_2); //RIGA 756
        controlla_vittoria(pmago, pmago_2);
        break;
      }else{
        printf("Puoi attaccare solo una volta\n");
        break;
      }
    }
    case 4:{
      passa(); //RIGA 900 AZZERA ALCUNE VARIABILI STATICHE E AUMENTA/DIMINUISCE LA VARIABILE STATICA CHE DECIDE IL TURNO
      break;
    }
    case 5:{ //PERMETTE DI STAMPARE LA PROPRIA MANO
      printf("\nLa tua mano:\n");
      stampa_mano(pmago); //RIGA 366
      break;
    }
    case 6:{//PERMETTE DI STAMPARE ENTRAMBI I CAMPI
      printf("\nIl tuo campo:\n");
      stampa_campo(pmago); //RIGA 388
      printf("\nIl campo avversario\n");
      stampa_campo(pmago_2); //IN QUESTO CASO PASSO IL PUNTATORE AL MAGO 2
      break;
    }
    case 7:{  //VISUALIZZA LO STATO DEI DUE MAGHI
      printf("Il tuo mago: Classe: %s. Punti vita rimanenti: %d \n", converti_in_classe(pmago->classe), pmago->punti_vita); //LA FUNZIONE CONVERTI IN CLASSE RIGA 526
      printf("Il mago avversario: Classe %s. Punti vita rimanenti: %d \n", converti_in_classe(pmago_2->classe), pmago_2->punti_vita);
      break;
    }
    case 8:{ //PERMETTE DI VISUALIZZARE IL NUMERO DI CARTE RIMASTA, IN QUANTO AL TERMINE DELLA CARTE DA PESCARE TERMINA IL GIOCO
      if(conta_carte_rimaste(pmago) == 1){ //SE RIMANE UNA SOLA CARTE //CHIAMA FUNZIONE RIGA 911
      printf("Rimane solo una carta da pescare\n");
      }else{
      printf("Sono rimaste %d carte nel mazzo\n", conta_carte_rimaste(pmago));  //CHIAMA FUNZIONE RIGA 911
      }
      break;
    }
    case 9:{ 
      vittoria++; //MODIFICA ALLA VARIABILE GLOBALE CHE TERMINA IL LOOP IN COMBATTI 
      printf("Vittoria per %s", pmago_2->nome_mago); //STAMPA LA VITTORIA PER IL MAGO AVVERSARIO
      break;
    }
    default:{
      printf("Scegli un opzione valida\n");
    }
  }
}

const char* converti_in_classe(int temp){                              //FUNZIONE PER CONVERTIRE GLI ELEMENTI DI ENUM IN STRINGHE S
   switch (temp)
   {
      case 0: return "Tenebre";
      case 1: return "Vita";
      case 2: return "Luce";
      default: return "Impossibile almeno toglie il warning";
   }
}


void pesca(struct Mago* pmago){
  if(pmago->mano[5] != NULL){          //CONTROLLA SE NEL ULTIMO PUNTATORE DELL'ARRAY È PRESENTE UNA CARTA E NEL CASO CANCELLA LA CARTA PESCATA
    printf("Mano piena, distruggo la carta pescata\n");
    rimuovi_in_coda(pmago);
  }else{
    int temp = 0;
    while(pmago->mano[temp] != NULL ){   //LOOP CHE SCORRE FINO AL PRIMO PUNTATORE A NULL, OVVERO IL PRIMO SLOT CARTA VUOTO
    temp++;}
    pmago->mano[temp] = (struct Carta*) malloc(sizeof(struct Carta*));              //CREO LA CARTA
    pmago->mano[temp]->tipo = pmago->fine_mazzo->tipo;    //ASSEGNO I VALORI DELL'ULTIMA CARTA NEL MAZZO
    pmago->mano[temp]->punti_vita_inflitti = pmago->fine_mazzo->punti_vita_inflitti;
    pmago->mano[temp]->pNext = NULL;
    rimuovi_in_coda(pmago); //RIGA 556
    printf("\nCarta pescata. Stampo la mano:\n");
    stampa_mano(pmago);
  }
}


void rimuovi_in_coda(struct Mago* pmago){        //FUNZIONE DI RIMOZIONE IN CODA
  struct Carta* pPrev = NULL;
  struct Carta* pScan = pmago->inizio_mazzo;
  if(pScan->pNext == NULL){
    printf("Ultima carta. Vince chi ha più HP\n");                   //CASO IN CUI MANCA UNA SOLA CARTA
    free(pScan);
    pmago->inizio_mazzo = NULL;
  }
  else{
    do{
      if(pScan->pNext == pmago->fine_mazzo ){
        pPrev = pScan;
        break;
      }
      else{
        pScan = pScan->pNext; //SCORRE NEL MAZZO DI CARTA IN CARTA
      }
    }while(pScan->pNext != NULL); //CONTINUA FINCHÉ LA CARTA SUCCESSIVA A SCAN È NULL 
  free(pPrev->pNext);  
  pPrev->pNext = NULL;
  pmago->fine_mazzo = pPrev;
  }
}


void gioca_carta(struct Mago * pmago, struct Mago * pmago_2){
  printf("\nHai scelto di giocare una carta: \n\n");
  stampa_mano(pmago);       //PRIMA DI SCEGIERE UNA CARTA SI STAMPA LA MANO
  int scelta_carta = 0;
  int controllo = 0;  //VARIABILE DI CONTROLLO
  while(controllo == 0){
    if(pmago->mano[0] == NULL){  //SE MANO VUOTA 
     printf("Impossibile giocare carte. Mano vuota\n"); 
     static_gioca--; //RIDUCO LA VARIABILE STATICA. PERMETTENDO DI RIGIOCARE IN SEGUITO AD UNA PESCATA
     controllo =1; //VARIABILE PER INTERROMPERE IL LOOP
     break;  //INTERROMPO LOOP
    } 
    printf("\nScegli la carta da giocare:\n");
    printf("La tua scelta: ");
    scanf("%d", &scelta_carta);
    while(getchar() != '\n'){};
    if((pmago->mano[(scelta_carta -1)] == NULL) || ((scelta_carta != 1)&& (scelta_carta != 2)&& (scelta_carta != 3)&& (scelta_carta != 4)&& (scelta_carta != 5)&& (scelta_carta != 6))){    //SE  SCEGLI UNA CARTA NON PRESENTE NELLA MANO SI RICHIEDE INPUT DA MODIFICARE DA MODIFICARE
      printf("\nCarta non presente riprova\n");
    }else if(pmago->mano[(scelta_carta -1)]->tipo == creatura){  //SE SCEGLI UNA CREATURA
      printf("\nHai giocato una creatura\n\n");
      int temp = 0;
      while(pmago->campo[temp] != NULL) //TROVA IL PRIMO ELEMENTO LIBERO NEL CAMPO
      temp++;
      if(temp >= 4){
        printf("Impossbile giocare la creatura. Campo pieno.\n");
      }else{
        pmago->campo[temp] = (struct Carta*) malloc(sizeof(struct Carta*)); //CREA LA CARTA NEL CAMPO FORMATA DAI VALORI DELLA CARTA GIOCATA
        pmago->campo[temp]->tipo = pmago->mano[scelta_carta -1]->tipo;
        pmago->campo[temp]->punti_vita_inflitti = pmago->mano[scelta_carta -1]->punti_vita_inflitti;
        pmago->campo[temp]->pNext = NULL;
        pmago->mano[scelta_carta - 1] = NULL;  //IL PUNTATORE DELLA CARTA IN MANO GIOCATA VIENE PUNTATO A NULL
        printf("Il tuo campo: \n");
        stampa_campo(pmago);
        riordina_mano(pmago); //RIGA 853
        controllo = 1;
      }
    }else if(pmago->mano[(scelta_carta -1)]->tipo == rimuovi_carta){ //SE SCEGLI UNA CARTA RIMUOVI CREATURA
      printf("\nHai giocato Rimuovi creatura\n");
      if(pmago_2->campo[0] == NULL && pmago_2->campo[1] == NULL && pmago_2->campo[2] == NULL && pmago_2->campo[3] == NULL){       //SE IL CAMPO DELL'AVVERSARIO È VUOTO SI RICHIEDE DI GIOCARE UN'ALTRA CARTA
        printf("\nCampo avversario vuoto. Impossibile giocare la carta.\n");
      }else{
        printf("Campo avversario:\n");
        stampa_campo(pmago_2);
        int controllo_rimuovi = 0;  //VARIABILE PER TERMINARE DO WHILE RIGA SUCCESSIVA
        do{
        int scelta_obiettivo = 0;
        printf("\nScegli l'obiettivo della tua carta:\n");
        printf("La tua scelta: ");
        scanf("%d", &scelta_obiettivo);
        while(getchar() != '\n'){};
          if((pmago_2->campo[scelta_obiettivo-1] != NULL) && (scelta_obiettivo < 5)){ //SE LA SCELTA È CONSENTITA
            free(pmago_2->campo[scelta_obiettivo-1]);  //FREE DI ENTRAMBE LE CARTE
            free(pmago->mano[scelta_carta-1]);
            pmago_2->campo[scelta_obiettivo-1] = NULL;
            pmago->mano[scelta_carta - 1] = NULL;
            stampa_campo(pmago_2);
            riordina_mano(pmago);
            printf("\nCarta rimossa con successo.\n");
            controllo_rimuovi = 1;
          }else{
            printf("Scelta non consentita.\n");
          }
        }while(controllo_rimuovi == 0);
        controllo=1;
      }
    }else if(pmago->mano[(scelta_carta -1)]->tipo == infliggi_danno){  // CASO NEL CUI SI GIOCA UNA CARTA INFLIGGI DANNO
      int controllo_infliggi; //VARIABILE PER TERMINARE DO WHILE RIGA SUCCESSIVA
      do{
        printf("\nHai giocato Infliggi danno\n");
        printf("\nScegli contro chi giocare la carta:\n");
        printf("Premi 1 per attaccare il mago avversario.\n");
        printf("Premi 2 per attaccare una creatura sul campo avversario.\n");
        controllo_infliggi = 0;
        int scelta_infliggi_danno = 0;
        printf("La tua scelta: ");
        scanf("%d", &scelta_infliggi_danno);
        while(getchar() != '\n');
        if(scelta_infliggi_danno != 1 && scelta_infliggi_danno != 2){ // SE INPUT INVALIDO SI RICHIEDE
          printf("\nScelta non consentita");
        }else if(scelta_infliggi_danno == 1){  //SE SI SCEGLIE DI DANNEGGIARE IL MAGO AVVERSARIO DIRETTAMENTE
          pmago_2->punti_vita = pmago_2->punti_vita - pmago->mano[scelta_carta -1]->punti_vita_inflitti; //SI SOTTRAGGONO PUNTI VITA
          free(pmago->mano[scelta_carta-1]); //FREE CARTA GIOCATA
          pmago->mano[scelta_carta - 1] = NULL;
          printf("\nAttacco diretto eseguito. PV rimasti all'avversario: %d\n", pmago_2->punti_vita);
          riordina_mano(pmago);
          controllo_infliggi = 1;
        }else if(scelta_infliggi_danno == 2){  //SE SI SCEGLIE DI DANNEGGIARE UNA CREATURA DEL CAMPO AVVERSARIO
          if(pmago_2->campo[0] == NULL && pmago_2->campo[1] == NULL && pmago_2->campo[2] == NULL && pmago_2->campo[3] == NULL){ //CASO IN CUI IL CAMPO AVVERSARIO È VUOTO
            printf("\nCampo avversario vuoto. Impossibile giocare la carta.\n");
          }else{
            printf("\nCampo avversario:\n");
            stampa_campo(pmago_2);
            int scelta_obiettivo = 0;  //VARIABILE PER LA SCELTA DELLA CARTA DA DANNEGGIARE
            printf("\nScegli il tuo obiettivo.\n");
            printf("La tua scelta: ");
            scanf("%d", &scelta_obiettivo);
            while(getchar() != '\n');
            if((pmago_2->campo[scelta_obiettivo-1] != NULL) && (scelta_obiettivo < 5)){   //CASO NEL CUI LA SCELTA DELL'OBIETTIVO SIA CONSENTITA
              if(pmago_2->campo[scelta_obiettivo-1]->punti_vita_inflitti <= pmago->mano[scelta_carta-1]->punti_vita_inflitti){ //SE I PUNTI VITA DELLA CREATURA SCELTA SONO INFERIORI DELLA CARTA INFLIGGI DANNO GIOCATA
                free(pmago_2->campo[scelta_obiettivo-1]);     //FREE DI ENTRAMBE LE CARTE
                free(pmago->mano[scelta_carta-1]);
                pmago_2->campo[scelta_obiettivo-1] = NULL;
                pmago->mano[scelta_carta - 1] = NULL;
                printf("\nCampo avversario:\n");
                stampa_campo(pmago_2);
                printf("\nCreatura rimossa\n");
                riordina_mano(pmago);
                controllo_infliggi = 1;        //VARIABILE DI CONTROLLO PER USCIRE DAL LOOP
              }else{ //SE I PUNTI VITA DELLA CREATURA SCELTA SONO SUPERIORI DELLA CARTA INFLIGGI DANNO GIOCATA
                pmago_2->campo[scelta_obiettivo-1]->punti_vita_inflitti = pmago_2->campo[scelta_obiettivo-1]->punti_vita_inflitti - pmago->mano[scelta_carta-1]->punti_vita_inflitti; //TOLGO I PUNTI VITA ALLA CREATURA
                free(pmago->mano[scelta_carta-1]); //FREE SOLO DELLA CARTA INFLIGGI DANNO GIOCATA
                pmago->mano[scelta_carta - 1] = NULL;
                riordina_mano(pmago);
                controllo_infliggi = 1;  //VARIABILE DI CONTROLLO PER USCIRE DAL LOOP
              }
            }else{
              printf("Scelta non consentita\n");
            }
          }
        }
      }while(controllo_infliggi == 0);
        controllo = 1;
    }else{ //SE SI SCEGLIE GUARISCI DANNO
     int controllo_guarisci= 0; //VARIABILE PER TERMINARE DO WHILE RIGA SUCCESSIVA
      do{
        printf("\nHai giocato Guarisci danno\n");
        printf("\nScegli l'obiettivo della tua cura:\n");
        printf("Premi 1 per curare il tuo mago.\n");
        printf("Premi 2 per curare una creatura del tuo campo.\n");
        int scelta_guarisci_danno = 0;
        printf("La tua scelta: ");
        scanf("%d", &scelta_guarisci_danno);
        while(getchar() != '\n');
        if(scelta_guarisci_danno != 1 && scelta_guarisci_danno != 2 ){  //CASO IN CUI LA SCELTA NON SIA NE 1 NE 2
          printf("Scelta non consentita\n");
        }else if(scelta_guarisci_danno == 1){ // CASO SE SI GUARISCE IL PROPRIO MAGO
          pmago->punti_vita = pmago->punti_vita + pmago->mano[scelta_carta-1]->punti_vita_inflitti; //AGGIUNGO HP AL MAGO
          free(pmago->mano[scelta_carta-1]);   //FREE CARTA GIOCATA
          pmago->mano[scelta_carta-1] = NULL;
          riordina_mano(pmago); //RIORDINO LA MANO
          printf("Mago curato\n");
          controllo_guarisci = 1;    //INTERROMPO LOOP
        }else if(scelta_guarisci_danno == 2){ //SE SI GUARISCE UNA CREATURA
          if(pmago->campo[0] == NULL && pmago->campo[1] == NULL && pmago->campo[2] == NULL && pmago->campo[3] == NULL){ //SE IL PROPRIO CAMPO È VUOTO
            printf("Campo vuoto. Impossibile curare.\n");
          }else{ //SE CAMPO NON VUOTO
            printf("Il tuo campo:\n");
            stampa_campo(pmago);
            int scelta_obiettivo = 0;
            printf("\nScegli il tuo obiettivo:\n");
            printf("La tua scelta: ");
            scanf("%d", &scelta_obiettivo);
            while(getchar() != '\n');
            if((pmago->campo[scelta_obiettivo-1] != NULL) && (scelta_obiettivo < 5) ){  // SE SI SCEGLIE UNA CREATURA PRESENTE NEL CAMPO
              pmago->campo[scelta_obiettivo -1]->punti_vita_inflitti = pmago->campo[scelta_obiettivo -1]->punti_vita_inflitti + pmago->mano[scelta_carta - 1]->punti_vita_inflitti; //SI AGGIUNGONO HP ALLA CREATURA
              if(pmago->campo[scelta_obiettivo -1]->punti_vita_inflitti > 10){
                pmago->campo[scelta_obiettivo -1]->punti_vita_inflitti = 10;  //LIMITE PER PV CREATURE IMPOSTATO A 10
              }
              free(pmago->mano[scelta_carta-1]); //FREE DELLA CARTA GIOCATA
              pmago->mano[scelta_carta - 1] = NULL;
              printf("\nCreatura curata.\n");
              riordina_mano(pmago);
              controllo_guarisci = 1;
            }else{
              printf("Scelta non consentita.\n");
            }
          }
        }
      }while(controllo_guarisci == 0);
      controllo = 1;
    }
  }
}


void attacca(struct Mago* pmago, struct Mago* pmago_2){     
  if(pmago->campo[0] == NULL && pmago->campo[1] == NULL && pmago->campo[2] == NULL && pmago->campo[3] == NULL){ //CASO NEL CUI IL PROPRIO CAMPO SIA VUOTO.
      printf("Impossibile attaccare. Campo vuoto.\n");
      static_attacca--; //DECREMENTO VARIABILE STATICA PER RIATTACARE IN SEGUITO AD UN EVENTUALE GIOCATA
      return;
  }else{
    int carte_sul_campo = 0;
    for(int i = 0; i < 4; i++){    //LOOP CHE CONTA IL NUMERO DI CARTE PRESENTI SUL CAMPO
      if(pmago->campo[i] != NULL){
        carte_sul_campo++;
      }
    }
    int array[4] = {0,0,0,0}; //ARRAY DI INT (TUTTI GLI ELEMENTI INIZIALIAZZATI A 0) CHE MEMORIZZA LA SCELTA DELLA CARTA ATTACCANTE. IN QUESTO MODO SI PUÒ IMPEDIRE AL GIOCATORE DI ATTACCARE DUE VOLTE CON LA STESSA CARTA NELLO STESSO TURNO
    for(int i = 0; i < carte_sul_campo; i++){ 
      int controllo= 0;  //VARIABILE PER IL LOOP
      do{
        printf("------------------------------------------------------\n");
        printf("Il tuo campo:\n");
        stampa_campo(pmago);
        printf("\n");
        printf("Campo avversario:\n");
        stampa_campo(pmago_2);
        printf("\n");
        printf("Scegli con quale creatura attaccare:\n");
        int scelta_carta = 0;
        printf("La tua scelta: ");
        scanf("%d", &scelta_carta);
        while(getchar() != '\n'){};
        if(scelta_carta != array[0] && scelta_carta != array[1] && scelta_carta != array[2] && scelta_carta != array[3]){ //CASO IN CUI LA CARTA NON HA GIÀ ATTACCATO
          if((pmago->campo[scelta_carta-1] != NULL) && (scelta_carta < 5)){
            printf("\nScegli chi attaccare:\n");
            printf("Premi 1 per attaccare il mago avversario.\n");
            printf("Premi 2 per attaccare una creatura sul campo avversario.\n");
            int scelta_obiettivo = 0;
            printf("La tua scelta: ");
            scanf("%d", &scelta_obiettivo);
            while(getchar() != '\n');
            if(scelta_obiettivo == 1){
              pmago_2->punti_vita = pmago_2->punti_vita - pmago->campo[scelta_carta-1]->punti_vita_inflitti;  //  SE SI SCEGLIE  DI ATTACCARE IL MAGO AVVERSARIO VENGONO SOTTRATTI GLI HP DELLA CARTA AL MAGO AVVERSARIO
              array[i] = scelta_carta;  //MEMORIZZO NELL'ARRAY LA SCELTA DELLA CARTA GATTACCANTE
              printf("\nAttacco diretto eseguito. PV rimasti all'avversario: %d\n", pmago_2->punti_vita);
              controllo = 1;
            }else if(scelta_obiettivo == 2){
              if(pmago_2->campo[0] == NULL && pmago_2->campo[1] == NULL && pmago_2->campo[2] == NULL && pmago_2->campo[3] == NULL){ //SE CAMPO VUOTO SI RICHIEDE L'OBIETTIVO
                printf("\nCampo avversario vuoto. Impossibile attaccare una creatura.\n");
              }else{
                printf("\nScegli il tuo obiettivo:\n");
                stampa_campo(pmago_2);
                int scelta_attacco = 0;
                printf("La tua scelta: ");
                scanf("%d", &scelta_attacco);
                while(getchar() != '\n');
                if((pmago_2->campo[scelta_attacco -1] != NULL) && (scelta_attacco < 5)){
                  if(pmago->campo[scelta_carta-1]->punti_vita_inflitti > pmago_2->campo[scelta_attacco-1]->punti_vita_inflitti){ //SE LA CREATURA ATTACCANTE HA PIÙ HP DELLA CREATURA ATTACCATA, VENGONO SOTTRATTI GLI HP DALLA CARTA ATTACCANTE, MENTRE QUELLA ATTACCATA VIENE ELIMINATA(FREE)
                    pmago->campo[scelta_carta-1]->punti_vita_inflitti = pmago->campo[scelta_carta-1]->punti_vita_inflitti - pmago_2->campo[scelta_attacco-1]->punti_vita_inflitti;
                    free(pmago_2->campo[scelta_attacco-1]);
                    pmago_2->campo[scelta_attacco-1] = NULL; //PUNTATORE A NULL
                    array[i] = scelta_carta;  //MEMORIZZO NELL'ARRAY LA SCELTA DELLA CARTA GATTACCANTE
                    printf("\nCreatura avversaria distrutta\n");
                    controllo = 1;
                  }else if(pmago->campo[scelta_carta-1]->punti_vita_inflitti < pmago_2->campo[scelta_attacco-1]->punti_vita_inflitti){//SE LA CREATURA ATTACCANTE HA MENO HP DELLA CREATURA ATTACCATA, VENGONO SOTTRATTI GLI HP DALLA CARTA ATTACCATA, MENTRE QUELLA ATTACCANTE VIENE ELIMINATA(FREE)
                    pmago_2->campo[scelta_attacco-1]->punti_vita_inflitti = pmago_2->campo[scelta_attacco-1]->punti_vita_inflitti - pmago->campo[scelta_carta-1]->punti_vita_inflitti;
                    free(pmago->campo[scelta_carta-1]);
                    pmago->campo[scelta_carta-1] = NULL; //PUNTATORE A NULL
                    array[i] = scelta_carta;  //MEMORIZZO NELL'ARRAY LA SCELTA DELLA CARTA GATTACCANTE
                    printf("\nCreatura avversaria danneggiata\n");
                    controllo = 1;
                  }else{ // SE LE CARTE HANNO GLI STESSI HP VENGONO ELIMINATE(FREE) ENTRAMBE LE CREATURE
                    free(pmago_2->campo[scelta_attacco-1]);
                    free(pmago->campo[scelta_carta-1]);
                    pmago->campo[scelta_carta-1] = NULL;  //PUNTATORI A NULL
                    pmago_2->campo[scelta_attacco-1] = NULL;
                    array[i] = scelta_carta;  //MEMORIZZO NELL'ARRAY LA SCELTA DELLA CARTA GATTACCANTE
                    printf("\nSia la tua creatura che quella avversaria sono state distrutte\n");
                    controllo = 1;
                  }
                }else{
                  printf("Scelta non consentita\n");
                }
              }
            }else{
              printf("Scelta non consentita\n");
            }
          }else{
              printf("Scelta non consentita\n");
          }
        }else{ 
          printf("Carta già giocata o scelta non consentita\n");
          i--; //DIMINUISCO i DI 1 PER RIPETERE IL CICLO 
          controllo = 1;
        }
      }while(controllo ==0);
    }
  }    
}


void riordina_mano(struct Mago* pmago){  //QUESTA FUNZIONE RIORDINA LA MANO
  int carta_giocata = 0;
  while(pmago->mano[carta_giocata] != NULL)  //TROVO L'ELEMENTO DELL'ARRAY CHE CORRISPONDE ALLA CARTA GIOCATA
  carta_giocata++;
  int temp = 5;
  while(pmago->mano[temp] == NULL) // TROVO QUANTE VOLTE È NECESSARIO ESEGUIRE IL FOR
  temp--;
  for(int i = carta_giocata; i < temp; i++){  //QUESTO CICLO SCORRE LA MANO E ASSEGNA AL VALORE DELLA EX CARTA GIOCATA QUELLO DELL'ELEMENTO SUCCESSIVO E RIPETE L'OPERAZIONE PER IL RESTO DELLA MANO
    free(pmago->mano[i]);
    pmago->mano[i] = NULL;
    pmago->mano[i] = (struct Carta*) malloc(sizeof(struct Carta*));
    pmago->mano[i]->tipo = pmago->mano[i+1]->tipo;
    pmago->mano[i]->punti_vita_inflitti = pmago->mano[i+1]->punti_vita_inflitti;
    pmago->mano[i]->pNext = NULL;
    carta_giocata++;
  }
  pmago->mano[carta_giocata] = NULL;   //VIENE PUNTATO A NULL L'ELEMENTO CHE SAREBBE RIDONDANTE
}


void controlla_vittoria(struct Mago* pmago, struct Mago* pmago_2){  //QUESTA FUNZIONE VERIFICA SE LE CONDIZIONI CHE TERMINANO LA PARTITA SONO SODDISFATTE
  if((pmago->inizio_mazzo == NULL || pmago_2->inizio_mazzo  == NULL) /*&& (pmago->mano[0] == NULL || pmago_2->mano[0] == NULL)*/){  //CASO IN CUI UNO DEI DUE GIOCATORI FINISCE LE CARTE
    if(pmago->punti_vita > pmago_2->punti_vita){
      printf("Vittoria per %s", pmago->nome_mago);
      vittoria = 1;
    }else if(pmago->punti_vita < pmago_2->punti_vita){
      printf("Vittoria per %s", pmago_2->nome_mago);
      vittoria = 1;
    }else{
      printf("Pareggio\n");
      vittoria = 1;
    }
  }else if(pmago->punti_vita <= 0){  // CASO NEL CUI IL MAGO 1 ARRIVI A 0 PV
    printf("Vittoria per %s", pmago_2->nome_mago);
    vittoria = 1;
  }else if(pmago_2->punti_vita <= 0){ // CASO NEL CUI IL MAGO 2 ARRIVI A 0 PV
    printf("Vittoria per %s", pmago->nome_mago);
    vittoria = 1;
  }else{
    return;
  }
}


void free_mazzo(struct Mago* pmago){  //LIBERA MEMORIA OCCUPATA DAL MAZZO
  struct Carta* temp;
  while(pmago->inizio_mazzo != NULL){
    temp = pmago->inizio_mazzo;
    pmago->inizio_mazzo = pmago->inizio_mazzo->pNext;
    free(temp);
  }
}

void passa(){
  static_pesca = 0; //AZZERAMENTO 3 VARIABILI STATICHE NECESSARIE PER EVITARE CHE IL GIOCATORE PESCHI/GIOCHI/ATTACCHI PIÙ DI UNA VOLTA
  static_gioca = 0;
  static_attacca = 0;
  if(inizio_game  == 0){ //PASSSA IL TURNO: SE LA VARIABILE ASSEGNATA AL NUMERO GENERATO RANDOMICAMENTE È UGUALE 0 INCREMENTA LA VARIABILE ALTRIMENTI LA DECREMENTA
    inizio_game++;
  }else{
    inizio_game--;
  }
}

int conta_carte_rimaste(struct Mago* pmago){
  int a = 0;
  struct Carta* pScan = pmago->inizio_mazzo;
  while(pScan != NULL){
    a++;
    pScan = pScan->pNext;
  }
  return a;
}


void stampa_regole(){
  system("clear");
  printf("\n\nBENVENUTO NEL MONDO DI TRAGIC.\n");
  printf("\nLE BASI DEL GIOCO:\n\n");
  printf("Si gioca in due.\n");
  printf("Ad ognuno è chiesto di inserire un nickname ed una classe.\n");
  printf("Ci sono tre classi:\n");
  printf("Tenebre, i cui maghi provengono dalla lontana regione di Gor-ca.\n");
  printf("Vita, i cui maghi provengono dalla lussureggiante città di Skaven.\n");
  printf("Luce, i cui maghi discendono degli antichi Dei.\n");

  printf("\n\nI TIPI DI CARTE:\n");
  printf("\nCREATURE: non sono altro che esseri manipolati e schiavizzati dai rispettivi maghi.\nAnche se non possiedono il libero arbitrio, in loro rimane la coscienza di essere solo pedine in mano a tiranni.\nLe creature più forti sono anche le più protette, perciò dovrete giocare a fondo per sfoderare una di loro.\nPunti vita da 1 a 8.\n");
  printf("\nRIMUOVI CREATURA: Antico strumento di punizione utilizzato per eliminare nemici.\nQuesto attacco è stato bandito da tutte le civiltà più avanzate, in quanto ritenuto un atto disumano.\nTuttavia i maghi non temono nessun uomo e ne fanno un uso sconsiderato.\nPermette di rimuovere una creatura avversaria, indipendentemente dai suoi punti vita.\n");
  printf("\nINFLIGGI DANNO: Forma di attacco semplice ma efficace.\nRimane tuttora utilizzata nelle grandi battaglie campali.\nDanni da 1 a 3 PV.\n");
  printf("\nGUARISCI_DANNO: Antichissima forma di cura lasciata in eredità dagli antichi Dei.\nPermette di incrementare i PV del tuo mago o di una creatura.\nPV guariti da 2 a 4\n");
  printf("\n\nLE REGOLE:\n");
  printf("Ogni mago ha a disposizione un mazzo di lunghezza variabile ed inizia con una mano di 5 carte.\n");
  printf("Il primo mago a giocare è scelto dall'antico rituale magico segreto.\n");
  printf("Per ogni turno il giocatore può scegliere di pescare, di giocare una carta e di attaccare.\n");
  printf("La partita può terminare in due modi:\n");
  printf("-quando uno dei due maghi raggiunge 0 PV\n");
  printf("-quando, finite le carte da pescare, vince il giocatore con più PV\n");
  printf("\n\nNOTE IMPORTANTI:\n");
  printf("Alcune carte subiscono le effetto dei propri maghi:\n");
  printf("-le creature hanno la metà dei PV in più, se il mago ha classe vita.\n");
  printf("-le infliggi danno hanno il doppia dei PV, se il mago ha classe tenebre.\n");
  printf("-le guarisci danno hanno due volte e mezzo dei PV, se il mago ha classe luce.\n\n");
}