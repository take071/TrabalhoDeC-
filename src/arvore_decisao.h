#ifndef ARVORE_DECISAO_H
#define ARVORE_DECISAO_H

#include "conteudo.h"

// =============================================
//  Classe Arvore Binaria de Decisao
//  Navega perguntas Sim/Nao para recomendar
//  Segue o padrao de classe do ArvoreB.cpp
// =============================================

class ArvoreDecisao {
public:
    NodoDecisao* raiz;

    // Metodos
    NodoDecisao* criarNo(string pergunta, bool ehFolha,
                          Tipo tipo, Genero genero, FiltroAno filtroAno = QUALQUER_ANO);
    void construirArvore();
    void navegar(NodoDecisao* no, Tipo &tipoResult, Genero &generoResult, FiltroAno &filtroAnoResult);
    void imprimir(NodoDecisao* no, int espaco);
    void imprimirArvore();

    ArvoreDecisao();
};

#endif
