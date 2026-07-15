#include "lista_simples.h"

// =============================================
//  Construtor
// =============================================
ListaSimples::ListaSimples() {
    inicio = nullptr;
}

// =============================================
//  Inserir conteudo em ordem alfabetica (por nome)
// =============================================
void ListaSimples::inserirOrdenado(string nome, Tipo tipo, Genero genero,
                                    int ano, int viz) {
    // Verifica se ja existe
    if (buscar(nome) != nullptr) {
        cout << "\n ERRO: Conteudo '" << nome << "' ja cadastrado!\n";
        return;
    }

    // Cria novo no
    Conteudo* novo = new Conteudo();
    if (novo == nullptr) {
        cout << "\n ERRO: Memoria insuficiente!\n";
        return;
    }
    novo->nome = nome;
    novo->tipo = tipo;
    novo->genero = genero;
    novo->anoLancamento = ano;
    novo->visualizacoes = viz;
    novo->avaliacao = 0.0;
    novo->totalAvaliacoes = 0;
    novo->prox = nullptr;

    // Lista vazia ou insere antes do inicio
    if (inicio == nullptr || nome < inicio->nome) {
        novo->prox = inicio;
        inicio = novo;
        cout << "\n Conteudo '" << nome << "' cadastrado com sucesso!\n";
        return;
    }

    // Percorre ate encontrar posicao correta
    Conteudo* atual = inicio;
    while (atual->prox != nullptr && atual->prox->nome < nome) {
        atual = atual->prox;
    }
    novo->prox = atual->prox;
    atual->prox = novo;

    cout << "\n Conteudo '" << nome << "' cadastrado com sucesso!\n";
}

// =============================================
//  Buscar conteudo por nome
// =============================================
Conteudo* ListaSimples::buscar(string nome) {
    Conteudo* atual = inicio;
    while (atual != nullptr) {
        if (atual->nome == nome)
            return atual;
        atual = atual->prox;
    }
    return nullptr;
}

// =============================================
//  Remover conteudo por nome
// =============================================
bool ListaSimples::remover(string nome) {
    if (inicio == nullptr) {
        cout << "\n Lista vazia!\n";
        return false;
    }

    Conteudo* ret;

    // Se e o primeiro elemento
    if (inicio->nome == nome) {
        ret = inicio;
        inicio = inicio->prox;
        delete ret;
        cout << "\n Conteudo '" << nome << "' removido!\n";
        return true;
    }

    // Percorre a lista
    Conteudo* atual = inicio;
    while (atual->prox != nullptr && atual->prox->nome != nome) {
        atual = atual->prox;
    }

    if (atual->prox == nullptr) {
        cout << "\n Conteudo '" << nome << "' nao encontrado!\n";
        return false;
    }

    ret = atual->prox;
    atual->prox = ret->prox;
    delete ret;
    cout << "\n Conteudo '" << nome << "' removido!\n";
    return true;
}

// =============================================
//  Remover conteudo por indice/posicao (1-based)
// =============================================
bool ListaSimples::removerPorIndice(int pos) {
    if (inicio == nullptr) {
        cout << "\n Lista vazia!\n";
        return false;
    }

    if (pos <= 0) {
        cout << "\n Indice invalido!\n";
        return false;
    }

    Conteudo* ret;

    if (pos == 1) {
        ret = inicio;
        inicio = inicio->prox;
        cout << "\n Conteudo '" << ret->nome << "' removido!\n";
        delete ret;
        return true;
    }

    Conteudo* atual = inicio;
    int idx = 1;

    // Percorre ate o penultimo antes do desejado
    while (atual->prox != nullptr && idx < pos - 1) {
        atual = atual->prox;
        idx++;
    }

    if (atual->prox == nullptr) {
        cout << "\n Indice nao encontrado (fora dos limites)!\n";
        return false;
    }

    ret = atual->prox;
    atual->prox = ret->prox;
    cout << "\n Conteudo '" << ret->nome << "' removido!\n";
    delete ret;
    return true;
}

// =============================================
//  Listar todos os conteudos
// =============================================
void ListaSimples::listar() {
    if (inicio == nullptr) {
        cout << "\n Nenhum conteudo cadastrado.\n";
        return;
    }

    cout << "\n===== CATALOGO DE CONTEUDOS =====\n";
    Conteudo* atual = inicio;
    int i = 1;
    while (atual != nullptr) {
        cout << "\n " << i << ". " << atual->nome;
        cout << "\n    Tipo: " << tipoParaString(atual->tipo);
        cout << "  |  Genero: " << generoParaString(atual->genero);
        cout << "\n    Ano: " << atual->anoLancamento;
        cout << "  |  Visualizacoes: " << atual->visualizacoes;
        if (atual->totalAvaliacoes > 0) {
            cout << "\n    Avaliacao:";
            exibirEstrelas(atual->avaliacao);
            cout << " (" << atual->totalAvaliacoes << " avaliacoes)";
        }
        cout << "\n";
        atual = atual->prox;
        i++;
    }
}

// =============================================
//  Contar total de conteudos
// =============================================
int ListaSimples::contarTotal() {
    int cont = 0;
    Conteudo* atual = inicio;
    while (atual != nullptr) {
        cont++;
        atual = atual->prox;
    }
    return cont;
}

// =============================================
//  Listar filtrado por tipo e/ou genero
//  Usado na recomendacao apos navegar a arvore
// =============================================
void ListaSimples::listarFiltrado(Tipo tipo, Genero genero, FiltroAno filtroAno) {
    Conteudo* atual = inicio;
    int encontrados = 0;
    int index = 1;

    while (atual != nullptr) {
        bool match = true;
        // Filtra por tipo (OUTRO_TIPO = qualquer tipo)
        if (tipo != OUTRO_TIPO && atual->tipo != tipo) match = false;
        // Filtra por genero (OUTRO_GENERO = qualquer genero)
        if (genero != OUTRO_GENERO && atual->genero != genero) match = false;
        
        // Filtra pelo ano (recentes: >= 2021 | classicos: < 2005)
        if (filtroAno == APENAS_RECENTES && atual->anoLancamento < 2021) match = false;
        if (filtroAno == APENAS_CLASSICOS && atual->anoLancamento >= 2005) match = false;

        if (match) {
            encontrados++;
            cout << "\n  " << index << ". " << atual->nome;
            cout << " (" << tipoParaString(atual->tipo) << " - "
                 << generoParaString(atual->genero) << ", "
                 << atual->anoLancamento << ")";
            cout << "  [" << atual->visualizacoes << " viz]";
            if (atual->totalAvaliacoes > 0) {
                exibirEstrelas(atual->avaliacao);
            }
        }
        atual = atual->prox;
        index++;
    }

    if (encontrados == 0) {
        cout << "\n  Nenhum conteudo encontrado para este filtro.";
    }
}
