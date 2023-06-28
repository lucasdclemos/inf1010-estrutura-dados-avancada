#include <stdio.h>
#include <stdlib.h>

#define INFINITY 2147483647

typedef struct adjList AdjList;
struct adjList{
    int node;
    int weight;
    AdjList* prox;
};
typedef struct graph Graph;
struct graph{
    int qtdNodes;
    AdjList** adj;
};
typedef struct pilha Pilha;
struct pilha{
    int elem;
    Pilha* prox;
};

// Função para inicializar o grafo
Graph* makeGraph(){
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph){
        printf("ERROR: Falta de memória\n");
        exit(1);
    }
    AdjList** adj = (AdjList**)malloc(7*sizeof(AdjList*));
    if (!adj){
        printf("ERROR: Falta de memória\n");
        exit(1);
    }
    int arestaPeso[7][7] = {
        {0,5,4,2,0,0,0},
        {5,0,6,0,6,0,9},
        {4,6,0,3,4,0,0},
        {2,0,3,0,5,9,0},
        {0,6,4,5,0,2,6},
        {0,0,0,9,2,0,3},
        {0,9,0,0,6,3,0}
    };
    for (int i = 0; i<7; i++){
        *(adj+i) = (AdjList*)malloc(sizeof(AdjList));
        if (!(*(adj+i))){
            printf("ERROR: Falta de memória\n");
            exit(1);
        }
        AdjList* atual = *(adj+i);
        AdjList* ant = atual;
        for (int j = 0; j<7; j++){
            if (arestaPeso[i][j] != 0){
                atual->node = j;
                atual->weight = arestaPeso[i][j];
                ant = atual;
                atual->prox = (AdjList*)malloc(sizeof(AdjList));
                if (!(atual->prox)){
                    printf("ERROR: Falta de memória\n");
                    exit(1);
                }
                atual = atual->prox;
            }
        }
        free(atual);
        ant->prox = NULL;
    }
    graph->qtdNodes = 7;
    graph->adj = adj;
    return graph;
}

// Função que realiza o cálculo das distâncias, usando o Algoritmo de Djikstra
int* djikstra(Graph* graph, int initialNode){
    int* dist = (int*)malloc(sizeof(int)*(graph->qtdNodes));
    if (!dist){
        printf("ERROR: Falta de memória\n");
        exit(1);
    }
    int visitados[graph->qtdNodes];
    for(int i=0; i<graph->qtdNodes;i++){
        *(dist+i) = INFINITY;
        visitados[i] = 0;
    }
    *(dist+initialNode) = 0;
    int qtdVisitas = 0;
    int actualNode = initialNode;
    while(qtdVisitas < graph->qtdNodes){
        qtdVisitas++;
        visitados[actualNode] = 1;
        AdjList* actual = *((graph->adj)+actualNode);
        while (actual){
            *(dist+actual->node) = (actual->weight + *(dist+actualNode) < *(dist+actual->node) ? actual->weight + *(dist+actualNode) : *(dist+actual->node));
            actual = actual->prox;
        }
        int menorDist = INFINITY;
        for (int i=0; i<graph->qtdNodes; i++){
            if (!visitados[i] && *(dist+i) <= menorDist){
                menorDist = *(dist+i);
                actualNode = i;
            }
        }
    }
    return dist;
}

// Função para realizar a busca em profundidade
void dfs(Graph* graph, Pilha* pilha, int* visitados){
    if (!pilha){
        printf("\n");
        return;
    }
    printf("%c ", pilha->elem == 6 ? pilha->elem+1+65 : pilha->elem+65);
    Pilha* newPile = (Pilha*)malloc(sizeof(Pilha));
    if (!newPile){
        printf("ERROR: Falta de memória\n");
        exit(1);
    }
    newPile->prox = pilha->prox;
    AdjList* adj = *((graph->adj)+pilha->elem);
    free(pilha);
    while(adj){
        if (!(*(visitados+adj->node))){
            *(visitados+adj->node) = 1;
            newPile->elem = adj->node;
            Pilha* newNewPile = (Pilha*)malloc(sizeof(Pilha));
            if (!newNewPile){
                printf("ERROR: Falta de memória\n");
                exit(1);
            }   
            newNewPile->prox = newPile;
            newPile = newNewPile;
        }
        adj = adj->prox;
    }
    pilha = newPile->prox;
    free(newPile);
    return dfs(graph,pilha,visitados);
}

int main(void){
    Graph* graph = makeGraph();
    int* dist = djikstra(graph, 0);
    printf("Resultado das distâncias calculadas usando djikstra:\n");
    for (int i=0; i<graph->qtdNodes;i++){
        printf("dist(%c,%c)=%d\n",65,65+(i == 6 ? i+1 : i),*(dist+i));
    }
    printf("Dfs do grafo:\n");
    Pilha* pilha = (Pilha*)malloc(sizeof(Pilha));
    if (!pilha){
        printf("ERROR: Falta de memoria");
        exit(1);
    }
    pilha->elem = 0;
    pilha->prox = NULL;
    int visitados[7] = {1,0,0,0,0,0,0};
    dfs(graph,pilha,visitados);
    return 0;
}