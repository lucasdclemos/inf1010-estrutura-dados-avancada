//Nome: Jayme Augusto Avelino de Paiva; Matrícula: 2210289
//Nome: Lucas Demarco Cambraia Lemos; Matrícula: 2110013

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "t2-operacoes.h"
#include "t2-inicializacao.h"

int main(void) {
  Node* arvoreABB= inicializa();
  Node* arvorePorNivel = inicializa();
  printf("Chaves a serem inseridas: ");
  int valores[10] = {0,0,0,0,0,0,0,0,0,0};
  geraChaves(valores);
  for (int i = 0; i < 10; i++){
    arvorePorNivel = insereAbNivel(arvorePorNivel, valores[i], 0,0);
    arvoreABB = insereABB(arvoreABB, valores[i]);
  }
  printf("\n");
  /* Testes para a árvore gerada com inserção para ABB*/
  printf("---------------------------------------------------------------------");
  printf("\n");
  printf("Percurso em pré-ordem da ABB: \n\n");
  exibeArvore(arvoreABB);
  printf("\n");
  printf("Altura desta árvore: %d", verificaAltura(arvoreABB)); 
  printf("\n\n");
  if (verificaABB(arvoreABB) == 1){
   printf("É uma árvore ABB"); 
  }
  else{
    printf("Não é uma árvore ABB");
  }
  printf("\n");
  Node* invABB = inverteArvore(arvoreABB);
  printf("\nPercurso em pré-ordem desta árvore invertida:\n\n");
  exibeArvore(invABB);
  printf("---------------------------------------------------------------------\n");
  
  /* Testes para a árvore gerada com inserção por nível*/
  printf("\n");
  printf("Percurso em pré-ordem da Árvore Binária por Nível: \n\n");
  exibeArvore(arvorePorNivel);
  printf("\n");
  printf("Altura desta árvore: %d", verificaAltura(arvorePorNivel)); 
  printf("\n\n");
  if (verificaABB(arvorePorNivel) == 1){
   printf("É uma árvore ABB"); 
  }
  else{
    printf("Não é uma árvore ABB");
  }
  printf("\n");
  Node* invAbPorNivel = inverteArvore(arvorePorNivel);
  printf("\nPercurso em pré-ordem desta árvore invertida:\n\n");
  exibeArvore(invAbPorNivel);

  return 0;
}