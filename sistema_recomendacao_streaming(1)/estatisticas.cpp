#include "estatisticas.h"

Estatisticas::Estatisticas() {
    totalRecomendacoes = 0;
    for (int i = 0; i < 5; i++) contRecomTipo[i] = 0;
    for (int i = 0; i < 6; i++) contRecomGenero[i] = 0;
}

void Estatisticas::registrarRecomendacao(Tipo tipo, Genero genero) {
    totalRecomendacoes++;
    if (tipo >= 0 && tipo < 5) contRecomTipo[tipo]++;
    if (genero >= 0 && genero < 6) contRecomGenero[genero]++;
}

void Estatisticas::exibirEstatisticas(ListaSimples &catalogo, ListaDupla &historico) {
    exibirCabecalho("ESTATISTICAS DO SISTEMA");

    cout << "\n Total de recomendacoes realizadas: " << totalRecomendacoes;

    int totalViz = 0;
    Conteudo* atual = catalogo.inicio;
    while (atual != nullptr) {
        totalViz += atual->visualizacoes;
        atual = atual->prox;
    }
    cout << "\n Total de visualizacoes: " << totalViz;

    // --- Tipo mais e menos recomendado ---
    if (totalRecomendacoes > 0) {
        int maxTipo = 0, minTipo = 0;
        for (int i = 1; i < 5; i++) {
            if (contRecomTipo[i] > contRecomTipo[maxTipo]) maxTipo = i;
            if (contRecomTipo[i] < contRecomTipo[minTipo]) minTipo = i;
        }
        cout << "\n\n Tipo mais recomendado:  " << tipoParaString((Tipo)maxTipo)
             << " (" << contRecomTipo[maxTipo] << " vezes)";
        cout << "\n Tipo menos recomendado: " << tipoParaString((Tipo)minTipo)
             << " (" << contRecomTipo[minTipo] << " vezes)";
    }

    // --- Genero mais e menos recomendado ---
    if (totalRecomendacoes > 0) {
        int maxGen = 0, minGen = 0;
        for (int i = 1; i < 6; i++) {
            if (contRecomGenero[i] > contRecomGenero[maxGen]) maxGen = i;
            if (contRecomGenero[i] < contRecomGenero[minGen]) minGen = i;
        }
        cout << "\n\n Genero mais recomendado:  " << generoParaString((Genero)maxGen)
             << " (" << contRecomGenero[maxGen] << " vezes)";
        cout << "\n Genero menos recomendado: " << generoParaString((Genero)minGen)
             << " (" << contRecomGenero[minGen] << " vezes)";
    }

    // --- Titulo mais assistido por tipo ---
    cout << "\n\n --- Mais assistido por tipo ---";
    for (int i = 0; i < 5; i++) {
        ConteudoHistorico* h = historico.getMaisAssistidoPorTipo((Tipo)i);
        if (h != nullptr) {
            cout << "\n  " << tipoParaString((Tipo)i) << ": "
                 << h->nome << " (" << h->vezesAssistido << " viz)";
        }
    }

    cout << "\n\n --- Mais assistido por genero ---";
    for (int i = 0; i < 6; i++) {
        ConteudoHistorico* h = historico.getMaisAssistidoPorGenero((Genero)i);
        if (h != nullptr) {
            cout << "\n  " << generoParaString((Genero)i) << ": "
                 << h->nome << " (" << h->vezesAssistido << " viz)";
        }
    }

    cout << "\n\n --- Titulos nunca selecionados ---";
    atual = catalogo.inicio;
    int nuncaVisto = 0;
    while (atual != nullptr) {
        if (atual->visualizacoes == 0) {
            cout << "\n  - " << atual->nome;
            nuncaVisto++;
        }
        atual = atual->prox;
    }
    if (nuncaVisto == 0)
        cout << "\n  Todos os titulos ja foram selecionados!";

    cout << COR_TITULO << "\n\n" << centralizar(string(40, '=')) << COR_RESET << "\n";
}

//  Avaliar conteudo com estrelas (1-5)
//  Funcionalidade Extra
void Estatisticas::avaliarConteudo(ListaSimples &catalogo) {
    string nome;
    int estrelas;

    exibirCabecalho("AVALIAR CONTEUDO");
    cin.ignore();
    cout << "\n Nome do conteudo: ";
    getline(cin, nome);

    Conteudo* c = catalogo.buscar(nome);
    if (c == nullptr) {
        cout << COR_ERRO << "\n Conteudo nao encontrado!" << COR_RESET << "\n";
        return;
    }

    cout << "\n Conteudo: " << c->nome;
    cout << "\n Tipo: " << tipoParaString(c->tipo);
    cout << " | Genero: " << generoParaString(c->genero);

    if (c->totalAvaliacoes > 0) {
        cout << "\n Avaliacao atual:";
        exibirEstrelas(c->avaliacao);
        cout << " (" << c->totalAvaliacoes << " avaliacoes)";
    }

    cout << "\n\n Sua avaliacao (1-5 estrelas): ";
    cin >> estrelas;

    if (estrelas < 1 || estrelas > 5) {
        cout << COR_ERRO << "\n Avaliacao invalida! Use valores de 1 a 5." << COR_RESET << "\n";
        return;
    }

    // Calcula nova media ponderada
    float somaAnterior = c->avaliacao * c->totalAvaliacoes;
    c->totalAvaliacoes++;
    c->avaliacao = (somaAnterior + estrelas) / c->totalAvaliacoes;

    cout << COR_SUCESSO << "\n Avaliacao registrada! Nova media:" << COR_RESET;
    exibirEstrelas(c->avaliacao);
    cout << "\n";
}
void Estatisticas::listarPorAvaliacao(ListaSimples &catalogo) {
    if (catalogo.inicio == nullptr) {
        cout << "\n Nenhum conteudo cadastrado.\n";
        return;
    }

    // Conta quantos tem avaliacao
    int total = 0;
    Conteudo* atual = catalogo.inicio;
    while (atual != nullptr) {
        if (atual->totalAvaliacoes > 0) total++;
        atual = atual->prox;
    }

    if (total == 0) {
        cout << "\n Nenhum conteudo foi avaliado ainda.\n";
        return;
    }

    exibirCabecalho("RANKING POR AVALIACAO");

    // Selection sort simples para exibir em ordem decrescente
    bool* exibido = new bool[total]();
    Conteudo** avaliados = new Conteudo*[total];

    // Coleta avaliados
    int idx = 0;
    atual = catalogo.inicio;
    while (atual != nullptr) {
        if (atual->totalAvaliacoes > 0) {
            avaliados[idx] = atual;
            exibido[idx] = false;
            idx++;
        }
        atual = atual->prox;
    }

    // Imprime em ordem decrescente de avaliacao
    for (int i = 0; i < total; i++) {
        int melhor = -1;
        for (int j = 0; j < total; j++) {
            if (!exibido[j]) {
                if (melhor == -1 || avaliados[j]->avaliacao > avaliados[melhor]->avaliacao)
                    melhor = j;
            }
        }
        exibido[melhor] = true;
        cout << "\n " << (i + 1) << ". " << avaliados[melhor]->nome;
        cout << " -";
        exibirEstrelas(avaliados[melhor]->avaliacao);
        cout << " (" << avaliados[melhor]->totalAvaliacoes << " avaliacoes)";
    }

    delete[] exibido;
    delete[] avaliados;
    cout << "\n";
}