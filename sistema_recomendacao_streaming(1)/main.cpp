#include "arvore_decisao.h"
#include "lista_simples.h"
#include "lista_dupla.h"
#include "estatisticas.h"
#include <ctype.h>
#include <limits>
#include <cstdlib>

//  Arquivos de persistencia (Melhoria: dados
//  sobrevivem ao fechar o programa)
const string ARQ_CATALOGO  = "catalogo.txt";
const string ARQ_HISTORICO = "historico.txt";

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

//  Funcao para cadastrar conteudos iniciais
//  (dados de exemplo para demonstracao)
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

    cout << COR_SUCESSO << "\n [!] " << catalogo.contarTotal() << " conteudos carregados!" << COR_RESET << "\n";
}

void processarRecomendacao(ArvoreDecisao &arvore, ListaSimples &catalogo,
                           ListaDupla &historico, Estatisticas &stats) {
    limparTela();
    Tipo tipoResult = OUTRO_TIPO;
    Genero generoResult = OUTRO_GENERO;
    FiltroAno filtroAnoResult = QUALQUER_ANO;

    exibirCabecalho("PROCESSO DE RECOMENDACAO");
    cout << " Responda as perguntas para receber recomendacoes!\n";

    // Navega pela arvore de decisao
    arvore.navegar(arvore.raiz, tipoResult, generoResult, filtroAnoResult);

    // Registra estatistica
    stats.registrarRecomendacao(tipoResult, generoResult);

    // Exibe conteudos filtrados
    cout << "\n--- Conteudos recomendados para voce ---";
    catalogo.listarFiltrado(tipoResult, generoResult, filtroAnoResult);

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
            cout << COR_ERRO << "\n Numero invalido!" << COR_RESET << "\n";
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
            cout << COR_SUCESSO << "\n Voce esta assistindo: " << c->nome << "!" << COR_RESET;
            cout << "\n Visualizacoes totais: " << c->visualizacoes << "\n";
        } else {
            cout << COR_ERRO << "\n Conteudo nao encontrado para esse ID!" << COR_RESET << "\n";
        }
    }
}

//  Cadastrar novo conteudo manualmente
void cadastrarConteudo(ListaSimples &catalogo) {
    limparTela();
    string nome;
    int ano;

    exibirCabecalho("CADASTRAR CONTEUDO");
    cin.ignore();
    cout << "\n Nome do titulo: ";
    getline(cin, nome);

    Tipo tipo = lerTipo();
    Genero genero = lerGenero();

    cout << "\n Ano de lancamento: ";
    cin >> ano;

    catalogo.inserirOrdenado(nome, tipo, genero, ano, 0);
}


void removerConteudo(ListaSimples &catalogo) {
    limparTela();
    exibirCabecalho("REMOVER CONTEUDO");
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

int main() {
    
    ArvoreDecisao arvore;
    ListaSimples catalogo;
    ListaDupla historico;
    Estatisticas stats;

    catalogo.carregarArquivo(ARQ_CATALOGO);
    historico.carregarArquivo(ARQ_HISTORICO);

    if (catalogo.contarTotal() == 0) {
        carregarDadosIniciais(catalogo);
    } else {
        cout << COR_SUCESSO << "\n [!] Dados carregados do arquivo salvo ("
             << catalogo.contarTotal() << " conteudos)." << COR_RESET << "\n";
    }

    int op;

    for (;;) {
        limparTela();
        exibirBanner();
        cout << COR_TITULO;
        imprimirCentralizado("SISTEMA DE RECOMENDACAO DE STREAMING");
        cout << COR_RESET << "\n";

        imprimirCentralizado("1. Cadastrar conteudo");
        imprimirCentralizado("2. Remover conteudo");
        imprimirCentralizado("3. Listar conteudos cadastrados");
        imprimirCentralizado("4. Obter recomendacao (navegar arvore)");
        imprimirCentralizado("5. Ver historico de mais assistidos");
        imprimirCentralizado("6. Avaliar conteudo (estrelas)");
        imprimirCentralizado("7. Ranking por avaliacao");
        imprimirCentralizado("8. Ver estatisticas completas");
        imprimirCentralizado("9. Visualizar arvore de decisao");
        imprimirCentralizado("0. Sair");
        cout << "\n\n" << centralizar("Opcao: ");
        cin >> op;

        // Limpa entrada invalida
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << COR_ERRO << "\n Opcao invalida!" << COR_RESET << "\n";
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
                // Persistencia (Melhoria): salva catalogo e historico em
                // disco antes de encerrar, para recuperar na proxima execucao
                catalogo.salvarArquivo(ARQ_CATALOGO);
                historico.salvarArquivo(ARQ_HISTORICO);
                cout << COR_SUCESSO << "\n Dados salvos com sucesso!" << COR_RESET;
                cout << "\n Saindo do sistema...\n";
                return 0;

            default:
                cout << COR_ERRO << "\n Opcao invalida!" << COR_RESET << "\n";
        }

        cout << "\n Pressione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    return 0;
}