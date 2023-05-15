//Aluno1: Jayme Augusto Avelino de Paiva; matrícula: 2210289
//Aluno2: Lucas Demarco Cambraia Lemos; matrícula: 2110013

//Código das cores: vermelho=1; amarelo=2; verde=3

#include <stdio.h>
#include <stdlib.h>

char* prioridadesCodes[] = {"vermelho","amarelo","verde"};

static int pos = 0;

//criação da lista estruturada com o ponteiro pro proximo item e as infos da cor(prioridade) e da posição(ordem de chegada)
struct linkedList{
    struct linkedList* prox;
    int cor;
    int pos;
};

typedef struct linkedList List;

//função para iniciar a lista encadeada com NULL
List* mkList(void){
    List* lst = NULL;
    return lst;
};

//função recursiva para inserir um elemento novo na lista encadeada de forma a manter a ordem da lista, recebe a lista e as infos do elemento novo como parâmetros
List* insereElem(List* lst, int cor, int pos){
    //Tratando o caso em que a lista está vazia
    if(lst == NULL || (lst->prox == NULL && lst->cor > cor)){
        List* firstElem = (List*)malloc(sizeof(List));
        if(firstElem == NULL){
            printf("ERROR: Falta de memória");
            exit(1);
        };
        firstElem->cor = cor;
        firstElem->pos = pos;
        firstElem->prox = lst;
        return firstElem;
    };
    /*inserindo o item novo da lista nessa posição caso tenha chegado ao fim da lista
      ou se a cor(prioridade) do item seguinte for maior que a do atual ou se o atual tiver a mesma prioridade, mas uma posição(ordem de chegada) maior*/
    if(lst->prox == NULL || lst->prox->cor > cor || (lst->prox->cor == cor && lst->prox->pos > pos)){
        List* newElem = (List*)malloc(sizeof(List));
        if(newElem == NULL){
            printf("ERROR: Falta de memória");
            exit(1);
        };
        newElem->cor = cor;
        newElem->pos = pos;
        newElem->prox = lst->prox;
        lst->prox = newElem;
    }else{
        //chamada recursiva para percorrer a lista até achar a posição devida para a inserção
        insereElem(lst->prox,cor,pos);
    };

    return lst;
};

//função recursiva para printar lista, recebe a lista e um array para contar a quantidade de pacientes de cada cor(prioridade)
void printList(List* lst, int* p){
    //caso base, caso a lista tenha chegado no fim é printado a quantidade de pacientes de cada cor(prioridade)
    if (lst == NULL){
        printf("\n%d pacientes vermelho, %d amarelos e %d verdes\n\n",*p,*(p+1),*(p+2));
        return;
    };
    //atualizando os valores do array p adicionando a contagem do elemento atual nele
    *(p+(lst->cor)-1) += 1;
    //printando as infos do paciente na posição atual da lista
    printf("Paciente de prioridade %s, posição %d\n", prioridadesCodes[lst->cor-1], lst->pos);
    //chamada recursiva para percorrer a lista
    return printList(lst->prox,p);
};

//função para printar a lista que já declara o array para nela para não ter que declarar ele zerado toda vez que for printar
void printListWQtd(List* lst){
    int p[] = {0,0,0};
    printList(lst,p);
    return;
};

//função recursiva para deletar um item da lista, recebe a lista e as infos do elemento que deve ser deletado
List* popLL(List* lst, int cor, int pos){
    /*tratando caso no qual o primeiro elemento da lista é o elemento a ser deletado, esse caso não vai ser chamado durante as recursões porque a recursão já teria deletado o item
      antes de chegar nesse caso novamente*/
    if(lst->cor == cor && lst->pos == pos){
        printf("Deletando paciente de prioridade %s e de posicao de chegada %d\n",prioridadesCodes[cor-1],pos);
        List* newLst = lst->prox;
        free(lst);
        return newLst;
    };
    /*se o próximo item for NULL quer dizer que a lista toda foi percorrida e o elemento a ser deletado não foi encontrado,
      o elemento anterior ao NULL não pode ser o elemento a ser deletado, pois o caso(if) anterior já teria visto caso esse 
      elemento fosse o primeiro e o caso(if) seguinte veria durante as chamadas recursivas*/
    if(lst->prox == NULL){
        printf("O elemento não está presente na lista\n");
        return;
    //tratando o caso em que o elemento seguinte ao atual é o elemento a ser deletado
    }else if(lst->prox->cor==cor && lst->prox->pos==pos){
        printf("Deletando paciente de prioridade %s e de posicao de chegada %d\n",prioridadesCodes[cor-1],pos);
        List* lstAux = lst->prox->prox;
        free(lst->prox);
        lst->prox = lstAux;
        return;
    //chamada recursiva da função para percorrer a lista caso o elemento ainda não tenha sido encontrado
    }else{
        popLL(lst->prox,cor,pos);
    };
    //retornando a lista depois da exclusão do elemento (caso o elemento tenha sido o primeiro elemento da lista o retorna já teria sido feito no primeiro if
    return lst;
};


List* insereElemF(List* lst, int prioridade){
    pos++;
    return insereElem(lst,prioridade,pos);
};

int main(void){ 
    List* lst = mkList();
    lst = insereElemF(lst,3);
    lst = insereElemF(lst,1);
    lst = insereElemF(lst,3);
    lst = insereElemF(lst,2);
    lst = insereElemF(lst,1);
    lst = insereElemF(lst,1);
    lst = insereElemF(lst,3);
    lst = insereElemF(lst,1);
    printListWQtd(lst);

    lst = popLL(lst,1,2);
    lst = popLL(lst,2,4);
    printListWQtd(lst);

    lst = insereElemF(lst,3);
    lst = insereElemF(lst,2);
    lst = insereElemF(lst,1);
    lst = insereElemF(lst,2);
    printListWQtd(lst);

    lst = popLL(lst,1,5);
    lst = popLL(lst,1,6);
    lst = popLL(lst,3,3);
    printListWQtd(lst);

    return 1;
};  

