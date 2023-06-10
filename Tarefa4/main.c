// Lucas Demarco Cambraia Lemos - 2110013
// Jayme Augusto Avelino de Paiva - 2210289

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct caracLista CaracLista;
struct caracLista {
  CaracLista *prox;
  char c;
  int freq;
};

typedef struct noArvore NoArvore;
struct noArvore {
  int freq;
  char c;
  unsigned cod;
  NoArvore *esq;
  NoArvore *dir;
  NoArvore *prox;
};

typedef struct listaArvores ListaArvores;
struct listaArvores {
  NoArvore *primeiro;
  int tamanho;
};

typedef struct listaCharsComp LCC;
struct listaCharsComp {
  int tamTotal;
  int ocuByte;
  char byte;
  LCC *prox;
};

typedef struct stri Stri;
struct stri {
  char c;
  Stri *prox;
};

int alturaArvore(NoArvore *raiz);
CaracLista *buscaListaTemp(CaracLista *lista, char c);
CaracLista *insereListaTemp(CaracLista *lista, char c);
CaracLista *leArqTexto(char *nomeArquivo, CaracLista *lista);
ListaArvores *ordenaLista(CaracLista *lista);
ListaArvores *insereListaArvoresOrdenada(ListaArvores *lista, NoArvore *arvore);
ListaArvores *calculaCodigo(ListaArvores *listaArvores, CaracLista *listaOrdenada);
ListaArvores* readHuffmanTreeFile ();
CaracLista *insereListaOrdenada(CaracLista *lst, char c, int freq);
NoArvore *criaArvore(int freq, char c, NoArvore *prox, NoArvore *esq, NoArvore *dir);
NoArvore *otimizaArvore(ListaArvores *lista);
void exibeArvore(NoArvore *ab);
void testeImprime(CaracLista *lista);
void liberaLista(CaracLista *lista);
void imprimirLista(ListaArvores *lista);
int montaCodigos(NoArvore *arv, int pos, char **matrix, char c);
char **montaCodigosCompleta(NoArvore *arv, char *nomeArquivo);
char *montaStringCompactada(char **codigo, char *nomeArquivo);
void writeBinFile(char *strCompact);
void descompacta(NoArvore *arv, char *nomeArquivoB, long tamA);
void liberaMemoriaCodigos(char **codigos, NoArvore *arv);
void liberaLCC(LCC *firstByte);
void returnTreeFile(ListaArvores *lista);
void writeTreeFile(NoArvore *arv, FILE *f);
void descompactaDef(char* nomeArquivoB);
void liberaListaArvores(ListaArvores* lista);
void liberaArvores (NoArvore* arv);
long tamAuxS = 0;

int main(void) {
  CaracLista *lista = NULL;
  ListaArvores *listaOrdenada = NULL;
  ListaArvores* listaArqHuffmanTree = NULL;
  /* Leitura do arquivo texto  */
  lista = leArqTexto("teste.txt", lista);
  printf("\n------------LISTA DESORDENADA COM FREQUÊNCIAS E "
         "CARACTERES------------\n");
  testeImprime(lista);
  /* Ordenação da lista */
  listaOrdenada = ordenaLista(lista);
  printf("\n------------LISTA ORDENADA COM FREQUÊNCIAS E "
         "CARACTERES------------\n");
  imprimirLista(listaOrdenada);
  returnTreeFile(listaOrdenada);
  NoArvore *arv = otimizaArvore(listaOrdenada);
  printf("\n------------PERCURSO PRÉ-ORDEM DA ÁRVORE DE HUFFMAN------------\n");
  exibeArvore(arv);
  printf("\n------------CARACTERES E SEUS CÓDIGOS------------\n");
  char **codigos = montaCodigosCompleta(arv, "teste.txt");
  char *strCompact = montaStringCompactada(codigos, "teste.txt");
  writeBinFile(strCompact);
  printf("\n------------LEITURA DO ARQUIVO COM AS INFORMAÇÕES DA ÁRVORE DE HUFFMAN------------\n");
  listaArqHuffmanTree = readHuffmanTreeFile();
  imprimirLista(listaArqHuffmanTree);
  printf("\n------------TEXTO LIDO DO ARQUIVO BIN E GERADO PELA "
         "DESCOMPACTAÇÃO------------\n");
  descompactaDef("teste.bin");
  liberaMemoriaCodigos(codigos, arv);
  free(strCompact);
  liberaListaArvores(listaArqHuffmanTree);
  liberaListaArvores(listaOrdenada);
  return 0;
}

// Função para ler o arquivo texto e chama função de inserir os caracteres na
// lista dos caracteres
CaracLista *leArqTexto(char *nomeArquivo, CaracLista *lista) {
  char c;
  FILE *f = fopen(nomeArquivo, "r");
  if (f == NULL) {
    printf("Erro ao abrir arquivo");
    return NULL;
  }
  while ((c = fgetc(f)) != EOF) {
    lista = insereListaTemp(lista, c);
  }
  // Adicionamos um caractere * para marcar o final do arquivo na hora da
  // descompactação
  lista = insereListaTemp(lista, '*');
  return lista;
}

// Função para inserir os caracteres na lista
CaracLista *insereListaTemp(CaracLista *lista, char c) {
  // Verifica na lista existente se esse caractere já está presente lá
  CaracLista *auxiliar = buscaListaTemp(lista, c);
  // Caso de percorrer a lista inteira e não encontrar o caractere, então
  // adiciona o novo caractere no início da lista
  if (auxiliar == NULL) {
    CaracLista *novoElem = (CaracLista *)malloc(sizeof(CaracLista));
    if (novoElem == NULL) {
      printf("Erro de malloc");
      return NULL;
    }
    novoElem->c = c;
    novoElem->prox = lista;
    novoElem->freq = 1;
    return novoElem;
  }
  // Caso de encontrar o caractere na lista, irá somar a frequência do nó já
  // existente
  else {
    auxiliar->freq++;
    return lista;
  }
}

// Função para buscar um caractere na lista de caracteres
CaracLista *buscaListaTemp(CaracLista *lista, char c) {
  CaracLista *auxiliar;
  for (auxiliar = lista; auxiliar != NULL; auxiliar = auxiliar->prox) {
    // Caso de encontrar o caractere, retorna um ponteiro para o nó que contém o
    // caractere procurado
    if (auxiliar->c == c) {
      return auxiliar;
    }
  }
  // Caso percorra a lista inteira, retorna NULL, representando que não
  // encontrou o elemento
  return NULL;
}

// Função chamada que percorre a lista original e chama a função ordena essa
// lista
ListaArvores *ordenaLista(CaracLista *lista) {
  CaracLista *auxiliarInsercao;
  CaracLista *auxiliarListaArvores;
  CaracLista *listaOrdenada = NULL;
  // Percorre a lista original e cria uma nova lista ordenada de forma crescente
  // com base na frequência
  for (auxiliarInsercao = lista; auxiliarInsercao != NULL;
       auxiliarInsercao = auxiliarInsercao->prox) {
    listaOrdenada = insereListaOrdenada(listaOrdenada, auxiliarInsercao->c,
                                        auxiliarInsercao->freq);
  }
  // Cria uma lista de árvores, em que cada nó é uma árvore binária
  ListaArvores *listaArvores = (ListaArvores *)malloc(sizeof(ListaArvores));
  if (listaArvores == NULL) {
    printf("Erro de malloc");
    return NULL;
  }
  listaArvores->primeiro = NULL;
  listaArvores->tamanho = 0;
  for (auxiliarListaArvores = listaOrdenada; auxiliarListaArvores != NULL;
       auxiliarListaArvores = auxiliarListaArvores->prox) {
    // Chama função criaArvore, inicializando os ponteiros esq, dir e prox como
    // NULL
    NoArvore *arvore = criaArvore(auxiliarListaArvores->freq,
                                  auxiliarListaArvores->c, NULL, NULL, NULL);
    // Essa árvore é passada como parâmetro para a função que insereas árvores
    // ordenadas nas listas
    listaArvores = insereListaArvoresOrdenada(listaArvores, arvore);
  }
  // Libera a memória das listas que não serão mais utilizadas
  liberaLista(lista);
  return listaArvores;
}

// Função que retorna uma lista ordenada, com base na lista original
CaracLista *insereListaOrdenada(CaracLista *lst, char c, int freq) {
  CaracLista *novo;
  CaracLista *ant = NULL;
  CaracLista *p = lst;
  while (p != NULL && p->freq < freq) {
    ant = p;
    p = p->prox;
  }
  novo = (CaracLista *)malloc(sizeof(CaracLista));
  if (novo == NULL) {
    printf("Erro de malloc");
    return NULL;
  }
  novo->freq = freq;
  novo->c = c;
  // Caso de inserir no início da lista
  if (ant == NULL) {
    novo->prox = lst;
    lst = novo;
  }
  // Caso de inserir no meio da lista, procura posição correta para inserir
  else {
    novo->prox = ant->prox;
    ant->prox = novo;
  }
  return lst;
}

// Função que cria as árvores com um nó para inserir na lista de árvores
NoArvore *criaArvore(int freq, char c, NoArvore *prox, NoArvore *esq,
                     NoArvore *dir) {
  NoArvore *novaArvore = (NoArvore *)malloc(sizeof(NoArvore));
  if (novaArvore == NULL) {
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
ListaArvores *insereListaArvoresOrdenada(ListaArvores *lista,
                                         NoArvore *arvore) {
  NoArvore *auxiliar;
  if (lista->primeiro == NULL) {
    lista->primeiro = arvore;
  } else if (arvore->freq < lista->primeiro->freq) {
    arvore->prox = lista->primeiro;
    lista->primeiro = arvore;
  } else {
    auxiliar = lista->primeiro;
    while (auxiliar->prox != NULL && auxiliar->prox->freq <= arvore->freq) {
      auxiliar = auxiliar->prox;
    }
    if (auxiliar->prox != NULL) {
      arvore->prox = auxiliar->prox;
    }
    auxiliar->prox = arvore;
  }
  lista->tamanho++;
  return lista;
}

// Função para retirar o menor nó de uma lista de árvores
NoArvore *retiraMenorNo(ListaArvores *lista) {
  NoArvore *auxiliar;
  if (lista->primeiro != NULL) {
    auxiliar = lista->primeiro;
    lista->primeiro = auxiliar->prox;
    auxiliar->prox = NULL;
    lista->tamanho--;
  }
  return auxiliar;
}

// Função para otimizar a árvore (juntar as árvores, até que sobre apenas uma)
NoArvore *otimizaArvore(ListaArvores *lista) {
  while (lista->tamanho > 1) {
    NoArvore *menorElem1 = retiraMenorNo(lista);
    NoArvore *menorElem2 = retiraMenorNo(lista);
    NoArvore *novo = (NoArvore *)malloc(sizeof(NoArvore));
    if (novo == NULL) {
      printf("Erro de malloc\n");
      return NULL;
    } else {
      // If para definir qual menorElem ficará na esquerda --> a menor
      // frequência fica na esquerda
      if (menorElem1->freq < menorElem2->freq) {
        novo = criaArvore(menorElem1->freq + menorElem2->freq, '~', NULL,
                          menorElem1, menorElem2);
      } else {
        novo = criaArvore(menorElem1->freq + menorElem2->freq, '~', NULL,
                          menorElem2, menorElem1);
      }
      lista = insereListaArvoresOrdenada(lista, novo);
    }
  }
  return lista->primeiro;
}

// Função que monta os códigos de um char e coloca ele numa matriz onde o indice
// da linha equivale ao char e o conteudo da linha é o código
int montaCodigos(NoArvore *arv, int pos, char **matrix, char c) {
  if (arv->c == c) {
    *(*(matrix + (int)c) + pos) = '\0';
    return 1;
  } else if (!(arv->dir) && !(arv->esq))
    return 0;
  if (montaCodigos(arv->esq, pos + 1, matrix, c)) {
    *(*(matrix + (int)c) + pos) = '0';
    return 1;
  } else if (montaCodigos(arv->dir, pos + 1, matrix, c)) {
    *(*(matrix + (int)c) + pos) = '1';
    return 1;
  }
  return 0;
}

// função auxiliar para saber a altura da arvore
int alturaArvore(NoArvore *raiz) {
  if (!raiz)
    return 0;
  int altEsq = 1 + alturaArvore(raiz->esq);
  int altDir = 1 + alturaArvore(raiz->dir);
  if (altDir > altEsq)
    return altDir;
  return altEsq;
}

// função para montar o código de todos os chars que aparecem no texto chamando
// a montaCodigos
char **montaCodigosCompleta(NoArvore *arv, char *nomeArquivo) {
  int altura = alturaArvore(arv);

  CaracLista *listaAux = NULL;
  listaAux = leArqTexto(nomeArquivo, listaAux);
  int tam = 0;
  CaracLista *current = listaAux;
  while (current) {
    tam++;
    current = current->prox;
  }
  current = listaAux;
  char letras[tam + 1];
  int pos = 0;
  while (current) {
    *(letras + pos) = current->c;
    pos++;
    current = current->prox;
  }

  char **codigos = (char **)malloc(255 * sizeof(char *));
  if (!codigos) {
    printf("ERROR: Falta de memória\n");
    exit(1);
  }
  for (int i = 0; i < 255; i++) {
    *(codigos + i) = (char *)malloc((altura + 1) * sizeof(char));
    if (!(*(codigos + i))) {
      printf("ERROR: Falta de memória\n");
      exit(1);
    }
  }
  for (int i = 0; i < tam; i++) {
    montaCodigos(arv, 0, codigos, letras[i]);
    printf("%c -> ", letras[i]);
    char *bit = *(codigos + (int)letras[i]);
    while (*bit != '\0') {
      printf("%c", *bit);
      bit++;
    }
    printf("\n");
  }
  liberaLista(listaAux);
  return codigos;
}

// função que monta a string equivalente ao texto compactado seguindo os códigos
// da arvore
char *montaStringCompactada(char **codigo, char *nomeArquivo) {
  Stri* strCP = (Stri*)malloc(sizeof(Stri));
  strCP->c = 0;
  strCP->prox = NULL;
  Stri* current = strCP;
  Stri* ant = NULL;
  FILE *f = fopen(nomeArquivo, "r");
  int qtdBits=0;
  char c;
  int tam = 1;
  while ((c = fgetc(f)) != EOF){
    char* cod = *(codigo + c);
    int pos = 0;
    while (*(cod+pos) != '\0'){
      char bit = *(cod+pos) - '0';
      if (bit)
        current->c |= (0x80 >> qtdBits);
      pos++;
      qtdBits++;
      if (qtdBits == 8){
        qtdBits = 0;
        Stri* next = (Stri*)malloc(sizeof(Stri));
        next->c = 0;
        next->prox = 0;
        current->prox = next;
        ant = current;
        current = next;
        tam++;
      }
    }
  }
  if (qtdBits == 0 && ant){
    ant->prox = NULL;
    current = ant;
    tam--;
  }else{
    char* padding = *(codigo+'*');
    int posPad = 0;
    for (int i = 0; i < 8-qtdBits; i++){
      if (*(padding + posPad) == '\0')
        posPad = 0;
      char bit = *(padding+posPad) - '0';
      posPad++;
      if (bit)
        current->c |= (0x80>>(qtdBits+i));
    }
  }
  char* stringCompact = (char*)malloc(sizeof(char)*tam);
  current = strCP;
  for (int i = 0; i < tam; i++){
    *(stringCompact+i) = current->c;
    current = current->prox;
  }
  tamAuxS = tam;
  return  stringCompact;
}

// Escrevendo arquivo binário
void writeBinFile(char *strCompact) {
  FILE *arquivo = fopen("teste.bin", "wb");
  if (arquivo == NULL) {
    printf("ERRO: Não foi possível abrir o arquivo de escrita.\n");
    exit(1);
  }
  int pos = 0;
  while (pos < tamAuxS) {
    fputc(*(strCompact + pos), arquivo);
    pos++;
  }
  fclose(arquivo);
}

// descompactando arquivo binário
void descompacta(NoArvore *arv, char *nomeArquivoB, long tamA) {
  FILE *arquivoB = fopen("teste.bin", "rb");
  if (!arquivoB) {
    printf("ERROR: Não foi possível abrir arquivo compactado\n");
    exit(1);
  }
  LCC *strDescomp = (LCC *)malloc(sizeof(LCC));
  strDescomp->byte = 0;
  strDescomp->prox = NULL;
  char c;
  int aux2 = 0;

  LCC *current = strDescomp;
  NoArvore *currentNode = arv;
  int stop = 0;
  while (aux2 < tamA) {
    aux2++;
    c = fgetc(arquivoB);
    int bitPos = 0;
    while (bitPos < 8) {
      char aux = c;
      aux = (aux << bitPos) & (0x80);
      bitPos++;
      if (!aux) {
        currentNode = currentNode->esq;
      } else {
        currentNode = currentNode->dir;
      }
      if (!(currentNode->dir) && !(currentNode->esq)) {
        if (currentNode->c == '*') {
          stop = 1;
          break;
        }
        current->byte = currentNode->c;
        current->prox = malloc(sizeof(LCC));
        current->prox->byte = 0;
        current->prox->prox = NULL;
        current = current->prox;
        currentNode = arv;
      }
    }
    if (stop)
      break;
  }
  printf("\n");
  current = strDescomp;
  int tam = 0;
  while (current) {
    tam++;
    current = current->prox;
  }
  current = strDescomp;
  char strLida[tam + 1];
  for (int pos = 0; pos < tam; pos++) {
    *(strLida + pos) = current->byte;
    current = current->prox;
  }
  *(strLida + tam) = '\0';

  FILE *arquivo = fopen("testeDescompactado.txt", "w");
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
void liberaMemoriaCodigos(char **codigos, NoArvore *arv) {
  for (int i = 0; i < 255; i++) {
    free(*(codigos + i));
  }
  free(codigos);
}

void descompactaDef(char* nomeArquivoB){
  ListaArvores* lista = readHuffmanTreeFile();
  NoArvore* arv = otimizaArvore(lista);
  FILE *arquivo;
  long tamanho;

  arquivo = fopen(nomeArquivoB, "r");
  if (arquivo == NULL) {
      printf("Erro ao abrir o arquivo.\n");
      exit(1);
  }

  fseek(arquivo, 0, SEEK_END);  
  tamanho = ftell(arquivo);   
  fclose(arquivo);
  descompacta(arv, nomeArquivoB, tamanho);
  
}

// Função para exibir a árvore e os ponteiros corretamente, no percurso de
// pré-ordem
void exibeArvore(NoArvore *ab) {
  // Caso base -> identificar quando chegar ao final da árvore
  if (ab == NULL) {
    printf("A árvore está vazia\n");
    return;
  };
  printf("Chave: %d - Caractere: %c\n", ab->freq, ab->c);
  // Chamada recursiva para imprimir as chaves da esquerda e depois da direita,
  // como manda o percurso pré-ordem
  if (ab->esq != NULL)
    exibeArvore(ab->esq);
  if (ab->dir != NULL)
    exibeArvore(ab->dir);
};

void liberaListaArvores(ListaArvores* lista){
  liberaArvores(lista->primeiro);
  free(lista);
}

void liberaArvores (NoArvore* arv){
  if (arv == NULL){
    return;
  }
  liberaArvores(arv->esq);
  liberaArvores(arv->dir);
  free(arv);
}

// Função para imprimir a lista de árvores com apenas a raiz
void imprimirLista(ListaArvores *lista) {
  NoArvore *aux = lista->primeiro;
  printf("Tamanho: %d\n", lista->tamanho);
  while (aux != NULL) {
    printf("%c - %d\n", aux->c, aux->freq);
    aux = aux->prox;
  }
}

// Função para liberar o espaço da memória da lista
void liberaLista(CaracLista *lista) {
  CaracLista *p = lista;
  while (p != NULL) {
    CaracLista *temp = p->prox;
    free(p);
    p = temp;
  }
}

// Função para imprimir a lista desordenada temporária
void testeImprime(CaracLista *lista) {
  CaracLista *auxiliar;
  for (auxiliar = lista; auxiliar != NULL; auxiliar = auxiliar->prox) {
    printf("%c - %d\n", auxiliar->c, auxiliar->freq);
  }
}

// Função para liberar a memória da Lista de Char
void liberaLCC(LCC *firstByte) {
  if (!firstByte)
    return;
  liberaLCC(firstByte->prox);
  free(firstByte);
}

// Função para montar o arquivo com as informações da árvore de Huffman
void returnTreeFile(ListaArvores *lista) {
  FILE *f = fopen("huffman_tree.txt", "w");
  if (f == NULL) {
    printf("Erro ao abrir o arquivo\n");
    return;
  }
  writeTreeFile(lista->primeiro, f);
  fclose(f);
}

// Função para escrever os caracteres e as frequências no arquivo "huffman_tree.txt"
void writeTreeFile(NoArvore *arv, FILE *f) {
  if (arv == NULL) {
    return;
  }
  // Estávamos tendo dificuldades de ler o \n e ' ' do huffman_tree.txt, então salvamos como 1 e 2, respectivamente
  if (arv->c != '~') {
    if (arv->c == '\n'){
      fprintf(f, "1%d\n", arv->freq);
    }
    else if (arv->c == ' '){
      fprintf(f, "2%d\n", arv->freq);
    }
    else{
      fprintf(f, "%c%d\n", arv->c, arv->freq);
    }
  }
  writeTreeFile(arv->prox, f);
}

ListaArvores* readHuffmanTreeFile (){
  char c;
  int freq;
  ListaArvores* lista = (ListaArvores*)malloc(sizeof(ListaArvores));
  if (lista == NULL){
    printf("Erro de malloc\n");
    return NULL;
  }
  lista->primeiro = NULL;
  lista->tamanho = 0;
  FILE* f = fopen("huffman_tree.txt", "r");
  if (f == NULL){
    printf("Erro ao abrir o arquivo\n");
    return NULL;
  }
  // Como falado anteriormente, 1 representa '\n' e 2 represente ' '
  while (fscanf(f, "%c%d\n", &c, &freq) != EOF){
    if (c == '1'){
      //printf("%c - %d\n", c, freq);
      NoArvore* arvore = criaArvore(freq, '\n', NULL, NULL, NULL);
      lista = insereListaArvoresOrdenada(lista, arvore);
    }
    else if (c == '2'){
      NoArvore* arvore = criaArvore(freq, ' ', NULL, NULL, NULL);
      lista = insereListaArvoresOrdenada(lista, arvore);
    }
    else{
      NoArvore* arvore = criaArvore(freq, c, NULL, NULL, NULL);
      lista = insereListaArvoresOrdenada(lista, arvore);
    }
  }
  return lista;
}



