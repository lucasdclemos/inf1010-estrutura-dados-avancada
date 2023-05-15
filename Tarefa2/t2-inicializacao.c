//Nome: Jayme Augusto Avelino de Paiva; Matrícula: 2210289
//Nome: Lucas Demarco Cambraia Lemos; Matrícula: 2110013

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "t2-inicializacao.h"
#include "t2-operacoes.h"


// Função para gerar as chaves que serão inseridas nas árvores
void geraChaves(int* valores){
  srand(time(NULL));
  int i = 0;
  int chave;
  // Utiliza um contador i, que só é adicionado caso seja encontrado uma chave que ainda não tenha sido gerada, ou seja, ainda não esteja presente no array valores
  while(i < 10){
    chave = (rand() % 20) + 1;
    if (verificaChaves(valores, chave) == 1){
      printf("%d ", chave);
      // Adiciona o número gerado ao vetor, para garantir a não repetição das chaves
      valores[i] = chave; 
      i++;
    }
  }
  return;
}

// Função para verificar se os número aleatórios estão repetidos
int verificaChaves(int* valores, int numero){
  // Percorre o vetor para validar se o número já foi gerado ou não, retornando 1 caso não tenha sido gerado e 0 se for um valor repetido
  for (int i = 0; i<10; i++){
    if (valores[i] == numero){
      return 0;
    }
  }
  return 1;
}

// Função que retorna NULL para criar a raiz da árvore
Node *inicializa(void) { 
  return NULL; 
}

// Função para criar os nós (na inserção por nível)
Node* mkNode(int chave){
  // Aloca dinamicamente o nó na memória
  Node* node = (Node*)malloc(sizeof(Node));
  if(node == NULL){
    printf("Error: Falta de memória");
    exit(1);
  };
  // Inicializa o nó com todos os seus ponteiros apontando para NULL e sua chave com o valor passado
  node->esq = NULL; node->dir = NULL; node->chave = chave;
  return node;
};

// Função para inserção em nível
Node* insereAbNivel(Node* ab, int chave, int altura, int level){
    //check para saber se a chave conseguiu ser inserida nessa rodada da recursão
    int check = 1;
    /*
    if para inserção caso seja o primeiro elemento a ser inserido na arvore
    else if para verificar se o node da rodada tem um nivel maior que a altura da arvore,
    caso tenha retorna NULL, pois não é o lugar certo de inserção dele, já que tem
    pelo menos um node vago de nivel igual a altura
    */
    if(ab == NULL && !altura){
      return mkNode(chave);
    }else if(altura && level >= altura){
      return NULL;
    };
    //if para checar se a altura ainda não foi calculada, caso não tenha sido ele calcula
    //aqui uma única vez durante a recursão
    if(!altura){
      int alturaAtual = altura;
      Node* aux = ab;
      while(aux != NULL){
        aux = aux->dir;
        alturaAtual++;
      };
      //chamada recursiva com a altura calculada dessa vez
      insereAbNivel(ab,chave,alturaAtual,level);
    }else{
      /*
      se a recursão chegou até aqui, é pq esse node está no nível certo para inserir o 
      valor abaixo dele, primeiramente a função tenta inserir na esquerda, para não deixar
      buracos na arvore, depois tenta inserir a direita, caso não consiga tenta inserir
      na chamada recursiva da funçao com o node da esquerda, caso não consiga ela irá            
      retorna NULL e dessa forma a função será chamada recursivamente com o node de direita 
      */
      if(ab->esq == NULL){
        ab->esq = mkNode(chave);
        return ab;
      }else if(ab->dir == NULL){
        ab->dir = mkNode(chave);
        return ab;
      }else if(insereAbNivel(ab->esq,chave,altura,level+1) == NULL){
        check = (insereAbNivel(ab->dir,chave,altura,level+1) != NULL);
      };
    };
    // se o check continuar valendo 1 quer dizer que foi possível inserir o node, então ele
    // retorna a arvore, caso contrário retorna NULL
    if(check){
      return ab;
    }else{
      return NULL;
    };
}; 

// Função recursiva para inserir os elementos na Árvore Binária de Busca
Node *insereABB(Node *raiz, int chave) {
  // Verifica se a raiz é NULL, para adicionar o elemento. Caso base da recursão.
  if (raiz == NULL) {
    Node* auxiliar = mkNode(chave);
    return auxiliar;
  }
  // Compara o valor passado como parâmetro com as chaves já existentes na árvore. Caso seja maior, segue para a direita; caso contrário, segue para a esquerda
  else{
    if(chave > raiz->chave){
      raiz->dir = insereABB(raiz->dir, chave);
    }
    else if(chave < raiz->chave){
      raiz->esq = insereABB(raiz->esq, chave);
    }
  }
  return raiz;
}