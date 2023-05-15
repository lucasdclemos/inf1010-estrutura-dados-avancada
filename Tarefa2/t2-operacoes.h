//Nome: Jayme Augusto Avelino de Paiva; Matrícula: 2210289
//Nome: Lucas Demarco Cambraia Lemos; Matrícula: 2110013

typedef struct node Node;
struct node {
  int chave;
  Node *esq;
  Node *dir;
};

// Função para exibir a árvore e os ponteiros corretamente, no percurso de pré-ordem
void exibeArvore(Node* ab);

// Função para liberar o espaço alocado por cada nó
void libera(Node* ab);

// Função recursiva que verifica a altura da árvore
int verificaAltura (Node* raiz);

// Função recursiva que verifica se a árvore é ABB ou não
int verificaABB (Node* raiz);

// Função para inverter as sub-árvores da esquerda e direita
Node* inverteArvore(Node* ab);

