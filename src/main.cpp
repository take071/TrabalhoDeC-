#include "arvore_decisao.h"
#include "lista_simples.h"
#include "lista_dupla.h"
#include "estatisticas.h"
#include <ctype.h>
#include <limits>
#include <cstdlib>

// =============================================
//  Funcao utilitaria para limpar a tela
// =============================================
void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// =============================================
//  Funcao para cadastrar conteudos iniciais
//  (dados de exemplo para demonstracao)
// =============================================
void carregarDadosIniciais(ListaSimples &catalogo) {
    // Filmes de Acao
    catalogo.inserirOrdenado("Os Vingadores", FILME, ACAO, 2012, 0);
    catalogo.inserirOrdenado("Mad Max Estrada da Furia", FILME, ACAO, 2015, 0);
    catalogo.inserirOrdenado("Duro de Matar", FILME, ACAO, 1988, 0);
    catalogo.inserirOrdenado("John Wick 4", FILME, ACAO, 2023, 0);

    // Filmes de Drama
    catalogo.inserirOrdenado("A Lista de Schindler", FILME, DRAMA, 1993, 0);
    catalogo.inserirOrdenado("Parasita", FILME, DRAMA, 2019, 0);
    catalogo.inserirOrdenado("Forrest Gump", FILME, DRAMA, 1994, 0);

    // Filmes de Terror
    catalogo.inserirOrdenado("O Iluminado", FILME, TERROR, 1980, 0);
    catalogo.inserirOrdenado("Hereditario", FILME, TERROR, 2018, 0);

    // Series
    catalogo.inserirOrdenado("Breaking Bad", SERIE, DRAMA, 2008, 0);
    catalogo.inserirOrdenado("The Office", SERIE, COMEDIA, 2005, 0);
    catalogo.inserirOrdenado("Stranger Things", SERIE, FICCAO, 2016, 0);
    catalogo.inserirOrdenado("Black Mirror", SERIE, FICCAO, 2011, 0);
    catalogo.inserirOrdenado("Friends", SERIE, COMEDIA, 1994, 0);

    // Animes
    catalogo.inserirOrdenado("Naruto", ANIME, ACAO, 2002, 0);
    catalogo.inserirOrdenado("Attack on Titan", ANIME, ACAO, 2013, 0);
    catalogo.inserirOrdenado("Death Note", ANIME, DRAMA, 2006, 0);
    catalogo.inserirOrdenado("One Piece", ANIME, ACAO, 1999, 0);

    // Documentarios
    catalogo.inserirOrdenado("Cosmos", DOCUMENTARIO, FICCAO, 2014, 0);
    catalogo.inserirOrdenado("O Dilema das Redes", DOCUMENTARIO, OUTRO_GENERO, 2020, 0);

    cout << "\n [!] " << catalogo.contarTotal() << " conteudos carregados!\n";
}

// =============================================
//  Processo de recomendacao completo
// =============================================
void processarRecomendacao(ArvoreDecisao &arvore, ListaSimples &catalogo,
                           ListaDupla &historico, Estatisticas &stats) {
    limparTela();
    Tipo tipoResult = OUTRO_TIPO;
    Genero generoResult = OUTRO_GENERO;
    FiltroAno filtroAnoResult = QUALQUER_ANO;

    cout << "\n===== PROCESSO DE RECOMENDACAO =====\n";
    cout << " Responda as perguntas para receber recomendacoes!\n";

    // Navega pela arvore de decisao
    arvore.navegar(arvore.raiz, tipoResult, generoResult, filtroAnoResult);

    // Registra estatistica
    stats.registrarRecomendacao(tipoResult, generoResult);

    // Exibe conteudos filtrados
    cout << "\n--- Conteudos recomendados para voce ---";
    catalogo.listarFiltrado(tipoResult, generoResult, filtroAnoResult);

    // Permite selecionar um conteudo pelo indice mostrado (neste caso, a listarFiltrado usa enumeracao virtual entao
    // o usuario digitaria o nome ou indice global. Como listarFiltrado apenas imprime "1, 2...", 
    // pedir o indice real do catalogo e dificil. Entao vamos refazer a busca do indice filtrado, ou
    // melhor: pedir o ID exato da lista global ou titulo).
    // Como implementamos o numero global na nova versao de listarFiltrado, vamos usa-lo.
    
    cout << "\n\n Deseja selecionar um conteudo para assistir? (s/n): ";
    char resp;
    cin >> resp;

    if (resp == 's' || resp == 'S') {
        int indice;
        cout << " Digite o número do conteudo (ID mostrado acima): ";
        cin >> indice;

        // Limpa estado de erro caso o usuario digite string em vez de int
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n Numero invalido!\n";
            return;
        }

        // Buscar por indice
        Conteudo* c = catalogo.inicio;
        int idx = 1;
        while (c != nullptr && idx < indice) {
            c = c->prox;
            idx++;
        }

        if (c != nullptr) {
            c->visualizacoes++;
            historico.inserirOuAtualizar(c->nome, c->tipo, c->genero);
            cout << "\n Voce esta assistindo: " << c->nome << "!";
            cout << "\n Visualizacoes totais: " << c->visualizacoes << "\n";
        } else {
            cout << "\n Conteudo nao encontrado para esse ID!\n";
        }
    }
}


// =============================================
//  Cadastrar novo conteudo manualmente
// =============================================
void cadastrarConteudo(ListaSimples &catalogo) {
    limparTela();
    string nome;
    int ano;

    cout << "\n===== CADASTRAR CONTEUDO =====\n";
    cin.ignore();
    cout << "\n Nome do titulo: ";
    getline(cin, nome);

    Tipo tipo = lerTipo();
    Genero genero = lerGenero();

    cout << "\n Ano de lancamento: ";
    cin >> ano;

    catalogo.inserirOrdenado(nome, tipo, genero, ano, 0);
}

// =============================================
//  Remover conteudo por posicao
// =============================================
void removerConteudo(ListaSimples &catalogo) {
    limparTela();
    cout << "\n===== REMOVER CONTEUDO =====\n";
    catalogo.listar();

    int pos;
    cout << "\n Qual o numero do conteudo que deseja remover? ";
    cin >> pos;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n Indice invalido!\n";
        return;
    }

    catalogo.removerPorIndice(pos);
}

// =============================================
//  MAIN - Menu principal
//  Segue o padrao de loop do ArvoreB.cpp
// =============================================
int main() {
    // Instancias das estruturas de dados
    ArvoreDecisao arvore;
    ListaSimples catalogo;
    ListaDupla historico;
    Estatisticas stats;

    // Carrega dados de demonstracao
    carregarDadosIniciais(catalogo);

    int op;

    for (;;) {
        limparTela();
        cout << "\n==========================================";
        cout << "\n   SISTEMA DE RECOMENDACAO DE STREAMING";
        cout << "\n==========================================";
        cout << "\n";
        cout << "\n  1. Cadastrar conteudo";
        cout << "\n  2. Remover conteudo";
        cout << "\n  3. Listar conteudos cadastrados";
        cout << "\n  4. Obter recomendacao (navegar arvore)";
        cout << "\n  5. Ver historico de mais assistidos";
        cout << "\n  6. Avaliar conteudo (estrelas)";
        cout << "\n  7. Ranking por avaliacao";
        cout << "\n  8. Ver estatisticas completas";
        cout << "\n  9. Visualizar arvore de decisao";
        cout << "\n  0. Sair";
        cout << "\n\n  Opcao: ";
        cin >> op;

        // Limpa entrada invalida
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n Opcao invalida!\n";
            continue;
        }

        switch (op) {
            case 1:
                cadastrarConteudo(catalogo);
                break;

            case 2:
                removerConteudo(catalogo);
                break;

            case 3:
                limparTela();
                catalogo.listar();
                break;

            case 4:
                processarRecomendacao(arvore, catalogo, historico, stats);
                break;

            case 5:
                limparTela();
                historico.listar();
                break;

            case 6:
                limparTela();
                stats.avaliarConteudo(catalogo);
                break;

            case 7:
                limparTela();
                stats.listarPorAvaliacao(catalogo);
                break;

            case 8:
                limparTela();
                stats.exibirEstatisticas(catalogo, historico);
                break;

            case 9:
                limparTela();
                arvore.imprimirArvore();
                break;

            case 0:
                cout << "\n Saindo do sistema...\n";
                return 0;

            default:
                cout << "\n Opcao invalida!\n";
        }

        cout << "\n Pressione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    return 0;
}
