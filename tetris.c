#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

typedef struct Peca {
    char nome[2];
    int id;
} Peca;

typedef struct Pilha {
    Peca pilha[MAX_PILHA];
    int topo;
} Pilha;

typedef struct Fila {
    Peca fila[MAX_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

void inicializarPilha(Pilha* p);
int pilhaCheia(Pilha* p);
int pilhaVazia(Pilha* p);
void empilhar(Pilha* p, Peca peca);
Peca desempilhar(Pilha* p);
void exibirPilha(Pilha* p);

void inicializarFila(Fila* f);
int filaCheia(Fila* f);
int filaVazia(Fila* f);
void enfileirar(Fila* f, Peca peca);
Peca desenfileirar(Fila* f);
void exibirFila(Fila* f);

Peca gerarPeca(int id);


int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    int idAtual = 0;

    inicializarFila(&fila);
    inicializarPilha(&pilha);


    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca(idAtual++));
    }

    while (1) {
        printf("\nFila: ");
        exibirFila(&fila);
        printf("Pilha: ");
        exibirPilha(&pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça da fila\n");
        printf("2 - Usar peça da pilha\n");
        printf("3 - Enviar peça da fila para pilha\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Substituir 3 primeiros da fila com a pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        int opcao;
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca p = desenfileirar(&fila);
            printf("Peça jogada: [%s %d]\n", p.nome, p.id);
            enfileirar(&fila, gerarPeca(idAtual++));

        } else if (opcao == 2) {
            Peca p = desempilhar(&pilha);
            if (p.id != -1)
                printf("Peça usada da pilha: [%s %d]\n", p.nome, p.id);

        } else if (opcao == 3) {
            if (!pilhaCheia(&pilha)) {
                Peca p = desenfileirar(&fila);
                empilhar(&pilha, p);
                enfileirar(&fila, gerarPeca(idAtual++));
            } else {
                printf("Pilha cheia!\n");
            }

        } else if (opcao == 4) {
            
            if (!filaVazia(&fila) && !pilhaVazia(&pilha)) {
                int primeiroFilaIndex = fila.inicio;
                Peca topoPilha = pilha.pilha[pilha.topo];
                Peca primeiroFila = fila.fila[primeiroFilaIndex];

                // Inverte
                fila.fila[primeiroFilaIndex] = topoPilha;
                pilha.pilha[pilha.topo] = primeiroFila;

                printf("Troca realizada: frente da fila <-> topo da pilha\n");
            }

        } else if (opcao == 5) {
            if (fila.tamanho >= 3 && pilha.topo >= 2) {

                Peca filaTemp[3];
                Peca pilhaTemp[3];
                Peca restoFila[MAX_FILA];
                int restoTam = 0;

                // Remove os 3 primeiros da fila
                for (int i = 0; i < 3; i++) {
                    filaTemp[i] = desenfileirar(&fila);
                }

                // Remove os 3 da pilha
                pilhaTemp[0] = desempilhar(&pilha);
                pilhaTemp[1] = desempilhar(&pilha);
                pilhaTemp[2] = desempilhar(&pilha);

                // Guarda o restante da fila
                while (!filaVazia(&fila)) {
                    restoFila[restoTam++] = desenfileirar(&fila);
                }

                // Reconstroi a fila com as peças da pilha na frente
                enfileirar(&fila, pilhaTemp[0]);
                enfileirar(&fila, pilhaTemp[1]);
                enfileirar(&fila, pilhaTemp[2]);
                for (int i = 0; i < restoTam; i++) {
                    enfileirar(&fila, restoFila[i]);
                }

                // Reconstroi a pilha
                empilhar(&pilha, filaTemp[0]);
                empilhar(&pilha, filaTemp[1]);
                empilhar(&pilha, filaTemp[2]);

                printf("Troca dos 3 primeiros da fila com pilha realizada.\n");
            } else {
                printf("Peças insuficientes para troca.\n");
            }

        } else if (opcao == 0) {
            printf("Encerrando...\n");
            break;

        } else {
            printf("Opção inválida!\n");
        }
    }

    return 0;
}

// Pilha
void inicializarPilha(Pilha* p) { p->topo = -1; }
int pilhaCheia(Pilha* p) { return p->topo == MAX_PILHA - 1; }
int pilhaVazia(Pilha* p) { return p->topo == -1; }

void empilhar(Pilha* p, Peca peca) {
    if (!pilhaCheia(p)) p->pilha[++p->topo] = peca;
}

Peca desempilhar(Pilha* p) {
    if (pilhaVazia(p)) {
        Peca vazia = {"", -1};
        return vazia;
    }
    return p->pilha[p->topo--];
}

void exibirPilha(Pilha* p) {
    if (pilhaVazia(p)) printf("Pilha vazia!\n");
    else {
        for (int i = p->topo; i >= 0; i--)
            printf("[%s %d] ", p->pilha[i].nome, p->pilha[i].id);
        printf("\n");
    }
}

// Fila
void inicializarFila(Fila* f) { f->inicio = f->fim = f->tamanho = 0; }
int filaCheia(Fila* f) { return f->tamanho == MAX_FILA; }
int filaVazia(Fila* f) { return f->tamanho == 0; }

void enfileirar(Fila* f, Peca peca) {
    if (!filaCheia(f)) {
        f->fila[f->fim] = peca;
        f->fim = (f->fim + 1) % MAX_FILA;
        f->tamanho++;
    }
}

Peca desenfileirar(Fila* f) {
    if (filaVazia(f)) {
        Peca vazia = {"", -1};
        return vazia;
    }
    Peca p = f->fila[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->tamanho--;
    return p;
}

void exibirFila(Fila* f) {
    if (filaVazia(f)) printf("Fila vazia!\n");
    else {
        int i = f->inicio;
        for (int j = 0; j < f->tamanho; j++) {
            printf("[%s %d] ", f->fila[i].nome, f->fila[i].id);
            i = (i + 1) % MAX_FILA;
        }
        printf("\n");
    }
}


Peca gerarPeca(int id) {
    Peca p;
    const char* tipos[] = {"I", "O", "T", "L"};
    strcpy(p.nome, tipos[rand() % 4]);
    p.id = id;
    return p;
}