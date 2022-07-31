#include <stdio.h>
#include <stdlib.h>

void imposta_gioco();
void combatti();
void termina_gioco();
void stampa_regole();

enum Classe_mago {tenebre, vita, luce};
enum Tipo_carta {creatura, rimuovi_carta, infliggi_danno, guarisci_danno};

struct Carta {
  enum Tipo_carta tipo;
  short punti_vita_inflitti;   //possibile utilizzare un solo campo per punti vita e inflitti
  struct Carta* pNext;
};

struct Mago {
  char nome_mago[256];
  short punti_vita;  
  enum Classe_mago classe;
  struct Carta* inizio_mazzo;
  struct Carta* fine_mazzo;
  struct Carta* mano[6];
  struct Carta* campo[4];
};
