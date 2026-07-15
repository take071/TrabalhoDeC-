#ifndef LISTA_DUPLA_H
#define LISTA_DUPLA_H

#include "conteudo.h"

// =============================================
//  Classe Lista Duplamente Encadeada Ordenada
//  Armazena historico dos mais assistidos
//  Ordenada decrescente por vezesAssistido
// =============================================

class ListaDupla {
public:
    ConteudoHistorico* inicio;

    // Metodos
    void inserirOuAtualizar(string nome, Tipo tipo, Genero genero);
    void listar();
    ConteudoHistorico* getMaisAssistidoPorTipo(Tipo tipo);
    ConteudoHistorico* getMaisAssistidoPorGenero(Genero genero);

    ListaDupla();
};

#endif
