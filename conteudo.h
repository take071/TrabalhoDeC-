#ifndef CONTEUDO_H
#define CONTEUDO_H

#include <iostream>
#include <string>
using namespace std;

// =============================================
//  Enums para Tipo e Genero de conteudo
// =============================================

enum Tipo { FILME, SERIE, DOCUMENTARIO, ANIME, OUTRO_TIPO };
enum Genero { ACAO, COMEDIA, DRAMA, TERROR, FICCAO, OUTRO_GENERO };
enum FiltroAno { QUALQUER_ANO, APENAS_RECENTES, APENAS_CLASSICOS };

// =============================================
//  Struct do conteudo (no da lista simples)
// =============================================
struct Conteudo {
    string nome;
    Tipo tipo;
    Genero genero;
    int anoLancamento;
    int visualizacoes;
    float avaliacao;       // media de estrelas (1-5)
    int totalAvaliacoes;   // quantas vezes foi avaliado
    Conteudo* prox;
};

// =============================================
//  Struct do historico (no da lista dupla)
// =============================================
struct ConteudoHistorico {
    string nome;
    Tipo tipo;
    Genero genero;
    int vezesAssistido;
    ConteudoHistorico* prox;
    ConteudoHistorico* ant;
};

// =============================================
//  Struct da arvore de decisao
// =============================================
struct NodoDecisao {
    string pergunta;
    bool ehFolha;
    Tipo tipoFiltro;
    Genero generoFiltro;
    FiltroAno filtroAno;
    NodoDecisao* esq;  // resposta "Sim"
    NodoDecisao* dir;   // resposta "Nao"
};

// =============================================
//  Funcoes auxiliares para converter enums
// =============================================

inline string tipoParaString(Tipo t) {
    switch (t) {
        case FILME:         return "Filme";
        case SERIE:         return "Serie";
        case DOCUMENTARIO:  return "Documentario";
        case ANIME:         return "Anime";
        case OUTRO_TIPO:    return "Outro";
        default:            return "Desconhecido";
    }
}

inline string generoParaString(Genero g) {
    switch (g) {
        case ACAO:          return "Acao";
        case COMEDIA:       return "Comedia";
        case DRAMA:         return "Drama";
        case TERROR:        return "Terror";
        case FICCAO:        return "Ficcao Cientifica";
        case OUTRO_GENERO:  return "Outro";
        default:            return "Desconhecido";
    }
}

inline Tipo lerTipo() {
    int op;
    cout << "\n Tipos disponiveis:";
    cout << "\n  0 - Filme";
    cout << "\n  1 - Serie";
    cout << "\n  2 - Documentario";
    cout << "\n  3 - Anime";
    cout << "\n  4 - Outro";
    cout << "\n Escolha: ";
    cin >> op;
    if (op < 0 || op > 4) op = 4;
    return (Tipo)op;
}

inline Genero lerGenero() {
    int op;
    cout << "\n Generos disponiveis:";
    cout << "\n  0 - Acao";
    cout << "\n  1 - Comedia";
    cout << "\n  2 - Drama";
    cout << "\n  3 - Terror";
    cout << "\n  4 - Ficcao Cientifica";
    cout << "\n  5 - Outro";
    cout << "\n Escolha: ";
    cin >> op;
    if (op < 0 || op > 5) op = 5;
    return (Genero)op;
}

// Exibe estrelas visuais no terminal
inline void exibirEstrelas(float media) {
    int cheias = (int)media;
    cout << " ";
    for (int i = 0; i < cheias; i++) cout << "★";
    for (int i = cheias; i < 5; i++) cout << "☆";
    cout << " (" << media << "/5.0)";
}

#endif
