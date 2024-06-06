#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_DA_FILA 200

typedef struct {
    char nome_tarefa[20];
    char data[20];
    char prazo[20];
} Tarefa;

typedef struct {
    Tarefa v[TAMANHO_DA_FILA];
    int tail;
    int head;
    int count;
} Fila;

typedef struct Celula {
    struct Celula* anterior;
    struct Celula* proximo;
    char nome[35];
    char departamento[20];
    char cargo[20];
    Fila* tarefas;
} Celula;

typedef struct {
    Celula* primeira_tarefa;
    int qtde_tarefas;
} LDDE;

LDDE* inicia_lista() {
    LDDE* lista = malloc(sizeof(LDDE));
    lista->primeira_tarefa = NULL;
    lista->qtde_tarefas = 0;
    return lista;
}

Fila* inicializa_fila() {
    Fila* fila = malloc(sizeof(Fila));
    fila->count = 0;
    fila->tail = 0;
    fila->head = 0;
    return fila;
}

Celula* inicializa_celula(char nome[], char departamento[], char cargo[], Fila* tarefas) {
    Celula* celula = malloc(sizeof(Celula));
    celula->anterior = NULL;
    celula->proximo = NULL;
    strcpy(celula->nome, nome);
    strcpy(celula->departamento, departamento);
    strcpy(celula->cargo, cargo);
    celula->tarefas = tarefas;
    return celula;
}

int is_full(Fila* fila) {
    return fila->count == TAMANHO_DA_FILA;
}

int is_empty(Fila* fila) {
    return fila->count == 0;
}

int inserir_tarefa(Fila* fila, Tarefa tarefa) {
    if (!is_full(fila)) {
        fila->v[fila->tail % TAMANHO_DA_FILA] = tarefa;
        fila->count++;
        fila->tail++;
        return 1;
    }
    return 0;
}

void inserir(LDDE* lista, char nome[], char departamento[], char cargo[], Fila* tarefas, int posicao) {
    Celula* novo = inicializa_celula(nome, departamento, cargo, tarefas);

    if (posicao < 0 || posicao > lista->qtde_tarefas) {
        printf("Essa posição não existe, tente novamente!\n\n");
        return;
    }

    if (posicao == 0) {
        if (lista->primeira_tarefa == NULL) {
            lista->primeira_tarefa = novo;
        } else {
            novo->proximo = lista->primeira_tarefa;
            lista->primeira_tarefa->anterior = novo;
            lista->primeira_tarefa = novo;
        }
    } else {
        Celula* atual = lista->primeira_tarefa;
        int cont = 0;
        while (cont < posicao - 1) {
            atual = atual->proximo;
            cont++;
        }

        novo->proximo = atual->proximo;
        novo->anterior = atual;

        if (atual->proximo != NULL) {
            atual->proximo->anterior = novo;
        }
        atual->proximo = novo;
    }
    lista->qtde_tarefas++;
}

Celula* buscar(LDDE* lista, char nome[]) {
    Celula* atual = lista->primeira_tarefa;

    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

Tarefa buscar_tarefa(Fila* fila, char tarefa[]) {
    int i = fila->head;
    while (i < fila->tail) {
        Tarefa* t = &(fila->v[i % TAMANHO_DA_FILA]);
        if (strcmp(t->nome_tarefa, tarefa) == 0) {
            return *t;
        }
        i++;
    }
    Tarefa tarefa_vazia = {"", "", ""};
    return tarefa_vazia;
}

void remover(LDDE* lista, Celula* celula) {
    if (celula == NULL) {
        return;
    }

    if (celula == lista->primeira_tarefa) {
        lista->primeira_tarefa = celula->proximo;

        if (celula->proximo != NULL) {
            celula->proximo->anterior = NULL;
        }
    } else {
        celula->anterior->proximo = celula->proximo;

        if (celula->proximo != NULL) {
            celula->proximo->anterior = celula->anterior;
        }
    }

    free(celula);
}

Tarefa remover_tarefa(Fila* fila) {
    if (!is_empty(fila)) {
        Tarefa tarefa = fila->v[fila->head % TAMANHO_DA_FILA];
        fila->count--;
        fila->head++;
        return tarefa;
    }
    Tarefa tarefa_vazia = {"", "", ""};
    return tarefa_vazia;
}

void imprimir(LDDE* lista) {
    printf("\nRegistros:\n");
    printf("--------------------\n");

    Celula* atual = lista->primeira_tarefa;

    while (atual != NULL) {
        printf("Nome: %s\n", atual->nome);
        printf("Departamento: %s\n", atual->departamento);
        printf("Cargo: %s\n", atual->cargo);
        printf("--------------------\n\n");

        atual = atual->proximo;
    }
}

void imprimir_tarefa(Fila* fila) {
    for (int i = fila->head; i < fila->tail; i++) {
        Tarefa tarefa = fila->v[i % TAMANHO_DA_FILA];
        printf("Nome: %s\n", tarefa.nome_tarefa);
        printf("Data inserida: %s\n", tarefa.data);
        printf("Prazo: %s\n", tarefa.prazo);
    }
    printf("\n");
}

int main() {
    LDDE* lista = inicia_lista();
    Fila* fila = inicializa_fila();

    int posicao = 0;
    int tecla2 = 0;
    int tecla = 0;
    int tecla3 = 0;

    while(tecla != 3) {
        printf("O que deseja fazer?\n");
        printf("1. Colaborador:\n");
        printf("2. Tarefa:\n");
        printf("3. Sair\n");
        scanf("%d", &tecla);

        switch(tecla) {
            case 1: 
                printf("Menu:\n");
                printf("1. Inserir colaborador:\n");
                printf("2. Remover colaborador:\n");
                printf("3. Buscar colaborador:\n");
                printf("4. Mostrar colaborador:\n");
                scanf("%d", &tecla2);

                switch(tecla2) {
                    case 1:
                        printf("\nAdicionar Registro na lista\n");
                        char nome[35];
                        char departamento[20];
                        char cargo[20];

                        printf("Digite o nome: ");
                        scanf("%34s", nome);
                        printf("Digite o departamento: ");
                        scanf("%19s", departamento);
                        printf("Digite o cargo: ");
                        scanf("%19s", cargo);

                        printf("\n\n");

                        printf("1. Adicionar registro no inicio da lista\n");
                        printf("2. Adicionar registro no final da lista\n");
                        printf("3. Adicionar registro em uma posição especifica da lista\n");
                        scanf("%d", &posicao);

                        switch (posicao) {
                            case 1:
                                inserir(lista, nome, departamento, cargo, fila, 0);
                                break;

                            case 2:
                                inserir(lista, nome, departamento, cargo, fila, lista->qtde_tarefas);
                                break;

                            case 3:
                                printf("Digite a posição desejada: ");
                                scanf("%d", &posicao);
                                inserir(lista, nome, departamento, cargo, fila, posicao);
                                break;
                        }
                        break;

                    case 2:
                        printf("\nRemover Registro\n");
                        char nome_remover[35];
                        printf("Digite o nome para remover da lista: ");
                        scanf("%34s", nome_remover);

                        Celula* resultado_remover = buscar(lista, nome_remover);
                        remover(lista, resultado_remover);
                        printf("Registro removido com sucesso.\n");
                        printf("\n");
                        break;

                    case 3:
                        printf("\nBuscar Registro\n");
                        char nome_buscar[35];
                        printf("Digite o nome para buscar na lista: ");
                        scanf("%34s", nome_buscar);

                        Celula* resultado_busca = buscar(lista, nome_buscar);
                        if (resultado_busca != NULL) {
                            printf("\nRegistro encontrado:\n");
                            printf("Nome: %s\n", resultado_busca->nome);
                            printf("Departamento: %s\n", resultado_busca->departamento);
                            printf("Cargo: %s\n", resultado_busca->cargo);
                        } else {
                            printf("\nRegistro não encontrado.\n");
                        }
                        printf("\n");
                        break;

                    case 4:
                        imprimir(lista);
                        break;

                    default:
                        printf("Número inválido");
                }
                break;

            case 2:
                printf("1. Inserir Tarefa:\n");
                printf("2. Remover Tarefa:\n");
                printf("3. Buscar Tarefa:\n");
                printf("4. Mostrar Tarefa:\n");
                scanf("%d", &tecla3);

                switch(tecla3) {
                    case 1:
                        {
                            Tarefa tarefa;
                            printf("Digite o nome da tarefa: ");
                            scanf("%19s", tarefa.nome_tarefa);
                            printf("Digite a data da tarefa: ");
                            scanf("%19s", tarefa.data);
                            printf("Digite o prazo da tarefa: ");
                            scanf("%19s", tarefa.prazo);
                            inserir_tarefa(fila, tarefa);
                            printf("Tarefa adicionada com sucesso!\n");
                            break;
                        }

                    case 2:
                        {
                            Tarefa tarefa_removida = remover_tarefa(fila);
                            if (strcmp(tarefa_removida.nome_tarefa, "") == 0) {
                                printf("A fila está vazia!\n");
                            } else {
                                printf("Tarefa removida: Nome: %s, Data: %s, Prazo: %s\n", 
                                    tarefa_removida.nome_tarefa, tarefa_removida.data, tarefa_removida.prazo);
                            }
                            break;
                        }

                    case 3:
                        {
                            char nome[20];
                            printf("Digite o nome da tarefa a ser buscada: ");
                            scanf("%19s", nome);

                            Tarefa tarefa_encontrada = buscar_tarefa(fila, nome);
                            if (strcmp(tarefa_encontrada.nome_tarefa, "") != 0) {
                                printf("Tarefa encontrada: Nome: %s, Data: %s, Prazo: %s\n", 
                                    tarefa_encontrada.nome_tarefa, tarefa_encontrada.data, tarefa_encontrada.prazo);
                            } else {
                                printf("Tarefa não encontrada.\n");
                            }
                            break;
                        }

                    case 4:
                        imprimir_tarefa(fila);
                        break;

                    default:
                        printf("Número inválido");
                }
                break;

            case 3:
                printf("Saindo...\n");
                break;

            default:
                printf("Número inválido");
        }
    }

    // Limpeza de memória
    while (lista->primeira_tarefa != NULL) {
        Celula* temp = lista->primeira_tarefa;
        lista->primeira_tarefa = lista->primeira_tarefa->proximo;
        free(temp);
    }
    free(lista);
    free(fila);

    return 0;
}
