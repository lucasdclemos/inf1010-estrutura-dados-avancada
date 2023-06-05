// Lucas Demarco Cambraia Lemos - 2110013
// Jayme Augusto Avelino de Paiva - 2210289

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct caracLista CaracLista;
struct caracLista{
  CaracLista* prox;
  char c;
  int freq;
};

typedef struct noArvore NoArvore;
struct noArvore{
  int freq;
  char c;
  unsigned cod;
  NoArvore* esq;
  NoArvore* dir;
  NoArvore* prox;
};

typedef struct listaArvores ListaArvores;
struct listaArvores{
  NoArvore* primeiro;
  int tamanho;
};

typedef struct listaCharsComp LCC;
struct listaCharsComp{
  int tamTotal;
  int ocuByte;
  char byte;
  LCC* prox;
};

int alturaArvore(NoArvore* raiz);
CaracLista* buscaListaTemp(CaracLista* lista, char c); 
CaracLista* insereListaTemp(CaracLista* lista, char c); 
CaracLista* leArqTexto(char* nomeArquivo, CaracLista* lista); 
ListaArvores* ordenaLista(CaracLista* lista); 
ListaArvores* insereListaArvoresOrdenada(ListaArvores* lista, NoArvore* arvore); 
ListaArvores* calculaCodigo(ListaArvores* listaArvores, CaracLista* listaOrdenada);
CaracLista* insereListaOrdenada(CaracLista* lst, char c, int freq); 
NoArvore* criaArvore(int freq, char c, NoArvore* prox, NoArvore* esq, NoArvore* dir); 
NoArvore* otimizaArvore(ListaArvores* lista); 
void exibeArvore(NoArvore* ab); 
void testeImprime(CaracLista* lista); 
void liberaLista(CaracLista* lista); 
void imprimirLista(ListaArvores* lista); 
int montaCodigos(NoArvore* arv, int pos, char** matrix, char c);
char** montaCodigosCompleta(NoArvore* arv, char* nomeArquivo);
char* montaStringCompactada(char** codigo, char* nomeArquivo);
void writeBinFile(char* strCompact);
void descompacta(NoArvore* arv, char* nomeArquivoB);
void liberaMemoriaCodigos(char** codigos, NoArvore* arv);
void liberaLCC(LCC* firstByte);
void liberaArvore(NoArvore* arvore);

int main(void) {
  CaracLista* lista = NULL;
  ListaArvores* listaOrdenada = NULL;
  /* Leitura do arquivo texto  */
  lista = leArqTexto("teste.txt", lista);
  printf("\n------------LISTA DESORDENADA COM FREQUÊNCIAS E CARACTERES------------\n");
  testeImprime(lista);
  /* Ordenação da lista */
  listaOrdenada = ordenaLista(lista);
  printf("\n------------LISTA ORDENADA COM FREQUÊNCIAS E CARACTERES------------\n");
  imprimirLista(listaOrdenada);
  NoArvore* arv = otimizaArvore(listaOrdenada);
  printf("\n------------PERCURSO PRÉ-ORDEM DA ÁRVORE DE HUFFMAN------------\n");
  exibeArvore(arv);
  printf("\n------------CARACTERES E SEUS CÓDIGOS------------\n");
  char** codigos = montaCodigosCompleta(arv, "teste.txt");
  char* strCompact = montaStringCompactada(codigos, "teste.txt");
  writeBinFile(strCompact);
  printf("\n------------TEXTO LIDO DO ARQUIVO BIN E GERADO PELA DESCOMPACTAÇÃO------------\n");
  descompacta(arv, "teste.bin");
  liberaMemoriaCodigos(codigos, arv);
  free(strCompact);
  liberaArvore(arv);
  return 0;
}

// Função para ler o arquivo texto e chama função de inserir os caracteres na lista dos caracteres
CaracLista* leArqTexto(char* nomeArquivo, CaracLista* lista){
  char c;
  FILE *f = fopen(nomeArquivo, "r");
  if (f == NULL){
    printf("Erro ao abrir arquivo");
    return NULL;
  }
  while((c = fgetc(f)) != EOF){
    lista = insereListaTemp(lista, c);
  }
  // Adicionamos um caractere * para marcar o final do arquivo na hora da descompactação
  lista = insereListaTemp(lista, '*');
  return lista;
}

// Função para inserir os caracteres na lista
CaracLista* insereListaTemp(CaracLista* lista, char c){
  // Verifica na lista existente se esse caractere já está presente lá
  CaracLista* auxiliar = buscaListaTemp(lista, c);
  // Caso de percorrer a lista inteira e não encontrar o caractere, então adiciona o novo caractere no início da lista
  if (auxiliar == NULL){ 
    CaracLista* novoElem = (CaracLista*)malloc(sizeof(CaracLista));
    if (novoElem == NULL){
      printf("Erro de malloc");
      return NULL;
    }
    novoElem->c = c;
    novoElem->prox = lista;
    novoElem->freq = 1;
    return novoElem;
  }
  // Caso de encontrar o caractere na lista, irá somar a frequência do nó já existente 
  else{
    auxiliar->freq++;
    return lista;
  }
}

// Função para buscar um caractere na lista de caracteres
CaracLista* buscaListaTemp(CaracLista* lista, char c){
  CaracLista* auxiliar;
  for (auxiliar = lista; auxiliar != NULL; auxiliar = auxiliar->prox){
    // Caso de encontrar o caractere, retorna um ponteiro para o nó que contém o caractere procurado
    if (auxiliar->c == c){
      return auxiliar;
    }
  }
  // Caso percorra a lista inteira, retorna NULL, representando que não encontrou o elemento
  return NULL;
}

// Função chamada que percorre a lista original e chama a função ordena essa lista
ListaArvores* ordenaLista(CaracLista* lista){
  CaracLista* auxiliarInsercao;
  CaracLista* auxiliarListaArvores;
  CaracLista* listaOrdenada = NULL;
  // Percorre a lista original e cria uma nova lista ordenada de forma crescente com base na frequência
  for(auxiliarInsercao = lista; auxiliarInsercao != NULL; auxiliarInsercao = auxiliarInsercao->prox){
    listaOrdenada = insereListaOrdenada(listaOrdenada, auxiliarInsercao->c, auxiliarInsercao->freq);
  }
  // Cria uma lista de árvores, em que cada nó é uma árvore binária
  ListaArvores* listaArvores = (ListaArvores*)malloc(sizeof(ListaArvores));
  if (listaArvores == NULL){
    printf("Erro de malloc");
    return NULL;
  }
  listaArvores->primeiro = NULL;
  listaArvores->tamanho = 0;
  for(auxiliarListaArvores = listaOrdenada; auxiliarListaArvores != NULL; auxiliarListaArvores = auxiliarListaArvores->prox){
    // Chama função criaArvore, inicializando os ponteiros esq, dir e prox como NULL
    NoArvore* arvore = criaArvore(auxiliarListaArvores->freq, auxiliarListaArvores->c, NULL, NULL, NULL);
    // Essa árvore é passada como parâmetro para a função que insereas árvores ordenadas nas listas
    listaArvores = insereListaArvoresOrdenada(listaArvores, arvore);
  }
  // Libera a memória das listas que não serão mais utilizadas
  liberaLista(lista);
  liberaLista(listaOrdenada);
  return listaArvores;
}

// Função que retorna uma lista ordenada, com base na lista original
CaracLista* insereListaOrdenada(CaracLista* lst, char c, int freq){
  CaracLista* novo;
  CaracLista* ant = NULL;
  CaracLista* p = lst;
  while(p != NULL && p->freq < freq){
    ant = p;
    p = p->prox;
  }
  novo = (CaracLista*)malloc(sizeof(CaracLista));
  if (novo == NULL){
    printf("Erro de malloc");
    return NULL;
  }
  novo->freq = freq;
  novo->c = c;
  // Caso de inserir no início da lista
  if(ant == NULL){
    novo->prox = lst;
    lst = novo;
  }
  // Caso de inserir no meio da lista, procura posição correta para inserir
  else{
    novo->prox = ant->prox;
    ant->prox = novo;
  }
  return lst;
}

// Função que cria as árvores com um nó para inserir na lista de árvores
NoArvore* criaArvore(int freq, char c, NoArvore* prox, NoArvore* esq, NoArvore* dir){
  NoArvore* novaArvore = (NoArvore*)malloc(sizeof(NoArvore));
  if (novaArvore == NULL){
    printf("Erro de malloc");
    return NULL;
  }
  novaArvore->c = c;
  novaArvore->freq = freq;
  novaArvore->cod = 0;
  novaArvore->dir = dir;
  novaArvore->esq = esq;
  novaArvore->prox = prox;
  return novaArvore;
}

// Função que insere as árvores de somente um nó na lista de árvores
ListaArvores* insereListaArvoresOrdenada(ListaArvores* lista, NoArvore* arvore){
  NoArvore* auxiliar;
  if(lista->primeiro == NULL){
    lista->primeiro = arvore;
  }else if(arvore->freq < lista->primeiro->freq){
    arvore->prox = lista->primeiro;
    lista->primeiro = arvore;
  }else{
    auxiliar = lista->primeiro;
    while(auxiliar->prox != NULL && auxiliar->prox->freq <= arvore->freq){
      auxiliar = auxiliar->prox;
    }
    if(auxiliar->prox != NULL){
      arvore->prox = auxiliar->prox; 
    }
    auxiliar->prox = arvore;
  }
  lista->tamanho++;
  return lista;
}

// Função para retirar o menor nó de uma lista de árvores
NoArvore* retiraMenorNo(ListaArvores* lista){
  NoArvore* auxiliar;
  if(lista->primeiro != NULL){
    auxiliar = lista->primeiro;
    lista->primeiro = auxiliar->prox;
    auxiliar->prox = NULL;
    lista->tamanho--;
  }  
  return auxiliar;
}

// Função para otimizar a árvore (juntar as árvores, até que sobre apenas uma)
NoArvore* otimizaArvore(ListaArvores* lista){
  while(lista->tamanho > 1){
    NoArvore* menorElem1 = retiraMenorNo(lista);
    NoArvore* menorElem2 = retiraMenorNo(lista);
    NoArvore* novo = (NoArvore*)malloc(sizeof(NoArvore));
    if(novo == NULL){
      printf("Erro de malloc\n");
      return NULL;
    }else{
      // If para definir qual menorElem ficará na esquerda --> a menor frequência fica na esquerda
      if(menorElem1->freq < menorElem2->freq){
        novo = criaArvore(menorElem1->freq + menorElem2->freq, '~', NULL, menorElem1, menorElem2);
      }else{
        novo = criaArvore(menorElem1->freq + menorElem2->freq, '~', NULL, menorElem2, menorElem1);
      }
      lista = insereListaArvoresOrdenada(lista, novo);
    }
  }
  return lista->primeiro;
}

// Função que monta os códigos de um char e coloca ele numa matriz onde o indice da linha equivale ao char e o conteudo da linha é o código
int montaCodigos(NoArvore* arv, int pos, char** matrix, char c){
  if (arv->c == c){
    *(*(matrix+(int)c)+pos) = '\0';
    return 1;
  }else if (!(arv->dir) && !(arv->esq))
    return 0;
  if (montaCodigos(arv->esq, pos+1, matrix, c)){
    *(*(matrix+(int)c)+pos) = '0';
    return 1;
  }else if (montaCodigos(arv->dir, pos+1, matrix, c)){
    *(*(matrix+(int)c)+pos) = '1';
    return 1;
  }
  return 0;
}

//função auxiliar para saber a altura da arvore
int alturaArvore(NoArvore* raiz){
  if (!raiz)
    return 0;
  int altEsq = 1+alturaArvore(raiz->esq);
  int altDir = 1+alturaArvore(raiz->dir);
  if (altDir > altEsq)
    return altDir;
  return altEsq;
}

//função para montar o código de todos os chars que aparecem no texto chamando a montaCodigos
char** montaCodigosCompleta(NoArvore* arv, char* nomeArquivo){
  int altura = alturaArvore(arv);

  CaracLista* listaAux = NULL; listaAux = leArqTexto(nomeArquivo, listaAux);
  int tam = 0;
  CaracLista* current = listaAux;
  while (current){
    tam++;
    current = current->prox;
  }
  current = listaAux;
  char letras[tam+1];
  int pos = 0;
  while (current){
    *(letras+pos) = current->c;
    pos++;
    current = current->prox;
  }

  char** codigos = (char**)malloc(255*sizeof(char*));
  if (!codigos){
    printf("ERROR: Falta de memória\n");
    exit(1);
  }
  for (int i = 0; i < 255; i++){
    *(codigos+i) = (char*)malloc((altura+1)*sizeof(char));
    if (!(*(codigos+i))){
      printf("ERROR: Falta de memória\n");
      exit(1);
    }
  }
  
  for (int i = 0; i<tam; i++){
    montaCodigos(arv, 0, codigos, letras[i]);
    printf("%c -> ", letras[i]);
    char* bit = *(codigos+(int)letras[i]);
    while (*bit != '\0'){
      printf("%c", *bit);
      bit++;
    }
    printf("\n");
  }
  liberaLista(listaAux);
  return codigos;
}

//função que monta a string equivalente ao texto compactado seguindo os códigos da arvore
char* montaStringCompactada(char** codigo, char* nomeArquivo){
  char c;
  FILE *f = fopen(nomeArquivo, "r");
  LCC* firstByte = (LCC*)malloc(sizeof(LCC));
  if (!firstByte){
    printf("ERROR: Falta de memória\n");
    exit(1);
  }
  firstByte->byte = 0;
  firstByte->ocuByte = 0;
  firstByte->tamTotal = 0;
  firstByte->prox = NULL;
  LCC* current = firstByte;
  LCC* ant;
  while((c = fgetc(f)) != EOF){
    char* cod = *(codigo+(int)c);
    int tam = strlen(cod);
    int posBit = 0;
    while (posBit < tam){
      char bit = *(cod+posBit)-48;
      bit = bit<<(7-current->ocuByte);
      current->byte |= bit;
      current->ocuByte++;
      posBit++;
      if (current->ocuByte == 8){
        LCC* prox = (LCC*)malloc(sizeof(LCC));
        if (!prox){
          printf("ERROR: Falta de memória\n");
          exit(1);
        }
        prox->byte = 0;
        prox->ocuByte = 0;
        prox->tamTotal = 0;
        prox->prox = NULL;
        ant = current;
        current->prox = prox;
        current = current->prox;
      }
    }
  }
  if (current->ocuByte == 0){
    LCC* auxiliar = current;
    current = firstByte;
    free(auxiliar);
    ant->prox = NULL;
  }
  //adicionando um char para servir de indicação que acabou o conteúdo a ser descompactado no caso dele possuir um número de bits que não é múltiplo de 8
  if (current->ocuByte < 8){
    int bitsFaltando = 8 - current->ocuByte;
    char* aux = *(codigo+(int)'*');
    for (int cont = 0; cont<bitsFaltando; cont++){
      char bit = *(aux+cont)-48;
      bit = bit<<(7-current->ocuByte);
      current->byte |= bit;
      current->ocuByte++;
    }
  }
  int tam = 0;
  current = firstByte;
  while(current){
    tam++;
    current = current->prox;
  }
  current = firstByte;
  char* stringComp = (char*)malloc(tam*sizeof(char)+1);
  for(int pos = 0; pos<tam; pos++){
    *(stringComp+pos) = current->byte;
    current = current->prox;
  }
  *(stringComp+tam) = '\0';
  current = firstByte;
  liberaLCC(firstByte);
  return stringComp;
}

// Escrevendo arquivo binário
void writeBinFile(char* strCompact){
  FILE* arquivo = fopen("teste.bin", "wb");
    if (arquivo == NULL) {
        printf("ERRO: Não foi possível abrir o arquivo de escrita.\n");
        exit(1);
    }
    size_t tamanho = strlen(strCompact);
    fwrite(strCompact, sizeof(char), tamanho, arquivo);
    fclose(arquivo);
}

//descompactando arquivo binário
void descompacta(NoArvore* arv, char* nomeArquivoB){
  FILE* arquivoB = fopen("teste.bin", "rb");
  if (!arquivoB){
    printf("ERROR: Não foi possível abrir arquivo compactado\n");
    exit(1);
  }
  
  LCC* strDescomp = (LCC*)malloc(sizeof(LCC));
  strDescomp->byte = 0;
  strDescomp->prox = NULL;
  char c;

  LCC* current = strDescomp;
  NoArvore* currentNode = arv;
  while((c=fgetc(arquivoB))!=EOF){
    int bitPos = 0;
    while(bitPos < 8){
      char aux = c;
      aux = (aux<<bitPos)&(0x80);
      bitPos++;
      if (!aux){
        currentNode = currentNode->esq;
      }else{
        currentNode = currentNode->dir;
      }
      if(!(currentNode->esq) && !(currentNode->dir)){
        if (currentNode->c == '*')
          break;
        current->byte = currentNode->c;
        current->prox = malloc(sizeof(LCC));
        current->prox->byte = 0;
        current->prox->prox = NULL;
        current = current->prox;
        currentNode = arv;
      }
    }
  }
  current = strDescomp;
  int tam = 0;
  while (current){
    tam++;
    current = current->prox;
  }
  current = strDescomp;
  char strLida[tam+1];
  for(int pos = 0; pos<tam; pos++){
    *(strLida+pos) = current->byte;
    current = current->prox;
  }
  *(strLida+tam) = '\0';
  
  FILE* arquivo = fopen("testeDescompactado.txt", "w");
  if (arquivo == NULL) {
      printf("ERRO: Não foi possível abrir o arquivo de escrita.\n");
      exit(1);
  }
  size_t tamanho = strlen(strLida);
  fwrite(strLida, sizeof(char), tamanho, arquivo);
  fclose(arquivo);
  printf("String lida do arquivo comprimido:\n");
  printf("%s\n", strLida);
}

// Função para liberar a memória alocada para a matriz
void liberaMemoriaCodigos(char** codigos, NoArvore* arv){
  for(int i = 0; i<255; i++){
    free(*(codigos+i));
  }
  free(codigos);
}

// Função para exibir a árvore e os ponteiros corretamente, no percurso de pré-ordem
void exibeArvore(NoArvore* ab){
  // Caso base -> identificar quando chegar ao final da árvore
  if(ab == NULL){
      printf("A árvore está vazia\n");
      return;
  };
  printf("Frequência: %d - Caractere: %c\n", ab->freq, ab->c);
  // Chamada recursiva para imprimir as chaves da esquerda e depois da direita, como manda o percurso pré-ordem
  if(ab->esq != NULL)
      exibeArvore(ab->esq);
  if(ab->dir != NULL)
      exibeArvore(ab->dir);
};

// Função para imprimir a lista de árvores com apenas a raiz
void imprimirLista(ListaArvores* lista) {
  NoArvore* aux = lista->primeiro;
  printf("Tamanho: %d\n", lista->tamanho);
  while (aux != NULL) {
      printf("%c - %d\n", aux->c, aux->freq);
      aux = aux->prox;
  }
}

// Função para liberar o espaço da memória da lista
void liberaLista(CaracLista* lista){
  CaracLista* p = lista;
  while(p != NULL){
    CaracLista* temp = p->prox;
    free(p);
    p = temp;
  }
}

// Função para imprimir a lista desordenada temporária
void testeImprime(CaracLista* lista){
  CaracLista* auxiliar;
  for(auxiliar = lista; auxiliar != NULL; auxiliar = auxiliar->prox){
    printf("%c - %d\n", auxiliar->c, auxiliar->freq);
  }
}

// Função para liberar a memória da Lista de Char
void liberaLCC(LCC* firstByte){
  if (!firstByte)
    return;
  liberaLCC(firstByte->prox);
  free(firstByte);
}

void liberaArvore (NoArvore* arvore){
  if (arvore == NULL){
    return;
  }
  liberaArvore(arvore->esq);
  liberaArvore(arvore->dir);
  free(arvore);
}



