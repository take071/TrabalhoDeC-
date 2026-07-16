#include "lista_simples.h"
#include <fstream>
#include <sstream>

ListaSimples::ListaSimples() {
    inicio = nullptr;
}

//  Inserir conteudo em ordem alfabetica (por nome)
void ListaSimples::inserirOrdenado(string nome, Tipo tipo, Genero genero,
                                    int ano, int viz, bool silencioso) {
    if (buscar(nome) != nullptr) {
        if (!silencioso)
            cout << COR_ERRO << "\n ERRO: Conteudo '" << nome << "' ja cadastrado!" << COR_RESET << "\n";
        return;
    }

    Conteudo* novo = new Conteudo();
    if (novo == nullptr) {
        if (!silencioso)
            cout << COR_ERRO << "\n ERRO: Memoria insuficiente!" << COR_RESET << "\n";
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

    if (inicio == nullptr || nome < inicio->nome) {
        novo->prox = inicio;
        inicio = novo;
        if (!silencioso)
            cout << COR_SUCESSO << "\n Conteudo '" << nome << "' cadastrado com sucesso!" << COR_RESET << "\n";
        return;
    }

    // Percorre ate encontrar posicao correta
    Conteudo* atual = inicio;
    while (atual->prox != nullptr && atual->prox->nome < nome) {
        atual = atual->prox;
    }
    novo->prox = atual->prox;
    atual->prox = novo;

    if (!silencioso)
        cout << COR_SUCESSO << "\n Conteudo '" << nome << "' cadastrado com sucesso!" << COR_RESET << "\n";
}

//  Buscar conteudo por nome
Conteudo* ListaSimples::buscar(string nome) {
    Conteudo* atual = inicio;
    while (atual != nullptr) {
        if (atual->nome == nome)
            return atual;
        atual = atual->prox;
    }
    return nullptr;
}

//  Remover conteudo por nome
bool ListaSimples::remover(string nome) {
    if (inicio == nullptr) {
        cout << COR_ERRO << "\n Lista vazia!" << COR_RESET << "\n";
        return false;
    }

    Conteudo* ret;

    // Se e o primeiro elemento
    if (inicio->nome == nome) {
        ret = inicio;
        inicio = inicio->prox;
        delete ret;
        cout << COR_SUCESSO << "\n Conteudo '" << nome << "' removido!" << COR_RESET << "\n";
        return true;
    }

    // Percorre a lista
    Conteudo* atual = inicio;
    while (atual->prox != nullptr && atual->prox->nome != nome) {
        atual = atual->prox;
    }

    if (atual->prox == nullptr) {
        cout << COR_ERRO << "\n Conteudo '" << nome << "' nao encontrado!" << COR_RESET << "\n";
        return false;
    }

    ret = atual->prox;
    atual->prox = ret->prox;
    delete ret;
    cout << COR_SUCESSO << "\n Conteudo '" << nome << "' removido!" << COR_RESET << "\n";
    return true;
}

//  Remover conteudo por indice/posicao (1-based)
bool ListaSimples::removerPorIndice(int pos) {
    if (inicio == nullptr) {
        cout << COR_ERRO << "\n Lista vazia!" << COR_RESET << "\n";
        return false;
    }

    if (pos <= 0) {
        cout << COR_ERRO << "\n Indice invalido!" << COR_RESET << "\n";
        return false;
    }

    Conteudo* ret;

    if (pos == 1) {
        ret = inicio;
        inicio = inicio->prox;
        cout << COR_SUCESSO << "\n Conteudo '" << ret->nome << "' removido!" << COR_RESET << "\n";
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
        cout << COR_ERRO << "\n Indice Nao Encontrado (fora dos limites)!" << COR_RESET << "\n";
        return false;
    }

    ret = atual->prox;
    atual->prox = ret->prox;
    cout << COR_SUCESSO << "\n Conteudo '" << ret->nome << "' removido!" << COR_RESET << "\n";
    delete ret;
    return true;
}

//  Listar todos os conteudos
void ListaSimples::listar() {
    if (inicio == nullptr) {
        cout << "\n Nenhum conteudo cadastrado.\n";
        return;
    }

    exibirCabecalho("CATALOGO DE CONTEUDOS");
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

//  Contar total de conteudos
int ListaSimples::contarTotal() {
    int cont = 0;
    Conteudo* atual = inicio;
    while (atual != nullptr) {
        cont++;
        atual = atual->prox;
    }
    return cont;
}


//  Listar filtrado por tipo e/ou genero
void ListaSimples::listarFiltrado(Tipo tipo, Genero genero, FiltroAno filtroAno) {
    Conteudo* atual = inicio;
    int encontrados = 0;
    int index = 1;

    while (atual != nullptr) {
        bool match = true;
        if (tipo != OUTRO_TIPO && atual->tipo != tipo) match = false;
        if (genero != OUTRO_GENERO && atual->genero != genero) match = false;
        
        // Filtra pelo ano
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


void ListaSimples::salvarArquivo(string caminho) {
    ofstream arquivo(caminho);
    if (!arquivo.is_open()) {
        cout << COR_ERRO << "\n ERRO: Nao foi possivel salvar o arquivo '" << caminho << "'!" << COR_RESET << "\n";
        return;
    }

    Conteudo* atual = inicio;
    while (atual != nullptr) {
        arquivo << atual->nome << ";"
                << atual->tipo << ";"
                << atual->genero << ";"
                << atual->anoLancamento << ";"
                << atual->visualizacoes << ";"
                << atual->avaliacao << ";"
                << atual->totalAvaliacoes << "\n";
        atual = atual->prox;
    }

    arquivo.close();
}

//  Se o arquivo nao existir, simplesmente nao carrega nada
void ListaSimples::carregarArquivo(string caminho) {
    ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        return; 
    }

    string linha;
    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        stringstream ss(linha);
        string nome, campo;
        int tipo, genero, ano, viz, totalAval;
        float avaliacao;

        getline(ss, nome, ';');
        getline(ss, campo, ';'); tipo = stoi(campo);
        getline(ss, campo, ';'); genero = stoi(campo);
        getline(ss, campo, ';'); ano = stoi(campo);
        getline(ss, campo, ';'); viz = stoi(campo);
        getline(ss, campo, ';'); avaliacao = stof(campo);
        getline(ss, campo, ';'); totalAval = stoi(campo);

        inserirOrdenado(nome, (Tipo)tipo, (Genero)genero, ano, viz, true);

        Conteudo* c = buscar(nome);
        if (c != nullptr) {
            c->avaliacao = avaliacao;
            c->totalAvaliacoes = totalAval;
        }
    }

    arquivo.close();
}