#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

#include "lista_simples.h"
#include "lista_dupla.h"

//  Gera relatorios e gerencia avaliacoes
class Estatisticas {
public:
    int totalRecomendacoes;
    int contRecomTipo[5];   
    int contRecomGenero[6]; 

    void registrarRecomendacao(Tipo tipo, Genero genero);
    void exibirEstatisticas(ListaSimples &catalogo, ListaDupla &historico);
    void avaliarConteudo(ListaSimples &catalogo);
    void listarPorAvaliacao(ListaSimples &catalogo);

    Estatisticas();
};

#endif
