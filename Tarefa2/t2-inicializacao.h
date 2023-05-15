//Nome: Jayme Augusto Avelino de Paiva; Matrícula: 2210289
//Nome: Lucas Demarco Cambraia Lemos; Matrícula: 2110013

typedef struct node Node;

// Função para gerar as chaves que serão inseridas nas árvores
void geraChaves (int* valores);

// Função para verificar se os número aleatórios estão repetidos
int verificaChaves(int* valores, int numero);

// Função que retorna NULL para criar a raiz da árvore
Node* inicializa(void);

// Função para criar os nós (na inserção por nível)
Node* mkNode(int chave);

// Função para inserção em nível
Node* insereAbNivel(Node* ab, int chave, int altura, int level);

// Função recursiva para inserir os elementos na Árvore Binária de Busca
Node *insereABB(Node *raiz, int chave);

