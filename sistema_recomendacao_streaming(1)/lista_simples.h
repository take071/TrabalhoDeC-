#ifndef LISTA_SIMPLES_H
#define LISTA_SIMPLES_H

#include "conteudo.h"


//  Armazena o catalogo de conteudos (por nome)
class ListaSimples {
public:
    Conteudo* inicio;

    // Metodos
    void inserirOrdenado(string nome, Tipo tipo, Genero genero,
                         int ano, int viz, bool silencioso = false);
    Conteudo* buscar(string nome);
    bool remover(string nome);
    bool removerPorIndice(int pos);
    void listar();
    int contarTotal();
    void listarFiltrado(Tipo tipo, Genero genero, FiltroAno filtroAno = QUALQUER_ANO);

    // Persistencia em arquivo 
    void salvarArquivo(string caminho);
    void carregarArquivo(string caminho);

    ListaSimples();
};

#endif
