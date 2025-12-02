#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define ANO_MINIMO 1500
#define ANO_MAXIMO 2025
#define MAX_EXEMPLARES 15
#define MAX_TITULO 100
#define MAX_LOCALIZACAO 30

#define GERAR_ISBN() (rand() % (99999 - 10000 + 1) + 10000)
#define GERAR_NUMERO_EXEMPLAR(isbn, sequencial) ((isbn) * 100 + (sequencial))

typedef struct Exemplar {
    int numero;
    char localizacao[MAX_LOCALIZACAO + 1];
    char estado;
    char status;
    struct Exemplar *proximo;
    struct Exemplar *anterior;
} Exemplar;

typedef struct Livro {
    char titulo[MAX_TITULO + 1];
    int isbn;
    int anoPublicacao;
    char categoria;
    float multaDiaria;
    int prazoEmprestimo;
    int qtdeExemplares;
    Exemplar *exemplares;
    struct Livro *proximo;
} Livro;

void cadastrarTitulo(char *titulo);
int cadastrarAnoPublicacao();
char cadastrarCategoria();
void cadastrarMulta(float *multa, char categoria);
int cadastrarPrazo(int anoPublicacao);
void cadastrarQtdeExemplares(Livro *livro);
void cadastrarLocalizacao(char *localizacao);
void cadastrarEstado(char *estado);
char cadastrarStatus();

Livro* inserirLivro(Livro *lista);
Exemplar* inserirExemplarNoFim(Exemplar *lista, int isbn, int sequencial);
Livro* inserirExemplarEmLivro(Livro *lista);

Livro* excluirLivro(Livro *lista);
Livro* excluirExemplarDeLivro(Livro *lista);
void liberarExemplares(Exemplar *lista);

Livro* buscarLivroPorISBN(Livro *lista, int isbn);

void listarCompleto(Livro *lista, int *totalLivros, int *somaPrazos);
int listarPorISBN(Livro *lista, int isbn);
int listarPorCategoria(Livro *lista, char categoria);
void listarExemplaresDisponiveis(Livro *lista);
void listarExemplaresDisponiveisRecursivo(Exemplar *exemplar, int isbn, char *titulo);

void exibirLivro(Livro *livro);
void exibirExemplar(Exemplar *exemplar, int isbn, char *titulo);
void limparBuffer();

#endif

