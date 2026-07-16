#ifndef ARVORE_DECISAO_H
#define ARVORE_DECISAO_H

#include "conteudo.h"

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