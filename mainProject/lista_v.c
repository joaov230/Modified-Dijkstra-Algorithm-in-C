
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "lista_v.h"

static ListaV * lst_localiza_ultimo_rec(ListaV* ant, ListaV * inicio);
static void insere_lista(ListaV** inicio, ListaV* ant, int id, bool gas, int peso);

// Função de criação
ListaV* lst_cria (void)
{
  ListaV* novo = (ListaV*) malloc (sizeof(ListaV));
  novo = NULL;

  return novo;
}

void lst_libera (ListaV** inicio)
{
   ListaV* p = *inicio;
   while (p != NULL) {
      ListaV* t = p->prox;
      free(p);
      p = t;
   }
   *inicio = NULL;
}


// Retorna 1 se vazia e 0 se não vazia
int lst_vazia (ListaV* inicio)
{
   return (inicio == NULL);
}

// Insere no inicio
void lst_insere_comeco (ListaV** inicio, int id, bool gas, int peso) {
   ListaV* novo = (ListaV*) malloc(sizeof (ListaV));
   novo->id = id;
   novo->gas = gas;
   novo->peso = peso;
   novo->prox = *inicio;
   *inicio = novo;
}

void lst_insere_final (ListaV** inicio, int id, bool gas, int peso) {
   ListaV* ant = lst_localiza_ultimo(*inicio);	/* ponteiro para o �ltimo elemento	*/
   insere_lista(inicio, ant, id, gas, peso);
}

static void insere_lista(ListaV** inicio, ListaV* ant, int id, bool gas, int peso) {
    /* cria novo elemento */
   ListaV* novo = (ListaV*) malloc(sizeof (ListaV));
   novo->id = id;
   novo->gas = gas;
   novo->peso = peso;
   /* encadeia elemento */
   if (ant == NULL){         /* insere elemento no in�cio	*/
      novo->prox = *inicio;
      *inicio = novo;
   }
   else {                           /* insere elemento no meio da lista */
      novo->prox = ant->prox;
      ant->prox = novo;
   }
}

// A cabeça do mapeamento recebe isso
ListaV* lst_copia_lista (ListaV* lista) {
  ListaV* novo = lst_cria ();

  while (lista != NULL) {
    lst_insere_final (&novo, lista->id, lista->gas, lista->peso);
    lista = lista->prox;
  }

  return novo;
}

ListaV * lst_localiza_ultimo(ListaV * inicio){
   ListaV* p = inicio;	/* ponteiro para percorrer a lista	*/
   ListaV* ant = NULL;	/* ponteiro para elemento anterior	*/
   /* procura posi��o de inser��o */
   while (p != NULL){
      ant = p;
      p = p->prox;
   }
   return ant;
}

static ListaV * lst_localiza_ultimo_rec(ListaV * ant, ListaV * inicio) {

   if (inicio!=NULL)
        return lst_localiza_ultimo_rec(inicio, inicio->prox);
    else return ant;
}


bool lst_busca (ListaV* inicio, int id)
{
   ListaV* p = inicio;
   while (p!=NULL)
   {
        if (p->id ==  id)
            return true;
        p = p->prox;
   }
   return false;
}



void lst_imprime (ListaV* inicio) {
   ListaV* p = inicio;
   while (p != NULL) {
      printf("ID: %d\n", p->id);
      printf("Distancia do Vertice: %d\n", p->peso);
      if (p->gas)
        printf("Tem posto de gasolina\n");
      else
        printf("Nao tem posto de gasolina\n");
      p = p->prox;
   }
}


void lst_retira (ListaV** inicio, int id) {
   ListaV* ant = NULL;
   ListaV* p = *inicio;

   while (p != NULL && p->id != id){
      ant = p;
      p = p->prox;
   }

   if (p == NULL)   return;

   if (ant == NULL)
      *inicio = p->prox;
   else
      ant->prox = p->prox;

   free(p);
}
