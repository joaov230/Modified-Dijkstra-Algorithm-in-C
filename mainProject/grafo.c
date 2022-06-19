#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "grafo.h"
#include "lista_v.h"

struct vertice {
  char nome;
  int id;
  bool gas;
  float reabastece;
  ListaV * cabeca;
};

struct grafo {
  Vertice* vetor;
  int tamanho; // Tamanho do vetor
  int arestas;
};

// Vetor de caminhos possiveis
struct caminho {
  bool impossivel; // true - se o carro não consegue chegar até o destino, false - se consegue
  int dist; // Pra salvar a distancia total desse caminho
  int autonomia; // É usado como parametro pra limitar o quão longe o carro pode percorrer
  ListaV* cabeca;
};

struct mapa {
  Caminho* vetor;
  int tam; // Quantidade de caminhos
};

// Todas as possibilidades dos caminhos depois de serem analisadas (será usado num vetor de possibilidades)
struct possibilidade {
  int primeiro;
  int segundo;
  int dist_total;
  bool impossivel;
};


bool tudo_impossivel (Possibilidades* caminho, int tam);
bool existe_aberto (Grafo* grafo, bool* aberto);
void encontra_caminho (Grafo* grafo, ListaV* lista_aux, int peso, Mapa* vet, int* aberto, int atual, int fim);


// Cria um grafo
Grafo* grafo_cria (void) {
  Grafo* grafo = (Grafo*) malloc (sizeof (Grafo));

  grafo->vetor = (Vertice*) malloc (sizeof (Vertice*));
  grafo->vetor[0].cabeca = NULL;
  grafo->tamanho = 0;
  grafo->arestas = 0;
  grafo->vetor[0].reabastece = 0;

  return grafo;
}

int grafo_busca_tamanho (Grafo* grafo) {
  return grafo->tamanho;
}

// Se existe o vertice baseado no nome recebido por parametro
bool existe_vertice (Grafo* grafo, char inicio) {
  int i;
  for (i = 0; i < grafo->tamanho; i++) {
    if (inicio == grafo->vetor[i].nome)
      return true;
  }
  return false;
}

// Retorna o id atraves o nome
int grafo_busca_id (Grafo* grafo, char nome) {
  int id = 0;
  for (id = 0; id < grafo->tamanho; id++) {
    if (grafo->vetor[id].nome == nome) {
      return id;
    }
  }
  return -1;
}

// Retorna o nome atraves do id
char grafo_busca_nome (Grafo* grafo, int id) {
  return grafo->vetor[id].nome;
}

// Insere um vertice novo
void grafo_insere_vertice (Grafo* grafo, char nome, int id, bool gas) {
  // Dá realloc pra inserir mais um vertice
  if (id > 0) {
    grafo->vetor = (Vertice*) realloc (grafo->vetor, (grafo->tamanho+1) * sizeof(Vertice));
  }

  // Bota a cabeca da lista como NULL
  int i;
  for (i = grafo->tamanho; i < id; i++) {
    grafo->vetor[i].cabeca = NULL;
  }
  grafo->tamanho += 1;

  grafo->vetor[id].reabastece = 0;
  grafo->vetor[id].id = id;
  grafo->vetor[id].nome = nome;
  grafo->vetor[id].gas = gas;
}


// Insere uma aresta entre dois vertices
void grafo_insere_aresta (Grafo* grafo, char orig, char dest, int peso) {
  // Pega o id do vertice baseado no nome
  int origem = grafo_busca_id (grafo, orig);
  int destino = grafo_busca_id (grafo, dest);

  // Verifica se o id é maior que o tamanho do grafo
  if (origem >= grafo->tamanho || destino >= grafo->tamanho) {
    printf("Os vertices que voce quer ligar nao existem no grafo\n");
    return;
  }

  // Insere na lista da origem e do destino (grafo não direcionado)
  lst_insere_comeco (&grafo->vetor[origem].cabeca, destino, grafo->vetor[destino].gas, peso);
  lst_insere_comeco (&grafo->vetor[destino].cabeca, origem, grafo->vetor[origem].gas, peso);
}

// Verifica se tem conexão entre vertice1 e vertice2
// Se tem, retorna o peso da aresta
// Senão, retorna -1
int grafo_busca_aresta(Grafo * grafo, Vertice vertice1, Vertice vertice2){
  ListaV * aresta = vertice1.cabeca;
  while (aresta != NULL) {
    if (aresta->id == vertice2.id) {
      return aresta->peso;
    }
    aresta = aresta->prox;
  }
  return -1;
}


// Imprime um grafo na tela
// Percorre as arestas do id e imprime
// Passa pro proximo id e repete
void grafo_imprime_tab (Grafo * grafo) {
  int id;
  int i;
  ListaV* aresta;
  for (id = 0; id < grafo->tamanho; id++) {
    printf ("\n");
    aresta = grafo->vetor[id].cabeca;

    printf("%c (Gas: %d) --> ", grafo->vetor[id].nome, grafo->vetor[id].gas);

    while (aresta != NULL) {
      printf(" %c (%d) ", grafo_busca_nome(grafo, aresta->id), aresta->peso);

      aresta = aresta->prox;

      if (aresta != NULL) {
        printf(" -- ");
      }
    }
  }
  printf ("\n\n");
}


///////////////////////////////////////////////////
// MAPEAMENTO
/////////////////////////////////////////////////

Mapa* cria_mapa (void) {
  Mapa* map = (Mapa*) malloc (sizeof(Mapa)); // Vai ser usado pra guardar as listas copiadas
  map->vetor = (Caminho*) malloc (sizeof(Caminho)); // Aloca espaço no vetor de listas
  map->tam = 0; // Tamanho passa a ser 1
  map->vetor[0].impossivel = false;
  map->vetor[0].dist = 0;
  map->vetor[0].cabeca = NULL; // Coloca a cabeça da lista como NULL

  return map;
}

void copia_lista_pro_vetor (Mapa* map, ListaV* lista) {
  map->tam += 1;
  map->vetor = (Caminho*) realloc (map->vetor, map->tam * sizeof(Caminho));
  int ult_pos = map->tam - 1;
  map->vetor[ult_pos].cabeca = lst_copia_lista (lista);
}

// Encontra todos os caminhos possiveis do vertice inicial até o final (fim)
void encontra_caminho (Grafo* grafo, ListaV* lista_aux, int peso, Mapa* map, int* aberto, int atual, int fim) {
  // A condição e parada é quando, em último caso, chegar no ponto em que todos os vertices adjacentes
    // estao fechados

  // Se tá aberto, bota na lista e verifica oq fazer dps
  if (aberto[atual]) {

    lst_insere_final (&lista_aux, atual, grafo->vetor[atual].gas, peso);
    if (atual == fim) {
      copia_lista_pro_vetor (map, lista_aux); // Copia a lista aqui pra mais uma posicao do Caminho* vet (realloc)
      lst_retira (&lista_aux, atual); // Retira o atual da lista_aux
      return;
    }

    // Fecha o vertice atual
    aberto[atual] = 0;

    // Percorre os adjacentes
    ListaV* v_adj = grafo->vetor[atual].cabeca;
    while (v_adj != NULL) {
      encontra_caminho (grafo, lista_aux, v_adj->peso, map, aberto, v_adj->id, fim);
      v_adj = v_adj->prox;
    }

    // Abre novamente, retira da lista auxiliar e continua por outro caminho
    aberto[atual] = 1;
    lst_retira (&lista_aux, atual);
    return;

  }
}

// Mapeia UM mapa
void mapeia (Grafo* grafo, Mapa* map, int inicio, int fim) {
  ListaV* list_aux = lst_cria (); // Cria a lista pra verificar possiveis caminhos
  int* aberto = (int*) malloc (grafo->tamanho * sizeof(int)); // Pra verificar se tal vertice tá aberto

  // Abre todas as posições do vetor
  int i;
  for (i = 0; i < grafo->tamanho; i++) {
    aberto[i] = 1;
  }

  // Acha os caminhos
  encontra_caminho (grafo, list_aux, 0, map, aberto, inicio, fim);
}

// Analisa os caminhos da primeira cidade até a intermediaria
void analisa_caminhos (Grafo* grafo, Mapa* map, int inicio, int fim, int autonomia) {

  int distancia = 0;
  int atnm = autonomia;

  ListaV* aux;
  int i;
  for (i = 0; i < map->tam; i++) {
    aux = map->vetor[i].cabeca;

    while (aux != NULL) {
      distancia += aux->peso; // Soma a distancia com o peso da aresta do caminho

      aux->reabastecer = 0;

      atnm += aux->peso;


      if (aux->gas) { // Se tem posto de gasolina, autonomia vai pra 0
        if (atnm > 100) {
          map->vetor[i].impossivel = true;
          break;
        }
        aux->reabastecer = (float) atnm / 10;
        atnm = 0;
      }

      if (atnm > 100) { // Se for impossivel de ir por esse caminho
        map->vetor[i].impossivel = true;
        break;
      } else {
        map->vetor[i].impossivel = false;
      }

      aux = aux->prox;
    }
    map->vetor[i].dist = distancia;
    map->vetor[i].autonomia = atnm; // Autonomia da ultima posição
    // Reinicia a autonomia e distancia pro proximo caminho
    distancia = 0;
    atnm = autonomia;

  }
}

// Faz tudo
void mapear (Grafo* grafo, int inicio, int intermediario, int fim) {
  int autonomia = 0;
  // Acha o caminho do inicio até a cidade intermediaria
  Mapa* map1 = cria_mapa ();
  mapeia (grafo, map1, inicio, intermediario);
  analisa_caminhos (grafo, map1, inicio, intermediario, 0);


  // Acha o caminho da cidade intermediaria até o fim e analisa a melhor combinação
  Mapa* map2 = cria_mapa ();
  mapeia (grafo, map2, intermediario, fim);
  analisa_caminhos_2_e_imprime (grafo, map1, map2, inicio, intermediario, fim);
}

// Analisa o segundo caminho
void analisa_caminhos_2_e_imprime (Grafo* grafo, Mapa* map1, Mapa* map2, int inicio, int intermediario, int fim) {

  int i;
  int j=0;
  int k = 0;
  int tam = (map1->tam+1) * (map2->tam+1); // +1, pois começa em 0


  Possibilidades* caminho = (Possibilidades*) malloc (tam * sizeof (Possibilidades));

  for (i = 0; i < tam; i++) {
    caminho[i].impossivel = true;
  }

  // For q percorre o map1
  for (i = 0; i < map1->tam; i++) {
    // for que percorre o map2 pra cada posicão do map1
    analisa_caminhos (grafo, map2, intermediario, fim, map1->vetor[i].autonomia);
    for (j = 0; j < map2->tam; j++) {
      caminho[k].primeiro = i;
      caminho[k].segundo = j;
      caminho[k].dist_total = map1->vetor[i].dist + map2->vetor[j].dist;
      caminho[k].impossivel = map2->vetor[j].impossivel;
      k++;
    }
  }

    int menor = -1;
    int prim;
    int seg;

    for (i = 0; i < tam; i++) {
      if ((caminho[i].dist_total < menor || menor == -1) && !(caminho[i].impossivel)) {
        menor = caminho[i].dist_total;
        prim = caminho[i].primeiro;
        seg = caminho[i].segundo;
      }
    }

    if (tudo_impossivel(caminho,tam)) {
      printf("Nao existem postos de combustivel suficientes pra levar o carro ate o destino\n");
      return;
    }

    imprime_caminhos (grafo, map1, map2, prim, seg);
}

// Verifica se todos os caminhos são impossíveis
bool tudo_impossivel (Possibilidades* caminho, int tam) {
  int cont = 0;
  int i;

  for (i = 0; i < tam; i++) {
    if (caminho[i].impossivel) {
      cont++;
    }
  }

  if (cont == tam)
    return true;
  else
    return false;
}

void imprime_caminhos (Grafo* grafo, Mapa* map1, Mapa* map2, int id_caminho1, int id_caminho2) {
  if (map1->vetor[0].cabeca == NULL || map2->vetor[0].cabeca == NULL) {
    printf("Nao existe conexão entre as cidades\n");
    return;
  }

  ListaV* c1 = map1->vetor[id_caminho1].cabeca;

  while (c1 != NULL) {
    if (c1->reabastecer == 0) {
      printf("cidade %c (sem reabastecer), ", grafo_busca_nome (grafo, c1->id));
    } else {
      printf("cidade %c (para reabastecer %.1f litros), ", grafo_busca_nome (grafo, c1->id), c1->reabastecer);
    }
    c1 = c1->prox;
  }

  ListaV* c2 = map2->vetor[id_caminho2].cabeca;

  c2 = c2->prox;
  while (c2 != NULL) {
    if (c2->reabastecer == 0) {
      if (c2->prox == NULL) {
        printf("cidade %c (sem reabastecer)\n", grafo_busca_nome (grafo, c2->id));
      } else {
        printf("cidade %c (sem reabastecer), ", grafo_busca_nome (grafo, c2->id));
      }
    } else {
      if (c2->prox == NULL) {
        printf("cidade %c (para reabastecer %.1f litros)\n", grafo_busca_nome (grafo, c2->id), c2->reabastecer);
      } else {
        printf("cidade %c (para reabastecer %.1f litros), ", grafo_busca_nome (grafo, c2->id), c2->reabastecer);
      }
    }
    c2 = c2->prox;
  }
}
