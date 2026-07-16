#ifndef LISTA_DUPLA_H
#define LISTA_DUPLA_H

#include "conteudo.h"

//  Armazena historico dos mais assistidos
//  Ordenada decrescente por vezesAssistido
class ListaDupla {
public:
    ConteudoHistorico* inicio;

    void inserirOuAtualizar(string nome, Tipo tipo, Genero genero);
    void listar();
    ConteudoHistorico* getMaisAssistidoPorTipo(Tipo tipo);
    ConteudoHistorico* getMaisAssistidoPorGenero(Genero genero);

    void salvarArquivo(string caminho);
    void carregarArquivo(string caminho);

    ListaDupla();
};

#endif
