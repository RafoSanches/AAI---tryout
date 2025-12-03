/*
 * Sistema de Controle de Acervo e Empréstimos
 * Biblioteca 
 * 
 * Integrantes do Grupo:
 * -Rafael Sanches 
 * -Arthur
 * 
 */

#include "biblioteca.h"

void exibirMenuPrincipal() {
    printf("\n========================================\n");
    printf("    SISTEMA DE CONTROLE DE ACERVO\n");
    printf("========================================\n");
    printf("A - Inserir\n");
    printf("B - Excluir\n");
    printf("C - Relatórios\n");
    printf("D - Finalizar\n");
    printf("========================================\n");
    printf("Escolha uma opção: ");
}

void exibirSubmenuInserir() {
    printf("\n--- SUBMENU: INSERIR ---\n");
    printf("1 - Efetuar inclusão de livro\n");
    printf("2 - Efetuar inclusão de exemplar\n");
    printf("0 - Voltar ao menu principal\n");
    printf("Escolha uma opção: ");
}

void exibirSubmenuExcluir() {
    printf("\n--- SUBMENU: EXCLUIR ---\n");
    printf("1 - Efetuar exclusão de livro\n");
    printf("2 - Efetuar exclusão de exemplar\n");
    printf("0 - Voltar ao menu principal\n");
    printf("Escolha uma opção: ");
}

void exibirSubmenuRelatorios() {
    printf("\n--- SUBMENU: RELATÓRIOS ---\n");
    printf("1 - Lista completa\n");
    printf("2 - Lista por ISBN\n");
    printf("3 - Lista por categoria\n");
    printf("4 - Lista de exemplares disponíveis\n");
    printf("0 - Voltar ao menu principal\n");
    printf("Escolha uma opção: ");
}

Livro* processarSubmenuInserir(Livro *lista) {
    int opcao;
    do {
        exibirSubmenuInserir();
        scanf("%d", &opcao);
        limparBuffer();
        
        switch (opcao) {
            case 1:
                lista = inserirLivro(lista);
                break;
            case 2:
                lista = inserirExemplarEmLivro(lista);
                break;
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    
    return lista;
}

Livro* processarSubmenuExcluir(Livro *lista) {
    int opcao;
    do {
        exibirSubmenuExcluir();
        scanf("%d", &opcao);
        limparBuffer();
        
        switch (opcao) {
            case 1:
                lista = excluirLivro(lista);
                break;
            case 2:
                lista = excluirExemplarDeLivro(lista);
                break;
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    
    return lista;
}

void processarSubmenuRelatorios(Livro *lista) {
    int opcao;
    do {
        exibirSubmenuRelatorios();
        scanf("%d", &opcao);
        limparBuffer();
        
        switch (opcao) {
            case 1: {
                printf("\n--- LISTA COMPLETA ---\n");
                int totalLivros = 0;
                int somaPrazos = 0;
                listarCompleto(lista, &totalLivros, &somaPrazos);
                
                if (totalLivros > 0) {
                    float mediaPrazos = (float)somaPrazos / totalLivros;
                    printf("\nMédia dos prazos de empréstimo: %.2f dias\n", mediaPrazos);
                } else {
                    printf("Nenhum livro cadastrado.\n");
                }
                break;
            }
            case 2: {
                int isbn;
                printf("Digite o ISBN do livro: ");
                scanf("%d", &isbn);
                limparBuffer();
                printf("\n--- LISTA POR ISBN ---\n");
                if (!listarPorISBN(lista, isbn)) {
                    printf("Livro não encontrado!\n");
                }
                break;
            }
            case 3: {
                char categoria;
                printf("Digite a categoria (F-Ficção, T-Técnico, D-Didático, I-Infantil): ");
                scanf(" %c", &categoria);
                limparBuffer();
                categoria = toupper(categoria);
                printf("\n--- LISTA POR CATEGORIA ---\n");
                if (listarPorCategoria(lista, categoria) == 0) {
                    printf("Nenhum livro encontrado nesta categoria.\n");
                }
                break;
            }
            case 4:
                printf("\n--- LISTA DE EXEMPLARES DISPONÍVEIS ---\n");
                listarExemplaresDisponiveis(lista);
                break;
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

int main() {
    srand(time(NULL));
    
    Livro *lista = NULL;
    char opcao;
    
    printf("Bem-vindo ao Sistema de Controle de Acervo e Empréstimos!\n");
    
    do {
        exibirMenuPrincipal();
        scanf(" %c", &opcao);
        limparBuffer();
        opcao = toupper(opcao);
        
        switch (opcao) {
            case 'A':
                lista = processarSubmenuInserir(lista);
                break;
            case 'B':
                lista = processarSubmenuExcluir(lista);
                break;
            case 'C':
                processarSubmenuRelatorios(lista);
                break;
            case 'D':
                printf("Finalizando o sistema...\n");
                break;
            default:
                printf("Opção inválida! Digite A, B, C ou D.\n");
        }
    } while (opcao != 'D');
    
    Livro *atual = lista;
    while (atual != NULL) {
        Livro *proximo = atual->proximo;
        liberarExemplares(atual->exemplares);
        free(atual);
        atual = proximo;
    }
    
    printf("Sistema finalizado. Obrigado!\n");
    
    return 0;
}

