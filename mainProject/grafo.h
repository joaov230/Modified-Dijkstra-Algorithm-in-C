#ifndef GRAFO
#define GRAFO

/////////////
//
// Nome: João Vítor Forgearini Beltrame
//
// Matrícula: 201713749
//
/////////////


typedef struct vertice Vertice;
typedef struct grafo Grafo;
typedef struct caminho Caminho;
typedef struct mapa Mapa;
typedef struct possibilidade Possibilidades;


Grafo* grafo_cria (void);

int grafo_busca_tamanho (Grafo* grafo);
bool existe_vertice (Grafo* grafo, char inicio);

int grafo_busca_id (Grafo* grafo, char nome);
char grafo_busca_nome (Grafo* grafo, int id);

void grafo_insere_vertice (Grafo* grafo, char nome, int id, bool gas);
void grafo_insere_aresta (Grafo* grafo, char orig, char dest, int peso);

void grafo_imprime_tab (Grafo * grafo);

/////////////////////////////////////////////////////
// Mapeamento

Mapa* cria_mapa (void);

void mapear (Grafo* grafo, int inicio, int intermediario, int fim);
void analisa_caminhos (Grafo* grafo, Mapa* vet, int inicio, int intermediario, int autonomia);

void analisa_caminhos_2_e_imprime (Grafo* grafo, Mapa* map1, Mapa* map2, int inicio, int intermediario, int fim);
void mapeia (Grafo* grafo, Mapa* map, int inicio, int fim);
void imprime_caminhos (Grafo* grafo, Mapa* map1, Mapa* map2, int caminho1, int caminho2);

#endif
