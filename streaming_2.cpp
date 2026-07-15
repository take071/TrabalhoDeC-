#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

// No da lista de conteudos cadastrados (catalogo)
struct Conteudo {
    string titulo;
    string tipo;       // Filme, Serie, Documentario, Anime, Outro
    string genero;      // Acao, Comedia, Drama, Terror, FiccaoCientifica, Outro
    int ano;
    int visualizacoes;
    int somaEstrelas;   // soma de todas as notas dadas
    int qtdAvaliacoes;  // quantas vezes foi avaliado
    string descricao;
    Conteudo* prox;
};

// No da lista dupla do historico de mais assistidos
struct NoHistorico {
    string titulo;
    string tipo;
    string genero;
    int vezesAssistido;
    NoHistorico* ant;
    NoHistorico* prox;
};

// No temporario usado para montar a lista de recomendacoes
struct NoRecomendado {
    Conteudo* conteudo;
    NoRecomendado* prox;
};

// No da arvore de decisao (perguntas S/N)
struct NoArvore {
    string pergunta;
    int nivel;
    bool folha;
    string sugestaoFinal;
    NoArvore* filhoSim;
    NoArvore* filhoNao;
};

// Preferencias coletadas ao navegar pela arvore
struct Preferencias {
    string tipo;
    string genero;
    string faixaEtaria;
    string duracao;
    string popularidade;
    string recencia;
};

// ---------- Variaveis globais ----------

Conteudo* catalogo = nullptr;
NoHistorico* historicoInicio = nullptr;
NoArvore* raiz = nullptr;

long totalRecomendacoes = 0;
int recTipoFilme = 0, recTipoSerie = 0;
int recGeneroAcao = 0, recGeneroComedia = 0;

const string ARQ_CATALOGO = "catalogo.txt";
const string ARQ_HISTORICO = "historico.txt";
const string ARQ_STATS = "estatisticas.txt";

const string LOGIN_ADMIN = "admin";
const string SENHA_ADMIN = "admin123";

// Uma pergunta por nivel da arvore (6 niveis = 6 criterios)
const string PERGUNTAS[6] = {
    "Voce prefere Series? (se responder N, sera considerado Filme)",
    "Voce gosta de conteudo de Acao? (se responder N, sera considerado Comedia)",
    "Voce prefere conteudo livre para todas as idades? (se N, considerado 16+)",
    "Voce prefere conteudo de duracao curta (episodios/filmes curtos)?",
    "Voce prefere os titulos mais populares (mais assistidos da plataforma)?",
    "Voce prefere lancamentos recentes (ultimos anos)? (se N, classicos)"
};

// ---------- Entrada de dados ----------

string lerLinha(const string& mensagem) {
    string valor;
    cout << mensagem;
    getline(cin, valor);
    return valor;
}

int lerInt(const string& mensagem) {
    string linha;
    int valor;
    while (true) {
        cout << mensagem;
        getline(cin, linha);
        stringstream ss(linha);
        if (ss >> valor) return valor;
        cout << "Entrada invalida. Digite um numero.\n";
    }
}

char lerSimNao(const string& mensagem) {
    string linha;
    while (true) {
        cout << mensagem;
        getline(cin, linha);
        if (!linha.empty()) {
            char c = linha[0];
            if (c == 'S' || c == 's') return 'S';
            if (c == 'N' || c == 'n') return 'N';
        }
        cout << "Responda apenas com S ou N.\n";
    }
}

double mediaEstrelas(Conteudo* c) {
    if (c->qtdAvaliacoes == 0) return 0.0;
    return (double) c->somaEstrelas / c->qtdAvaliacoes;
}

void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ---------- Arvore binaria de decisao ----------

// Monta a arvore recursivamente: 6 niveis de pergunta + 1 nivel de folha
NoArvore* construirArvore(int nivel) {
    NoArvore* no = new NoArvore();
    no->nivel = nivel;
    if (nivel > 6) {
        no->folha = true;
        no->sugestaoFinal = "No folha alcancado! Gerando sua lista de conteudos recomendados com base nas respostas...";
        no->filhoSim = nullptr;
        no->filhoNao = nullptr;
        return no;
    }
    no->folha = false;
    no->pergunta = PERGUNTAS[nivel - 1];
    no->filhoSim = construirArvore(nivel + 1);
    no->filhoNao = construirArvore(nivel + 1);
    return no;
}

void liberarArvore(NoArvore* no) {
    if (!no) return;
    liberarArvore(no->filhoSim);
    liberarArvore(no->filhoNao);
    delete no;
}

// Percorre a arvore perguntando ao usuario e monta as preferencias dele
Preferencias navegarArvore() {
    Preferencias p;
    NoArvore* atual = raiz;
    cout << "\n--- Vamos descobrir sua recomendacao! Responda as perguntas (S/N) ---\n";
    while (!atual->folha) {
        char resp = lerSimNao(atual->pergunta + " (S/N): ");
        bool sim = (resp == 'S');
        switch (atual->nivel) {
            case 1: p.tipo = sim ? "Serie" : "Filme"; break;
            case 2: p.genero = sim ? "Acao" : "Comedia"; break;
            case 3: p.faixaEtaria = sim ? "Livre" : "16+"; break;
            case 4: p.duracao = sim ? "Curta" : "Longa"; break;
            case 5: p.popularidade = sim ? "Popular" : "Nicho"; break;
            case 6: p.recencia = sim ? "Recente" : "Classico"; break;
        }
        atual = sim ? atual->filhoSim : atual->filhoNao;
    }

    cout << "\n" << atual->sugestaoFinal << "\n";

    totalRecomendacoes++;
    if (p.tipo == "Filme") recTipoFilme++; else recTipoSerie++;
    if (p.genero == "Acao") recGeneroAcao++; else recGeneroComedia++;

    cout << "\nResumo das suas preferencias:\n";
    cout << " Tipo: " << p.tipo << " | Genero: " << p.genero
         << " | Faixa etaria: " << p.faixaEtaria << " | Duracao: " << p.duracao
         << " | Popularidade: " << p.popularidade << " | Epoca: " << p.recencia << "\n";
    return p;
}

// ---------- Catalogo de conteudos (lista simples) ----------

void inserirConteudo(string titulo, string tipo, string genero, int ano,
                      int visualizacoesIniciais = 0, int somaEstrelas = 0, int qtdAvaliacoes = 0,
                      string descricao = "") {
    Conteudo* novo = new Conteudo();
    novo->titulo = titulo;
    novo->tipo = tipo;
    novo->genero = genero;
    novo->ano = ano;
    novo->visualizacoes = visualizacoesIniciais;
    novo->somaEstrelas = somaEstrelas;
    novo->qtdAvaliacoes = qtdAvaliacoes;
    novo->descricao = descricao;
    novo->prox = catalogo;
    catalogo = novo;
}

void cadastrarConteudoInterativo() {
    cout << "\n--- Cadastro de novo conteudo ---\n";
    string titulo = lerLinha("Titulo: ");
    cout << "Tipo (Filme, Serie, Documentario, Anime, Outro): ";
    string tipo; getline(cin, tipo);
    cout << "Genero (Acao, Comedia, Drama, Terror, FiccaoCientifica, Outro): ";
    string genero; getline(cin, genero);
    int ano = lerInt("Ano de lancamento: ");
    string descricao = lerLinha("Descricao/sinopse do conteudo: ");
    inserirConteudo(titulo, tipo, genero, ano, 0, 0, 0, descricao);
    cout << "Conteudo cadastrado com sucesso!\n";
}

bool removerConteudo(const string& titulo) {
    Conteudo* atual = catalogo;
    Conteudo* anterior = nullptr;
    while (atual) {
        if (atual->titulo == titulo) {
            if (anterior) anterior->prox = atual->prox;
            else catalogo = atual->prox;
            delete atual;
            return true;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return false;
}

void removerConteudoInterativo() {
    string titulo = lerLinha("\nDigite o titulo do conteudo a remover: ");
    if (removerConteudo(titulo)) cout << "Removido com sucesso.\n";
    else cout << "Conteudo nao encontrado.\n";
}

void listarConteudos() {
    cout << "\n--- Conteudos cadastrados ---\n";
    if (!catalogo) { cout << "Nenhum conteudo cadastrado.\n"; return; }
    Conteudo* atual = catalogo;
    int i = 1;
    while (atual) {
        cout << i << ". " << atual->titulo << " | " << atual->tipo << " | " << atual->genero
             << " | Ano: " << atual->ano << " | Visualizacoes: " << atual->visualizacoes
             << " | Nota media: ";
        if (atual->qtdAvaliacoes > 0) cout << mediaEstrelas(atual) << " (" << atual->qtdAvaliacoes << " avaliacoes)";
        else cout << "sem avaliacoes";
        cout << "\n";
        cout << "   Descricao: " << (atual->descricao.empty() ? "(sem descricao cadastrada)" : atual->descricao) << "\n";
        atual = atual->prox;
        i++;
    }
}

Conteudo* buscarConteudoPorTitulo(const string& titulo) {
    Conteudo* atual = catalogo;
    while (atual) {
        if (atual->titulo == titulo) return atual;
        atual = atual->prox;
    }
    return nullptr;
}

// ---------- Lista simples ordenada de recomendacoes (temporaria) ----------

// criterio: 1 = nome, 2 = mais visto, 3 = melhor avaliado
void inserirOrdenadoRecomendacao(NoRecomendado*& inicio, Conteudo* c, int criterio) {
    NoRecomendado* novo = new NoRecomendado();
    novo->conteudo = c;
    novo->prox = nullptr;

    if (!inicio) { inicio = novo; return; }

    bool antesDoInicio;
    if (criterio == 1) antesDoInicio = (c->titulo < inicio->conteudo->titulo);
    else if (criterio == 2) antesDoInicio = (c->visualizacoes > inicio->conteudo->visualizacoes);
    else antesDoInicio = (mediaEstrelas(c) > mediaEstrelas(inicio->conteudo));

    if (antesDoInicio) {
        novo->prox = inicio;
        inicio = novo;
        return;
    }

    NoRecomendado* atual = inicio;
    while (atual->prox) {
        bool antes;
        if (criterio == 1) antes = (c->titulo < atual->prox->conteudo->titulo);
        else if (criterio == 2) antes = (c->visualizacoes > atual->prox->conteudo->visualizacoes);
        else antes = (mediaEstrelas(c) > mediaEstrelas(atual->prox->conteudo));
        if (antes) break;
        atual = atual->prox;
    }
    novo->prox = atual->prox;
    atual->prox = novo;
}

void liberarListaRecomendacoes(NoRecomendado*& inicio) {
    while (inicio) {
        NoRecomendado* tmp = inicio;
        inicio = inicio->prox;
        delete tmp;
    }
}

// Filtra o catalogo pelas preferencias; se nao achar nada, vai relaxando o filtro
NoRecomendado* gerarListaRecomendacoes(const Preferencias& p, int criterio) {
    NoRecomendado* lista = nullptr;

    Conteudo* c = catalogo;
    while (c) {
        if (c->tipo == p.tipo && c->genero == p.genero)
            inserirOrdenadoRecomendacao(lista, c, criterio);
        c = c->prox;
    }

    if (!lista) {
        c = catalogo;
        while (c) {
            if (c->tipo == p.tipo) inserirOrdenadoRecomendacao(lista, c, criterio);
            c = c->prox;
        }
    }

    if (!lista) {
        c = catalogo;
        while (c) {
            inserirOrdenadoRecomendacao(lista, c, criterio);
            c = c->prox;
        }
    }

    return lista;
}

// ---------- Lista dupla ordenada: historico de mais assistidos ----------

void desconectarHistorico(NoHistorico* no) {
    if (no->ant) no->ant->prox = no->prox;
    else historicoInicio = no->prox;
    if (no->prox) no->prox->ant = no->ant;
    no->ant = nullptr;
    no->prox = nullptr;
}

void inserirHistoricoOrdenado(NoHistorico* no) {
    if (!historicoInicio || no->vezesAssistido > historicoInicio->vezesAssistido) {
        no->prox = historicoInicio;
        no->ant = nullptr;
        if (historicoInicio) historicoInicio->ant = no;
        historicoInicio = no;
        return;
    }
    NoHistorico* atual = historicoInicio;
    while (atual->prox && atual->prox->vezesAssistido >= no->vezesAssistido)
        atual = atual->prox;
    no->prox = atual->prox;
    if (atual->prox) atual->prox->ant = no;
    no->ant = atual;
    atual->prox = no;
}

// Atualiza (ou cria) a entrada do conteudo no historico, mantendo a ordem
void atualizarHistorico(Conteudo* c) {
    NoHistorico* atual = historicoInicio;
    NoHistorico* achado = nullptr;
    while (atual) {
        if (atual->titulo == c->titulo) { achado = atual; break; }
        atual = atual->prox;
    }
    if (achado) {
        achado->vezesAssistido = c->visualizacoes;
        desconectarHistorico(achado);
        inserirHistoricoOrdenado(achado);
    } else {
        NoHistorico* novo = new NoHistorico();
        novo->titulo = c->titulo;
        novo->tipo = c->tipo;
        novo->genero = c->genero;
        novo->vezesAssistido = c->visualizacoes;
        novo->ant = nullptr;
        novo->prox = nullptr;
        inserirHistoricoOrdenado(novo);
    }
}

void exibirHistorico() {
    cout << "\n--- Historico dos conteudos mais assistidos ---\n";
    if (!historicoInicio) { cout << "Nenhum conteudo assistido ainda.\n"; return; }
    NoHistorico* atual = historicoInicio;
    int i = 1;
    while (atual) {
        cout << i << ". " << atual->titulo << " | " << atual->tipo << " | " << atual->genero
             << " | Vezes assistido: " << atual->vezesAssistido << "\n";
        atual = atual->prox;
        i++;
    }
}

void liberarHistorico() {
    while (historicoInicio) {
        NoHistorico* tmp = historicoInicio;
        historicoInicio = historicoInicio->prox;
        delete tmp;
    }
}

// ---------- Processo de recomendacao / selecao ----------

void processoDeRecomendacao() {
    if (!catalogo) {
        cout << "\nNao ha conteudos cadastrados. Cadastre algum conteudo primeiro.\n";
        return;
    }

    Preferencias p = navegarArvore();

    cout << "\nOrdenar recomendacoes por:\n 1 - Nome\n 2 - Popularidade (visualizacoes)\n 3 - Avaliacao (estrelas)\n";
    int criterio = lerInt("Escolha: ");
    if (criterio < 1 || criterio > 3) criterio = 2;

    NoRecomendado* lista = gerarListaRecomendacoes(p, criterio);

    cout << "\n--- Conteudos recomendados para voce ---\n";
    if (!lista) {
        cout << "Nenhum conteudo disponivel no catalogo no momento.\n";
        return;
    }

    NoRecomendado* atual = lista;
    int i = 1;
    while (atual) {
        cout << i << ". " << atual->conteudo->titulo << " | " << atual->conteudo->tipo
             << " | " << atual->conteudo->genero << " | Ano: " << atual->conteudo->ano
             << " | Visualizacoes: " << atual->conteudo->visualizacoes << " | Nota: ";
        if (atual->conteudo->qtdAvaliacoes > 0) cout << mediaEstrelas(atual->conteudo);
        else cout << "sem avaliacoes";
        cout << "\n";
        if (!atual->conteudo->descricao.empty())
            cout << "   Descricao: " << atual->conteudo->descricao << "\n";
        atual = atual->prox;
        i++;
    }

    int escolha = lerInt("\nDigite o numero do conteudo que deseja assistir (0 para cancelar): ");
    if (escolha > 0) {
        atual = lista;
        int idx = 1;
        while (atual && idx != escolha) { atual = atual->prox; idx++; }
        if (atual) {
            Conteudo* c = atual->conteudo;
            c->visualizacoes++;
            atualizarHistorico(c);

            cout << "\nVoce assistiu \"" << c->titulo << "\"!\n";
            cout << "Dica: use a opcao \"Avaliar conteudo (estrelas)\" no menu principal para dar sua nota.\n";
        } else {
            cout << "Opcao invalida.\n";
        }
    }

    liberarListaRecomendacoes(lista);
}

// ---------- Avaliacao por estrelas (extra do usuario) ----------

// Fluxo separado de "assistir": o usuario escolhe um titulo do catalogo e da a nota
void avaliarConteudoInterativo() {
    if (!catalogo) {
        cout << "\nNao ha conteudos cadastrados para avaliar.\n";
        return;
    }

    listarConteudos();

    string titulo = lerLinha("\nDigite o titulo exato do conteudo que deseja avaliar (0 para cancelar): ");
    if (titulo == "0") return;

    Conteudo* c = buscarConteudoPorTitulo(titulo);
    if (!c) {
        cout << "Conteudo nao encontrado.\n";
        return;
    }

    int estrelas;
    while (true) {
        estrelas = lerInt("Avalie \"" + c->titulo + "\" de 1 a 5 estrelas: ");
        if (estrelas >= 1 && estrelas <= 5) break;
        cout << "Nota invalida, digite um numero de 1 a 5.\n";
    }
    c->somaEstrelas += estrelas;
    c->qtdAvaliacoes++;

    cout << "\nAvaliacao registrada! Nota media atual de \"" << c->titulo << "\": "
         << mediaEstrelas(c) << " (" << c->qtdAvaliacoes << " avaliacoes)\n";
}

// ---------- Estatisticas do sistema ----------

void exibirEstatisticas() {
    cout << "\n=========== ESTATISTICAS DO SISTEMA ===========\n";
    cout << "Total de recomendacoes realizadas: " << totalRecomendacoes << "\n";

    long totalVisualizacoes = 0;
    Conteudo* c = catalogo;
    while (c) { totalVisualizacoes += c->visualizacoes; c = c->prox; }
    cout << "Total de visualizacoes: " << totalVisualizacoes << "\n";

    if (totalRecomendacoes > 0) {
        cout << "Tipo mais recomendado: " << (recTipoFilme >= recTipoSerie ? "Filme" : "Serie")
             << " | Tipo menos recomendado: " << (recTipoFilme >= recTipoSerie ? "Serie" : "Filme") << "\n";
        cout << "Genero mais recomendado: " << (recGeneroAcao >= recGeneroComedia ? "Acao" : "Comedia")
             << " | Genero menos recomendado: " << (recGeneroAcao >= recGeneroComedia ? "Comedia" : "Acao") << "\n";
    } else {
        cout << "Ainda nao ha recomendacoes suficientes para apurar tipo/genero mais/menos recomendado.\n";
    }

    Conteudo *maisFilme = nullptr, *maisSerie = nullptr, *maisAcao = nullptr, *maisComedia = nullptr;
    c = catalogo;
    while (c) {
        if (c->tipo == "Filme" && (!maisFilme || c->visualizacoes > maisFilme->visualizacoes)) maisFilme = c;
        if (c->tipo == "Serie" && (!maisSerie || c->visualizacoes > maisSerie->visualizacoes)) maisSerie = c;
        if (c->genero == "Acao" && (!maisAcao || c->visualizacoes > maisAcao->visualizacoes)) maisAcao = c;
        if (c->genero == "Comedia" && (!maisComedia || c->visualizacoes > maisComedia->visualizacoes)) maisComedia = c;
        c = c->prox;
    }
    cout << "Titulo mais assistido (Filme): " << (maisFilme ? maisFilme->titulo : "N/A") << "\n";
    cout << "Titulo mais assistido (Serie): " << (maisSerie ? maisSerie->titulo : "N/A") << "\n";
    cout << "Titulo mais assistido (Genero Acao): " << (maisAcao ? maisAcao->titulo : "N/A") << "\n";
    cout << "Titulo mais assistido (Genero Comedia): " << (maisComedia ? maisComedia->titulo : "N/A") << "\n";

    cout << "Titulos nunca selecionados: ";
    bool algum = false;
    c = catalogo;
    while (c) {
        if (c->visualizacoes == 0) { cout << "\n  - " << c->titulo; algum = true; }
        c = c->prox;
    }
    if (!algum) cout << "Nenhum (todos os titulos ja foram assistidos)";
    cout << "\n=================================================\n";
}

// ---------- Persistencia em arquivos ----------

void salvarCatalogo() {
    ofstream arq(ARQ_CATALOGO.c_str());
    if (!arq.is_open()) { cout << "Erro ao salvar catalogo.\n"; return; }
    Conteudo* c = catalogo;
    while (c) {
        arq << c->titulo << "|" << c->tipo << "|" << c->genero << "|" << c->ano << "|"
            << c->visualizacoes << "|" << c->somaEstrelas << "|" << c->qtdAvaliacoes << "|"
            << c->descricao << "\n";
        c = c->prox;
    }
    arq.close();
}

string proximoCampo(stringstream& ss) {
    string campo;
    getline(ss, campo, '|');
    return campo;
}

void carregarCatalogo() {
    ifstream arq(ARQ_CATALOGO.c_str());
    if (!arq.is_open()) return;
    string linha;
    while (getline(arq, linha)) {
        if (linha.empty()) continue;
        stringstream ss(linha);
        string titulo = proximoCampo(ss);
        string tipo = proximoCampo(ss);
        string genero = proximoCampo(ss);
        string anoStr = proximoCampo(ss);
        string visStr = proximoCampo(ss);
        string somaStr = proximoCampo(ss);
        string qtdStr = proximoCampo(ss);
        string descricao = proximoCampo(ss);
        int ano = anoStr.empty() ? 0 : atoi(anoStr.c_str());
        int vis = visStr.empty() ? 0 : atoi(visStr.c_str());
        int soma = somaStr.empty() ? 0 : atoi(somaStr.c_str());
        int qtd = qtdStr.empty() ? 0 : atoi(qtdStr.c_str());
        inserirConteudo(titulo, tipo, genero, ano, vis, soma, qtd, descricao);
    }
    arq.close();
}

void salvarHistorico() {
    ofstream arq(ARQ_HISTORICO.c_str());
    if (!arq.is_open()) return;
    NoHistorico* h = historicoInicio;
    while (h) {
        arq << h->titulo << "|" << h->tipo << "|" << h->genero << "|" << h->vezesAssistido << "\n";
        h = h->prox;
    }
    arq.close();
}

void carregarHistorico() {
    ifstream arq(ARQ_HISTORICO.c_str());
    if (!arq.is_open()) return;
    string linha;
    while (getline(arq, linha)) {
        if (linha.empty()) continue;
        stringstream ss(linha);
        string titulo = proximoCampo(ss);
        string tipo = proximoCampo(ss);
        string genero = proximoCampo(ss);
        string vStr = proximoCampo(ss);
        int vezes = vStr.empty() ? 0 : atoi(vStr.c_str());
        NoHistorico* novo = new NoHistorico();
        novo->titulo = titulo; novo->tipo = tipo; novo->genero = genero;
        novo->vezesAssistido = vezes; novo->ant = nullptr; novo->prox = nullptr;
        inserirHistoricoOrdenado(novo);
    }
    arq.close();
}

void salvarEstatisticas() {
    ofstream arq(ARQ_STATS.c_str());
    if (!arq.is_open()) return;
    arq << totalRecomendacoes << "|" << recTipoFilme << "|" << recTipoSerie << "|"
        << recGeneroAcao << "|" << recGeneroComedia << "\n";
    arq.close();
}

void carregarEstatisticas() {
    ifstream arq(ARQ_STATS.c_str());
    if (!arq.is_open()) return;
    string linha;
    if (getline(arq, linha)) {
        stringstream ss(linha);
        string a = proximoCampo(ss), b = proximoCampo(ss), c = proximoCampo(ss),
               d = proximoCampo(ss), e = proximoCampo(ss);
        if (!a.empty()) totalRecomendacoes = atol(a.c_str());
        if (!b.empty()) recTipoFilme = atoi(b.c_str());
        if (!c.empty()) recTipoSerie = atoi(c.c_str());
        if (!d.empty()) recGeneroAcao = atoi(d.c_str());
        if (!e.empty()) recGeneroComedia = atoi(e.c_str());
    }
    arq.close();
}

void salvarTudo() {
    salvarCatalogo();
    salvarHistorico();
    salvarEstatisticas();
}

// ---------- Dados iniciais (usados so quando nao ha arquivo salvo) ----------

void inicializarCatalogoPadrao() {
    inserirConteudo("Fúria sem Limites", "Filme", "Acao", 2023, 0, 0, 0,
        "Um ex-agente busca vinganca contra a organizacao que destruiu sua familia.");
    inserirConteudo("Comando Relâmpago", "Filme", "Acao", 2019, 0, 0, 0,
        "Uma equipe de elite precisa impedir um ataque terrorista em 24 horas.");
    inserirConteudo("Guerreiros da Noite", "Serie", "Acao", 2024, 0, 0, 0,
        "Um grupo de guerreiros protege a cidade de ameacas sobrenaturais durante a noite.");
    inserirConteudo("Risada Garantida", "Filme", "Comedia", 2021, 0, 0, 0,
        "Um casal desastrado tenta organizar o casamento perfeito e tudo da errado.");
    inserirConteudo("A Firma dos Amigos", "Serie", "Comedia", 2022, 0, 0, 0,
        "Um grupo de amigos tenta manter um pequeno negocio de pe, gerando confusoes hilarias.");
    inserirConteudo("Vizinhos Bagunceiros", "Serie", "Comedia", 2018, 0, 0, 0,
        "Duas familias vizinhas vivem em constante disputa e confusao no bairro.");
    inserirConteudo("Codinome Vingança", "Filme", "Acao", 2025, 0, 0, 0,
        "Uma espia infiltrada descobre uma conspiracao e precisa escapar viva.");
    inserirConteudo("Rindo Até Cair", "Filme", "Comedia", 2020, 0, 0, 0,
        "Um grupo de amigos embarca em uma viagem cheia de imprevistos e gargalhadas.");
}

// ---------- Area do gerente (login administrativo) ----------

bool autenticarGerente() {
    cout << "\n--- Area do Gerente: login necessario ---\n";
    const int MAX_TENTATIVAS = 3;
    for (int tentativa = 1; tentativa <= MAX_TENTATIVAS; tentativa++) {
        string usuario = lerLinha("Usuario: ");
        string senha = lerLinha("Senha: ");
        if (usuario == LOGIN_ADMIN && senha == SENHA_ADMIN) {
            cout << "Login realizado com sucesso!\n";
            return true;
        }
        cout << "Usuario ou senha invalidos. Tentativa " << tentativa << "/" << MAX_TENTATIVAS << "\n";
    }
    cout << "Numero maximo de tentativas excedido. Voltando ao menu principal.\n";
    return false;
}

void exibirMenuGerente() {
    limparTela();
    cout << "\n===================================================\n";
    cout << "   AREA DO GERENTE\n";
    cout << "===================================================\n";
    cout << "1 - Cadastrar conteudo (com descricao)\n";
    cout << "2 - Remover conteudo\n";
    cout << "3 - Listar conteudos cadastrados\n";
    cout << "0 - Voltar ao menu principal\n";
    cout << "===================================================\n";
}

void areaDoGerente() {
    if (!autenticarGerente()) return;

    int opcao;
    do {
        exibirMenuGerente();
        opcao = lerInt("Escolha uma opcao: ");
        switch (opcao) {
            case 1: cadastrarConteudoInterativo(); break;
            case 2: removerConteudoInterativo(); break;
            case 3: listarConteudos(); break;
            case 0: cout << "Saindo da area do gerente.\n"; break;
            default: cout << "Opcao invalida.\n";
        }
        if (opcao != 0) {
            cout << "\nPressione ENTER para continuar...";
            cin.get();
        }
    } while (opcao != 0);
}

// ---------- Menu principal ----------

void exibirMenu() {
    limparTela();
    cout << "\n===================================================\n";
    cout << "   SISTEMA DE RECOMENDACAO - STREAMING (C++)\n";
    cout << "===================================================\n";
    cout << "1 - Listar conteudos cadastrados\n";
    cout << "2 - Obter recomendacao (navegar pela arvore de decisao)\n";
    cout << "3 - Avaliar conteudo (estrelas) [extra]\n";
    cout << "4 - Ver historico dos mais assistidos\n";
    cout << "5 - Ver estatisticas do sistema\n";
    cout << "6 - Area do Gerente (login administrativo)\n";
    cout << "0 - Salvar e sair\n";
    cout << "===================================================\n";
}

int main() {
    raiz = construirArvore(1);

    carregarCatalogo();
    if (!catalogo) inicializarCatalogoPadrao();
    carregarHistorico();
    carregarEstatisticas();

    cout << "Bem-vindo(a) ao Sistema de Recomendacao de Conteudo em Streaming!\n";

    int opcao;
    do {
        exibirMenu();
        opcao = lerInt("Escolha uma opcao: ");
        switch (opcao) {
            case 1: listarConteudos(); break;
            case 2: processoDeRecomendacao(); break;
            case 3: avaliarConteudoInterativo(); break;
            case 4: exibirHistorico(); break;
            case 5: exibirEstatisticas(); break;
            case 6: areaDoGerente(); break;
            case 0:
                salvarTudo();
                cout << "\nDados salvos. Ate logo!\n";
                break;
            default:
                cout << "Opcao invalida.\n";
        }
        if (opcao != 0 && opcao != 6) {
            cout << "\nPressione ENTER para continuar...";
            cin.get();
        }
    } while (opcao != 0);

    // libera toda a memoria alocada dinamicamente
    liberarArvore(raiz);
    liberarHistorico();
    while (catalogo) {
        Conteudo* tmp = catalogo;
        catalogo = catalogo->prox;
        delete tmp;
    }

    return 0;
}
