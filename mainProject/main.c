#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lista_v.h"
#include "grafo.h"

/////////////
//
// Nome: João Vítor Forgearini Beltrame
//
// Matrícula: 201713749
//
/////////////



char* separa_numero (char* string, int tamanho, int comeco);
void insere_vertice (FILE * fp, Grafo* grafo);
void insere_aresta (FILE * fp, Grafo* grafo);

char* separa_numero (char* string, int tamanho, int comeco) {
  int tam = (tamanho - comeco);
  char* numero = (char*) malloc ((tam) * sizeof(char));

  int i = 0;
  int j = comeco;
  while (j < tamanho) {
    numero[i] = string[j];
    i++;
    j++;
  }

  numero[j] = '\0';

  return numero;
}

// Insere um vertice
void insere_vertice (FILE* fp, Grafo* grafo) {
  char linha[50];
  char cidade1;
  bool gas;
  bool continua = true;
  int contador_vertices = 0;

  // While pra inserir os vertices
  while (continua) {
    fscanf (fp, "%s", linha);

    if (!strcmp (linha, "---")) {
      continua = false;
    } else {
      cidade1 = linha[0];
      if (linha[2] == '1') {
        gas = true;
      } else {
        gas = false;
      }
      grafo_insere_vertice (grafo, cidade1, contador_vertices, gas);
      contador_vertices++;
    }
  }
}

// Insere uma aresta
void insere_aresta (FILE * fp, Grafo* grafo) {
  char linha[50];
  char cidade1;
  char cidade2;
  int distancia;
  int tamanho_da_linha;

  // While pra inserir arestas entre os vertices
  while (fscanf (fp, "%s", linha) > 0) {

    cidade1 = linha[0];
    cidade2 = linha[2];

    tamanho_da_linha = strlen (linha);

    char* numero = separa_numero (linha, tamanho_da_linha, 4);

    sscanf (numero, "%d", &distancia);

    grafo_insere_aresta (grafo, cidade1, cidade2, distancia);

    free (numero);
  }
}

// Le as cidades que serão inseridas pelo usuário pra planejar a rota
void le_cidades (Grafo* grafo, char* inicial, char* finaleira, char* do_meio) {
  char inicio, final, intermediario;

  do {
    printf("\nInsira o nome do Vertice inicial:\n");
    scanf("%c", &inicio);
    getchar();
  } while (!existe_vertice (grafo, inicio));
  do {
    printf("\nInsira o nome do Vertice final:\n");
    scanf("%c", &final);
    getchar();
  } while (!existe_vertice (grafo, final));
  do {
    printf("\nInsira o nome do Vertice intermediario:\n");
    scanf("%c", &intermediario);
    getchar();
  } while (!existe_vertice (grafo, intermediario));

  *inicial = inicio;
  *finaleira = final;
  *do_meio = intermediario;
}


///////////////////////////////////////////////

int main (int argc, char ** argv) {
  Grafo* grafo = grafo_cria();
  FILE * fp = fopen("Mapa.txt", "r");

  if (fp == NULL) {
    printf("O arquivo não foi aberto ----- ERRO\n");
    return 0;
  }

  insere_vertice (fp, grafo);
  insere_aresta (fp, grafo);

  grafo_imprime_tab (grafo);


  printf("\n\n");
  char inicio;
  char intermediario;
  char final;

  le_cidades (grafo, &inicio, &final, &intermediario);

  int id_do_inicio = grafo_busca_id (grafo, inicio);
  int id_do_intermediario = grafo_busca_id (grafo, intermediario);
  int id_do_fim = grafo_busca_id (grafo, final);

  mapear (grafo, id_do_inicio, id_do_intermediario, id_do_fim);

  return 0;
}
