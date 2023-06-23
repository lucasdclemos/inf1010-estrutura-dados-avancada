#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define H 6
/*
A = 0
B = 1
C = 2
D = 3
E = 4
F = 5
H = 6
*/

typedef struct vertice Vertice;
struct vertice {
  int vertice;
  int peso;
  Vertice *prox;
};

typedef struct grafo Grafo;
struct grafo {
  int num_vertices;
  Vertice **vetor_lista;
};

Grafo *startGraph(int numVertices);
void insereGraph(Grafo *graph, int ori, int dest, int weight);
void printaGraph(Grafo *graph, int size);
void busca_dfs(Grafo *grafo);
void busca_dfs_auxiliar(Vertice **vetor_listas, bool *vetor_visitados, int vertice_inicial);

int main(void) {
    Grafo *graph = startGraph(7);
    int ori[] = {0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6};
    int dest[] = {1, 2, 3, 0, 2, 4, 6, 0, 1, 3, 4, 0, 2, 4, 5, 1, 2, 3, 5, 6, 3, 4, 6, 1, 4, 5};
    int weight[] = {5, 4, 2, 5, 6, 6, 9, 4, 6, 3, 4, 2, 3, 5, 9, 6, 4, 5, 2, 6, 9, 2, 3, 9, 6, 3};
    int size = sizeof(ori) / sizeof(ori[1]);
    for (int i = 0; i < size; i++) {
        insereGraph(graph, ori[i], dest[i], weight[i]);
    }
    printf("Grafo em lista de adjacências:\n");
    printaGraph(graph, 7);
    
    printf("\nBusca em profundidade partindo do vértice A\n");
    busca_dfs(graph);
    
    
    return 0;
}

// Função para imprimir o grafo
void printaGraph(Grafo *graph, int tam) {
    for (int i = 0; i < tam; i++) {
        printf("%c-> ", 65 + (i == 6 ? 7 : i));
        Vertice *line = *((graph->vetor_lista) + i);
        while (line) {
            printf("(%c, %d)", 65 + (line->vertice == 6 ? 7 : line->vertice), line->peso);
            line = line->prox;
            if (line)
                printf("-");
            }
        printf("\n");
    }
}

// Função para inicializar o grafo
Grafo *startGraph(int numVertices) {
  Grafo *graph = (Grafo *)malloc(sizeof(Grafo));
  if (!graph)
    exit(1);
  graph->num_vertices = numVertices;
  graph->vetor_lista = (Vertice **)malloc(sizeof(Vertice *) * numVertices);
  if (!graph->vetor_lista)
    exit(1);
  return graph;
}

// Função para inserir a aresta no grafo
void insereGraph(Grafo *graph, int ori, int dest, int weight) {
  Vertice **lista = graph->vetor_lista;
  if (!lista || !lista[ori]) {
    lista[ori] = (Vertice *)malloc(sizeof(Vertice));
    if (!lista[ori])
      exit(1);
    lista[ori]->peso = weight;
    lista[ori]->vertice = dest;
    lista[ori]->prox = NULL;
    return;
  }
  Vertice *aux = lista[ori];
  while (aux->prox != NULL) {
    aux = aux->prox;
  }
  aux->prox = malloc(sizeof(Vertice));
  aux->prox->vertice = dest;
  aux->prox->peso = weight;
  aux->prox->prox = NULL;
  return;
}

void busca_dfs(Grafo *grafo) {
    bool vetor_visitados[grafo->num_vertices];
    for (int i = 0; i < grafo->num_vertices; i++) {
        vetor_visitados[i] = false;
    }
    busca_dfs_auxiliar(grafo->vetor_lista, vetor_visitados, A);
}

void busca_dfs_auxiliar(Vertice **vetor_listas, bool *vetor_visitados, int vertice_inicial) {
    printf("dfs(%c)\n", 65 + (vertice_inicial == 6 ? 7 : vertice_inicial));
    vetor_visitados[vertice_inicial] = true;
    Vertice* vertice_atual = vetor_listas[vertice_inicial];
    while (vertice_atual){
        if (vertice_atual->prox && vetor_visitados[vertice_atual->prox->vertice] == false){
            busca_dfs_auxiliar(vetor_listas, vetor_visitados, vertice_atual->prox->vertice);
        }
        vertice_atual = vertice_atual->prox;
    }    
}
