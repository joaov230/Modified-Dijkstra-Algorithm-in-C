#ifndef LISTA
#define LISTA

#include <stdbool.h>

/////////////
//
// Nome: João Vítor Forgearini Beltrame
//
// Matrícula: 201713749
//
/////////////


typedef struct listav {
  int id; // Id
  bool gas; // Se tem posto de gasolina
  int peso; // Distancia da aresta
  float reabastecer;
  struct listav * prox; // Prox elemento da lista
} ListaV;

ListaV* lst_cria (void);
void lst_libera (ListaV** inicio);
int lst_vazia (ListaV* inicio);

void lst_insere_comeco (ListaV** inicio, int id, bool gas, int peso);
void lst_insere_final (ListaV** inicio, int id, bool gas, int peso);
ListaV* lst_copia_lista (ListaV* lista);

ListaV* lst_localiza_ultimo(ListaV * inicio);
bool lst_busca (ListaV* inicio, int id);

void lst_retira (ListaV** inicio, int id);

void lst_imprime (ListaV* inicio);
void lst_imprime_rec (ListaV* no);

#endif
