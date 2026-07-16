#ifndef CONTEUDO_H
#define CONTEUDO_H

#include <iostream>
#include <string>
using namespace std;

//  Codigos ANSI para cores no terminal
#define COR_RESET   "\033[0m"
#define COR_ERRO    "\033[1;31m"   // vermelho
#define COR_SUCESSO "\033[1;32m"   // verde
#define COR_TITULO  "\033[1;36m"   // ciano
#define COR_AVISO   "\033[1;33m"   // amarelo

// Largura padrao usada para centralizar textos no CLI
#define LARGURA_CLI 80

enum Tipo { FILME, SERIE, DOCUMENTARIO, ANIME, OUTRO_TIPO };
enum Genero { ACAO, COMEDIA, DRAMA, TERROR, FICCAO, OUTRO_GENERO };
enum FiltroAno { QUALQUER_ANO, APENAS_RECENTES, APENAS_CLASSICOS };

struct Conteudo {
    string nome;
    Tipo tipo;
    Genero genero;
    int anoLancamento;
    int visualizacoes;
    float avaliacao;       
    int totalAvaliacoes;   
    Conteudo* prox;
};

struct ConteudoHistorico {
    string nome;
    Tipo tipo;
    Genero genero;
    int vezesAssistido;
    ConteudoHistorico* prox;
    ConteudoHistorico* ant;
};

struct NodoDecisao {
    string pergunta;
    bool ehFolha;
    Tipo tipoFiltro;
    Genero generoFiltro;
    FiltroAno filtroAno;
    NodoDecisao* esq;  // resposta "Sim"
    NodoDecisao* dir;   // resposta "Nao"
};

//  Funcoes auxiliares para converter enums
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

//  Centraliza um texto com base na largura do CLI
inline string centralizar(const string &texto, int largura = LARGURA_CLI) {
    int espacos = (largura - (int)texto.length()) / 2;
    if (espacos < 0) espacos = 0;
    return string(espacos, ' ') + texto;
}

//  Exibe uma linha ja centralizada no terminal
inline void imprimirCentralizado(const string &texto, int largura = LARGURA_CLI) {
    cout << "\n" << centralizar(texto, largura);
}

inline void exibirBanner() {
    const int LARGURA_BOX = 55;
    string texto = "FLIX RECOMENDACOES DE STREAMING";

    string borda = string(LARGURA_BOX, '=');

    int padTexto = (LARGURA_BOX - 4 - (int)texto.length()) / 2;
    if (padTexto < 0) padTexto = 0;
    int padDireita = LARGURA_BOX - 4 - padTexto - (int)texto.length();
    if (padDireita < 0) padDireita = 0;

    string linhaVazia = "||" + string(LARGURA_BOX - 4, ' ') + "||";
    string linhaTexto = "||" + string(padTexto, ' ') + texto + string(padDireita, ' ') + "||";

    int margem = (LARGURA_CLI - LARGURA_BOX) / 2;
    if (margem < 0) margem = 0;
    string esq(margem, ' ');

    cout << COR_TITULO;
    cout << "\n" << esq << borda;
    cout << "\n" << esq << linhaVazia;
    cout << "\n" << esq << linhaTexto;
    cout << "\n" << esq << linhaVazia;
    cout << "\n" << esq << borda;
    cout << COR_RESET << "\n";
}

//  Cabecalho ASCII centralizado reutilizavel para
//  os titulos de cada tela do sistema
//  (mesma logica de margem unica do banner acima)
inline void exibirCabecalho(const string &titulo) {
    int larguraBox = (int)titulo.length() + 8;
    if (larguraBox < 44) larguraBox = 44;

    string borda = string(larguraBox, '=');

    int padTitulo = (larguraBox - (int)titulo.length()) / 2;
    if (padTitulo < 0) padTitulo = 0;
    string linhaTitulo = string(padTitulo, ' ') + titulo;

    int margem = (LARGURA_CLI - larguraBox) / 2;
    if (margem < 0) margem = 0;
    string esq(margem, ' ');

    cout << COR_TITULO;
    cout << "\n" << esq << borda;
    cout << "\n" << esq << linhaTitulo;
    cout << "\n" << esq << borda;
    cout << COR_RESET << "\n";
}
#endif