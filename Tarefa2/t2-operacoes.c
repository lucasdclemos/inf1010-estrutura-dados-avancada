//Nome: Jayme Augusto Avelino de Paiva; Matrícula: 2210289
//Nome: Lucas Demarco Cambraia Lemos; Matrícula: 2110013

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "t2-operacoes.h"

// Função para exibir a árvore e os ponteiros corretamente, no percurso de pré-ordem
void exibeArvore(Node* ab){
  // Caso base -> identificar quando chegar ao final da árvore
  if(ab == NULL){
      printf("A árvore está vazia\n");
      return;
  };
  printf("Chave: %d; Node pointer: %p; Left pointer: %p; Right pointer: %p\n", ab->chave, ab, ab->esq, ab->dir);
  // Chamada recursiva para imprimir as chaves da esquerda e depois da direita, como manda o percurso pré-ordem
  if(ab->esq != NULL)
      exibeArvore(ab->esq);
  if(ab->dir != NULL)
      exibeArvore(ab->dir);
};

// Função para liberar o espaço alocado por cada nó
void libera(Node* ab){
  // Libera primeira as sub-árvores da esquerda e direita, sendo o nó raiz o último a ser liberado
  libera(ab->esq);
  libera(ab->dir);
  free(ab);
};

// Função recursiva que verifica a altura da árvore
int verificaAltura (Node* raiz){
  if (raiz == NULL){
    // Retorna -1 oois a contagem dos níveis se inicia no 0
    return -1;
  }
  int alturaEsq = verificaAltura(raiz->esq);
  int alturaDir = verificaAltura(raiz->dir);
  // Verifica qual nivel, da direita ou esquerda, é maior e retorna a altura dele + 1 para 
  // Na recursão, o retorno é incrementado em uma unidade a cada chamada
  if (alturaDir > alturaEsq){
    return (alturaDir + 1);
  }
  else{
    return (alturaEsq + 1);
  } 
}

// Função recursiva que verifica se a árvore é ABB ou não
int verificaABB (Node* raiz){
  // Caso base, para quando passar pela árvore inteira e não encontrar nenhum erro, retornando 1
  if (raiz == NULL){
    return 1;
  }
  // Verifica primeiro caso o próximo elemento exista
  if (raiz->esq != NULL){
    // Caso a chave analisada seja menor que a chave à sua esquerda, não se adequa às regras da ABB, retornando 1 imediatamente
    if (raiz->chave < raiz->esq->chave){
      return 0;
    }
  }
  // Verifica primeiro caso o próximo elemento exista
  if (raiz->dir != NULL){
    // Caso a chave analisada seja maior que a chave à sua direita, não se adequa às regras da ABB, retornando 1 imediatamente
    if (raiz->chave > raiz->dir->chave){
      return 0;
    }
  }
  return verificaABB(raiz->esq) && verificaABB(raiz->dir);
}

// Função para inverter as sub-árvores da esquerda e direita
Node* inverteArvore(Node* ab){
  if(ab == NULL)
    return ab;
  Node* aux = ab->esq;
  // Invertendo os nodes recursivamente
  ab->esq = inverteArvore(ab->dir);
  ab->dir = inverteArvore(aux);
  return ab;
};


