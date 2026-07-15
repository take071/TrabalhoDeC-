#include "lista_dupla.h"

// =============================================
//  Construtor
// =============================================
ListaDupla::ListaDupla() {
    inicio = nullptr;
}

// =============================================
//  Inserir ou Atualizar conteudo no historico
//  Se ja existe, incrementa vezesAssistido e reordena
//  Se nao existe, cria novo no e insere ordenado
// =============================================
void ListaDupla::inserirOuAtualizar(string nome, Tipo tipo, Genero genero) {
    // Busca se ja existe no historico
    ConteudoHistorico* atual = inicio;
    ConteudoHistorico* encontrado = nullptr;

    while (atual != nullptr) {
        if (atual->nome == nome) {
            encontrado = atual;
            break;
        }
        atual = atual->prox;
    }

    if (encontrado != nullptr) {
        // Incrementa e reposiciona
        encontrado->vezesAssistido++;

        // Remove da posicao atual
        if (encontrado->ant != nullptr)
            encontrado->ant->prox = encontrado->prox;
        else
            inicio = encontrado->prox;

        if (encontrado->prox != nullptr)
            encontrado->prox->ant = encontrado->ant;

        // Reinsere na posicao correta (decrescente)
        encontrado->prox = nullptr;
        encontrado->ant = nullptr;

        if (inicio == nullptr || encontrado->vezesAssistido >= inicio->vezesAssistido) {
            encontrado->prox = inicio;
            if (inicio != nullptr) inicio->ant = encontrado;
            inicio = encontrado;
            return;
        }

        atual = inicio;
        while (atual->prox != nullptr &&
               atual->prox->vezesAssistido > encontrado->vezesAssistido) {
            atual = atual->prox;
        }
        encontrado->prox = atual->prox;
        encontrado->ant = atual;
        if (atual->prox != nullptr) atual->prox->ant = encontrado;
        atual->prox = encontrado;
    }
    else {
        // Cria novo no com 1 visualizacao
        ConteudoHistorico* novo = new ConteudoHistorico();
        if (novo == nullptr) {
            cout << "\n ERRO: Memoria insuficiente!\n";
            return;
        }
        novo->nome = nome;
        novo->tipo = tipo;
        novo->genero = genero;
        novo->vezesAssistido = 1;
        novo->prox = nullptr;
        novo->ant = nullptr;

        // Insere no final (1 viz = menor prioridade)
        if (inicio == nullptr) {
            inicio = novo;
            return;
        }

        atual = inicio;
        while (atual->prox != nullptr) {
            atual = atual->prox;
        }
        atual->prox = novo;
        novo->ant = atual;
    }
}

// =============================================
//  Listar historico de mais assistidos
// =============================================
void ListaDupla::listar() {
    if (inicio == nullptr) {
        cout << "\n Nenhum conteudo assistido ainda.\n";
        return;
    }

    cout << "\n===== HISTORICO - MAIS ASSISTIDOS =====\n";
    ConteudoHistorico* atual = inicio;
    int pos = 1;
    while (atual != nullptr) {
        cout << "\n " << pos << ". " << atual->nome;
        cout << "\n    Tipo: " << tipoParaString(atual->tipo);
        cout << "  |  Genero: " << generoParaString(atual->genero);
        cout << "\n    Assistido: " << atual->vezesAssistido << " vez(es)\n";
        atual = atual->prox;
        pos++;
    }
}

// =============================================
//  Retorna o titulo mais assistido de um tipo
// =============================================
ConteudoHistorico* ListaDupla::getMaisAssistidoPorTipo(Tipo tipo) {
    ConteudoHistorico* atual = inicio;
    while (atual != nullptr) {
        if (atual->tipo == tipo)
            return atual;  // O primeiro encontrado e o mais assistido (lista ordenada)
        atual = atual->prox;
    }
    return nullptr;
}

// =============================================
//  Retorna o titulo mais assistido de um genero
// =============================================
ConteudoHistorico* ListaDupla::getMaisAssistidoPorGenero(Genero genero) {
    ConteudoHistorico* atual = inicio;
    while (atual != nullptr) {
        if (atual->genero == genero)
            return atual;
        atual = atual->prox;
    }
    return nullptr;
}
