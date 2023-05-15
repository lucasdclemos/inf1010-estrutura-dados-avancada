//Nome: Jayme Augusto Avelino de Paiva; matrícula: 2210289
//Nome: Lucas Demarco Cambraia Lemos; matrícula: 2110013

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct hashTable HashTable;

struct hashTable{
    int col;
    int ocu;
    char** hash;
};

void readPlacas(char** linhas){
    FILE* file = fopen("placas.txt","r");
    char line[50];
    int i = 0;

    while (fgets(line,50,file)){
        strcpy(*(linhas+i),line);
        *(*(linhas+i)+7) = '\0';
        i++;
    }
}

int mkKey(char* placa, int k){
    unsigned long king = 0;
    unsigned long val2 = 0;
    unsigned char* p2 = (unsigned char*)(&val2);
    unsigned long val = 0;
    unsigned char* p = (unsigned char*)(&val);
    for (int i = 0; i<7; i++){
        unsigned char uc = *(placa+i);
        *(p+i) = uc;
        *(p2+7-i) = uc;
        king += uc;
    }
    unsigned long newVal = val*val2;
    int key = (int)(newVal + (val2)*k + (val)*k*k + k)%(1031);
    key = (key)%1031;
    if (king%89 == 1)
        key = key*(-1);
    key = key < 0 ? key + 1031 : key;
    return key;
}


char** insertHash(HashTable* hashTable, char* key){
    int inseriu = 0;
    int k = 0;
    if (hashTable->ocu == 1031){
        printf("Error: Falta de espaço\n");
        return hashTable->hash;
    }
    while(!inseriu && k < 1031){
        int pos = mkKey(key, k);
        if (*(hashTable->hash + pos) == NULL || !strcmp(*(hashTable->hash + pos),"#######")){
            *(hashTable->hash+pos) = (char*)(malloc(sizeof(char)*50));
            strcpy(*(hashTable->hash + pos), key);
            inseriu = 1;
            hashTable->ocu++;
        }else{
            hashTable->col++;
        }
        k++;
    }
    return hashTable->hash;
}


char* buscaHash(HashTable* hashTable, char* value){
    int key;
    int tentativa = 0;
    while (tentativa < 1031){
        key = mkKey(value, tentativa);
        if (hashTable->hash[key] == NULL){
            printf("O elemento não está na lista\n");
            return NULL;
        }

        if (!strcmp(hashTable->hash[key],value))
            return hashTable->hash[key];
        tentativa++;
    }
    printf("O elemento não está na lista\n");
    return NULL;
}

char** deleteHash(HashTable* hashTable, char* value){
    int key;
    int tentativa = 0;
    while (tentativa < 1031){
        key = mkKey(value, tentativa);
        if (hashTable->hash[key] == NULL){
            printf("O elemento não está na lista\n");
            return hashTable->hash;
        }
        if (!strcmp(hashTable->hash[key],value)){
            strcpy(hashTable->hash[key],"#######");
            hashTable->ocu--;
            return hashTable->hash;
        }
        tentativa++;
    }
    printf("O elemento não está na lista");
    return hashTable->hash;
}

void cleanList(HashTable* hashTable){
    for (int i=0; i<1031; i++){
        hashTable->hash[i] = NULL;
    }
    hashTable->col = 0;
    hashTable->ocu = 0;
}

void calcTimesInsertion(int k, HashTable* hashTable, char** placas){
    long int timeIni = clock();
    for (int i = 0; i < k; i++){
        insertHash(hashTable, *(placas+i));
    }
    long int timeFim = clock();
    double time = (double)(timeFim - timeIni)/CLOCKS_PER_SEC;
    //printf("%d\n", hashTable->col);
   
    timeIni = clock();
    for (int i = 0; i < k; i++){
        buscaHash(hashTable, *(placas+i));
    }
    timeFim = clock();
    double timeBus = (double)(timeFim - timeIni)/CLOCKS_PER_SEC;
    if (k%10 == 0)
        printf("%d inserções: %lfs\n%d colisões\n%d buscas: %lfs\n\n",k, time,hashTable->col,k,timeBus);
    cleanList(hashTable);
}

void calcAllTimesIns(HashTable* hashTable, char** placas){
    for (int k = 1; k <= 1000; k++){
        calcTimesInsertion(k, hashTable, placas);
    }
}

int main(void){
    char** linhas = (char**)malloc(sizeof(char*)*1000);
    if (linhas == NULL){
        printf("ERROR: Falta de memória\n");
        return 1;
    }
    for (int i = 0; i<1000; i++){
        *(linhas+i) = (char*)malloc(sizeof(char)*10);
        if (*(linhas+i) == NULL){
            printf("ERROR: Falta de memória\n");
            return 1;
        }
    }
    readPlacas(linhas);
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    if (hashTable == NULL){
        printf("ERROR: Falta de memória\n");
        return 1;
    }
    hashTable->hash = (char**)malloc(sizeof(char*)*1031);
    if (hashTable->hash == NULL){
        printf("ERROR: Falta de memória\n");
        return 1;
    }
    hashTable->col = 0;
    hashTable->ocu = 0;
    
    for (int i=0; i<1031; i++){
        *(hashTable->hash+i) = NULL;
    }

    calcAllTimesIns(hashTable, linhas);
    cleanList(hashTable);
    
    return 0;
}