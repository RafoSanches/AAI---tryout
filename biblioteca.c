#include "biblioteca.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cadastrarTitulo(char *titulo) {
    printf("Digite o título do livro (máximo %d caracteres): ", MAX_TITULO);
    fgets(titulo, MAX_TITULO + 1, stdin);
    int len = strlen(titulo);
    if (len > 0 && titulo[len - 1] == '\n') {
        titulo[len - 1] = '\0';
    }
}

int cadastrarAnoPublicacao() {
    int ano;
    do {
        printf("Digite o ano de publicação (%d a %d): ", ANO_MINIMO, ANO_MAXIMO);
        scanf("%d", &ano);
        limparBuffer();
        if (ano < ANO_MINIMO || ano > ANO_MAXIMO) {
            printf("Ano inválido! Digite um ano entre %d e %d.\n", ANO_MINIMO, ANO_MAXIMO);
        }
    } while (ano < ANO_MINIMO || ano > ANO_MAXIMO);
    return ano;
}

char cadastrarCategoria() {
    char categoria;
    do {
        printf("Digite a categoria (F-Ficção, T-Técnico, D-Didático, I-Infantil): ");
        scanf(" %c", &categoria);
        limparBuffer();
        categoria = toupper(categoria);
        if (categoria != 'F' && categoria != 'T' && categoria != 'D' && categoria != 'I') {
            printf("Categoria inválida! Digite F, T, D ou I.\n");
        }
    } while (categoria != 'F' && categoria != 'T' && categoria != 'D' && categoria != 'I');
    return categoria;
}

void cadastrarMulta(float *multa, char categoria) {
    switch (categoria) {
        case 'F':
            *multa = 1.50;
            break;
        case 'T':
            *multa = 3.00;
            break;
        case 'D':
            *multa = 2.00;
            break;
        case 'I':
            *multa = 1.00;
            break;
        default:
            *multa = 0.0;
    }
}

int cadastrarPrazo(int anoPublicacao) {
    if (anoPublicacao <= 1950) {
        return 7;
    } else if (anoPublicacao <= 2000) {
        return 14;
    } else if (anoPublicacao <= 2015) {
        return 21;
    } else {
        return 30;
    }
}

void cadastrarQtdeExemplares(Livro *livro) {
    int qtde;
    do {
        printf("Digite a quantidade de exemplares do livro [%s] (1 a %d): ", 
               livro->titulo, MAX_EXEMPLARES);
        scanf("%d", &qtde);
        limparBuffer();
        if (qtde < 1 || qtde > MAX_EXEMPLARES) {
            printf("Quantidade inválida! Digite um valor entre 1 e %d.\n", MAX_EXEMPLARES);
        }
    } while (qtde < 1 || qtde > MAX_EXEMPLARES);
    livro->qtdeExemplares = qtde;
}

void cadastrarLocalizacao(char *localizacao) {
    printf("Digite a localização do exemplar (máximo %d caracteres): ", MAX_LOCALIZACAO);
    fgets(localizacao, MAX_LOCALIZACAO + 1, stdin);
    int len = strlen(localizacao);
    if (len > 0 && localizacao[len - 1] == '\n') {
        localizacao[len - 1] = '\0';
    }
}

void cadastrarEstado(char *estado) {
    do {
        printf("Digite o estado de conservação (O-Ótimo, B-Bom, R-Regular, P-Precário): ");
        scanf(" %c", estado);
        limparBuffer();
        *estado = toupper(*estado);
        if (*estado != 'O' && *estado != 'B' && *estado != 'R' && *estado != 'P') {
            printf("Estado inválido! Digite O, B, R ou P.\n");
        }
    } while (*estado != 'O' && *estado != 'B' && *estado != 'R' && *estado != 'P');
}

char cadastrarStatus() {
    char status;
    char entrada[10];
    printf("Digite o status (D-Disponível, E-Emprestado, M-Manutenção) [padrão: D]: ");
    fgets(entrada, sizeof(entrada), stdin);
    
    if (entrada[0] == '\n' || entrada[0] == '\0') {
        return 'D';
    }
    
    status = toupper(entrada[0]);
    while (status != 'D' && status != 'E' && status != 'M') {
        printf("Status inválido! Digite D, E ou M.\n");
        printf("Digite o status (D-Disponível, E-Emprestado, M-Manutenção) [padrão: D]: ");
        fgets(entrada, sizeof(entrada), stdin);
        if (entrada[0] == '\n' || entrada[0] == '\0') {
            return 'D';
        }
        status = toupper(entrada[0]);
    }
    
    return status;
}

Exemplar* inserirExemplarNoFim(Exemplar *lista, int isbn, int sequencial) {
    Exemplar *novo = (Exemplar*)malloc(sizeof(Exemplar));
    if (novo == NULL) {
        printf("Erro ao alocar memória!\n");
        return lista;
    }
    
    novo->numero = GERAR_NUMERO_EXEMPLAR(isbn, sequencial);
    cadastrarLocalizacao(novo->localizacao);
    cadastrarEstado(&(novo->estado));
    novo->status = cadastrarStatus();
    novo->proximo = NULL;
    novo->anterior = NULL;
    
    if (lista == NULL) {
        return novo;
    }
    
    Exemplar *atual = lista;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }
    atual->proximo = novo;
    novo->anterior = atual;
    
    return lista;
}

Livro* inserirLivro(Livro *lista) {
    Livro *novo = (Livro*)malloc(sizeof(Livro));
    if (novo == NULL) {
        printf("Erro ao alocar memória!\n");
        return lista;
    }
    
    cadastrarTitulo(novo->titulo);
    novo->isbn = GERAR_ISBN();
    novo->anoPublicacao = cadastrarAnoPublicacao();
    novo->categoria = cadastrarCategoria();
    cadastrarMulta(&(novo->multaDiaria), novo->categoria);
    novo->prazoEmprestimo = cadastrarPrazo(novo->anoPublicacao);
    novo->exemplares = NULL;
    
    cadastrarQtdeExemplares(novo);
    
    for (int i = 1; i <= novo->qtdeExemplares; i++) {
        printf("\n--- Cadastro do Exemplar %d ---\n", i);
        novo->exemplares = inserirExemplarNoFim(novo->exemplares, novo->isbn, i);
    }
    
    novo->proximo = lista;
    
    printf("\nLivro cadastrado com sucesso! ISBN: %d\n", novo->isbn);
    return novo;
}

Livro* inserirExemplarEmLivro(Livro *lista) {
    if (lista == NULL) {
        printf("Não há livros cadastrados!\n");
        return lista;
    }
    
    int isbn;
    printf("Digite o ISBN do livro: ");
    scanf("%d", &isbn);
    limparBuffer();
    
    Livro *livro = buscarLivroPorISBN(lista, isbn);
    if (livro == NULL) {
        printf("Livro não encontrado!\n");
        return lista;
    }
    
    if (livro->qtdeExemplares >= MAX_EXEMPLARES) {
        printf("Limite máximo de exemplares atingido para este livro!\n");
        return lista;
    }
    
    int sequencial = livro->qtdeExemplares + 1;
    livro->exemplares = inserirExemplarNoFim(livro->exemplares, livro->isbn, sequencial);
    livro->qtdeExemplares++;
    
    printf("Exemplar cadastrado com sucesso! Número: %d\n", 
           GERAR_NUMERO_EXEMPLAR(livro->isbn, sequencial));
    
    return lista;
}

Livro* buscarLivroPorISBN(Livro *lista, int isbn) {
    Livro *atual = lista;
    while (atual != NULL) {
        if (atual->isbn == isbn) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void liberarExemplares(Exemplar *lista) {
    if (lista == NULL) {
        return;
    }
    liberarExemplares(lista->proximo);
    free(lista);
}

Livro* excluirLivro(Livro *lista) {
    if (lista == NULL) {
        printf("Não há livros cadastrados!\n");
        return lista;
    }
    
    int isbn;
    printf("Digite o ISBN do livro a ser excluído: ");
    scanf("%d", &isbn);
    limparBuffer();
    
    Livro *atual = lista;
    Livro *anterior = NULL;
    
    while (atual != NULL && atual->isbn != isbn) {
        anterior = atual;
        atual = atual->proximo;
    }
    
    if (atual == NULL) {
        printf("Livro não encontrado!\n");
        return lista;
    }
    
    liberarExemplares(atual->exemplares);
    
    if (anterior == NULL) {
        lista = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    
    free(atual);
    printf("Livro excluído com sucesso!\n");
    
    return lista;
}

Livro* excluirExemplarDeLivro(Livro *lista) {
    if (lista == NULL) {
        printf("Não há livros cadastrados!\n");
        return lista;
    }
    
    int isbn;
    printf("Digite o ISBN do livro: ");
    scanf("%d", &isbn);
    limparBuffer();
    
    Livro *livro = buscarLivroPorISBN(lista, isbn);
    if (livro == NULL) {
        printf("Livro não encontrado!\n");
        return lista;
    }
    
    if (livro->qtdeExemplares <= 1) {
        printf("Não é possível excluir! Deve haver pelo menos 1 exemplar.\n");
        return lista;
    }
    
    int numero;
    printf("Digite o número do exemplar a ser excluído: ");
    scanf("%d", &numero);
    limparBuffer();
    
    Exemplar *atual = livro->exemplares;
    Exemplar *anterior = NULL;
    
    while (atual != NULL && atual->numero != numero) {
        anterior = atual;
        atual = atual->proximo;
    }
    
    if (atual == NULL) {
        printf("Exemplar não encontrado!\n");
        return lista;
    }
    
    if (anterior == NULL) {
        livro->exemplares = atual->proximo;
        if (livro->exemplares != NULL) {
            livro->exemplares->anterior = NULL;
        }
    } else {
        anterior->proximo = atual->proximo;
        if (atual->proximo != NULL) {
            atual->proximo->anterior = anterior;
        }
    }
    
    free(atual);
    livro->qtdeExemplares--;
    
    printf("Exemplar excluído com sucesso!\n");
    
    return lista;
}

void exibirExemplar(Exemplar *exemplar, int isbn, char *titulo) {
    if (exemplar == NULL) {
        return;
    }
    
    printf("  Número: %d\n", exemplar->numero);
    printf("  Localização: %s\n", exemplar->localizacao);
    printf("  Estado: ");
    switch (exemplar->estado) {
        case 'O': printf("Ótimo"); break;
        case 'B': printf("Bom"); break;
        case 'R': printf("Regular"); break;
        case 'P': printf("Precário"); break;
    }
    printf("\n  Status: ");
    switch (exemplar->status) {
        case 'D': printf("Disponível"); break;
        case 'E': printf("Emprestado"); break;
        case 'M': printf("Manutenção"); break;
    }
    printf("\n\n");
    
    exibirExemplar(exemplar->proximo, isbn, titulo);
}

void exibirLivro(Livro *livro) {
    if (livro == NULL) {
        return;
    }
    
    printf("\n========================================\n");
    printf("Título: %s\n", livro->titulo);
    printf("ISBN: %d\n", livro->isbn);
    printf("Ano de Publicação: %d\n", livro->anoPublicacao);
    printf("Categoria: ");
    switch (livro->categoria) {
        case 'F': printf("Ficção"); break;
        case 'T': printf("Técnico"); break;
        case 'D': printf("Didático"); break;
        case 'I': printf("Infantil"); break;
    }
    printf("\n");
    printf("Multa Diária: R$ %.2f\n", livro->multaDiaria);
    printf("Prazo de Empréstimo: %d dias\n", livro->prazoEmprestimo);
    printf("Quantidade de Exemplares: %d\n", livro->qtdeExemplares);
    printf("\n--- Exemplares ---\n");
    exibirExemplar(livro->exemplares, livro->isbn, livro->titulo);
    printf("========================================\n\n");
}

void listarCompleto(Livro *lista, int *totalLivros, int *somaPrazos) {
    if (lista == NULL) {
        return;
    }
    
    exibirLivro(lista);
    (*totalLivros)++;
    (*somaPrazos) += lista->prazoEmprestimo;
    
    listarCompleto(lista->proximo, totalLivros, somaPrazos);
}

int listarPorISBN(Livro *lista, int isbn) {
    if (lista == NULL) {
        return 0;
    }
    
    if (lista->isbn == isbn) {
        exibirLivro(lista);
        return 1;
    }
    
    return listarPorISBN(lista->proximo, isbn);
}

int listarPorCategoria(Livro *lista, char categoria) {
    if (lista == NULL) {
        return 0;
    }
    
    int count = 0;
    if (lista->categoria == categoria) {
        exibirLivro(lista);
        count = 1;
    }
    
    return count + listarPorCategoria(lista->proximo, categoria);
}

void listarExemplaresDisponiveisRecursivo(Exemplar *exemplar, int isbn, char *titulo) {
    if (exemplar == NULL) {
        return;
    }
    
    if (exemplar->status == 'D') {
        printf("\n--- Exemplar Disponível ---\n");
        printf("Livro: %s (ISBN: %d)\n", titulo, isbn);
        printf("  Número: %d\n", exemplar->numero);
        printf("  Localização: %s\n", exemplar->localizacao);
        printf("  Estado: ");
        switch (exemplar->estado) {
            case 'O': printf("Ótimo"); break;
            case 'B': printf("Bom"); break;
            case 'R': printf("Regular"); break;
            case 'P': printf("Precário"); break;
        }
        printf("\n  Status: Disponível\n\n");
    }
    
    listarExemplaresDisponiveisRecursivo(exemplar->proximo, isbn, titulo);
}

void listarExemplaresDisponiveis(Livro *lista) {
    if (lista == NULL) {
        return;
    }
    
    listarExemplaresDisponiveisRecursivo(lista->exemplares, lista->isbn, lista->titulo);
    listarExemplaresDisponiveis(lista->proximo);
}

