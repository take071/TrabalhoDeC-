#include "arvore_decisao.h"

#define ESPACO_ARVORE 6

//  inicializa e constroi a arvore
ArvoreDecisao::ArvoreDecisao() {
    raiz = nullptr;
    construirArvore();
}

NodoDecisao* ArvoreDecisao::criarNo(string pergunta, bool ehFolha,
                                     Tipo tipo, Genero genero, FiltroAno filtroAno) {
    NodoDecisao* novo = new NodoDecisao();
    if (novo == nullptr) exit(1);
    novo->pergunta = pergunta;
    novo->ehFolha = ehFolha;
    novo->tipoFiltro = tipo;
    novo->generoFiltro = genero;
    novo->filtroAno = filtroAno;
    novo->esq = nullptr;
    novo->dir = nullptr;
    return novo;
}

void ArvoreDecisao::construirArvore() {

    // === NIVEL 0 ===
    raiz = criarNo("Voce prefere Filmes/Documentarios? (s=sim / n=Series/Anime)", false, OUTRO_TIPO, OUTRO_GENERO);

    // === NIVEL 1 ===
    // Sim -> Filmes/Docs
    raiz->esq = criarNo("Gosta de acao ou aventura?", false, FILME, OUTRO_GENERO);
    // Nao -> Series/Anime
    raiz->dir = criarNo("Prefere anime?", false, SERIE, OUTRO_GENERO);

    // === NIVEL 2 - Lado Filmes ===
    // Sim acao
    raiz->esq->esq = criarNo("Prefere lancamentos recentes (ultimos 5 anos)?", false, FILME, ACAO);
    // Nao acao -> drama ou outros
    raiz->esq->dir = criarNo("Gosta de drama?", false, FILME, OUTRO_GENERO);

    // === NIVEL 2 - Lado Series ===
    // Sim anime
    raiz->dir->esq = criarNo("Prefere anime de acao (shounen)?", false, ANIME, OUTRO_GENERO);
    // Nao anime -> series normais
    raiz->dir->dir = criarNo("Gosta de comedia?", false, SERIE, OUTRO_GENERO);

    // === NIVEL 3 - Filmes Acao ===
    // Acao recente
    raiz->esq->esq->esq = criarNo("Prefere filmes populares (muitas visualizacoes)?", false, FILME, ACAO);
    // Acao classico
    raiz->esq->esq->dir = criarNo("Filmes de Acao Classicos", true, FILME, ACAO, APENAS_CLASSICOS);

    // === NIVEL 3 - Filmes Drama/Outros ===
    // Sim drama
    raiz->esq->dir->esq = criarNo("Prefere drama recente?", false, FILME, DRAMA);
    // Nao drama -> terror ou outros
    raiz->esq->dir->dir = criarNo("Gosta de terror?", false, FILME, OUTRO_GENERO);

    // === NIVEL 3 - Anime ===
    // Anime shounen
    raiz->dir->esq->esq = criarNo("Prefere animes longos (100+ episodios)?", false, ANIME, ACAO);
    // Anime outros generos
    raiz->dir->esq->dir = criarNo("Animes Drama/Diversos", true, ANIME, DRAMA);

    // === NIVEL 3 - Series ===
    // Series comedia
    raiz->dir->dir->esq = criarNo("Series de Comedia", true, SERIE, COMEDIA);
    // Series outros
    raiz->dir->dir->dir = criarNo("Gosta de ficcao cientifica?", false, SERIE, OUTRO_GENERO);

    // === NIVEL 4 (Folhas - profundidade 4-5) ===
    // Filmes acao populares
    raiz->esq->esq->esq->esq = criarNo("Filmes de Acao Populares Recentes", true, FILME, ACAO, APENAS_RECENTES);
    raiz->esq->esq->esq->dir = criarNo("Filmes de Acao Cult/Indie", true, FILME, ACAO);

    // Drama
    raiz->esq->dir->esq->esq = criarNo("Dramas Recentes", true, FILME, DRAMA, APENAS_RECENTES);
    raiz->esq->dir->esq->dir = criarNo("Dramas Classicos", true, FILME, DRAMA, APENAS_CLASSICOS);


    // Terror / Documentario
    raiz->esq->dir->dir->esq = criarNo("Filmes de Terror", true, FILME, TERROR);
    raiz->esq->dir->dir->dir = criarNo("Prefere documentarios de ciencia?", false, DOCUMENTARIO, OUTRO_GENERO);

    // Anime longo / curto
    raiz->dir->esq->esq->esq = criarNo("Animes Shounen Longos", true, ANIME, ACAO);
    raiz->dir->esq->esq->dir = criarNo("Animes Shounen Curtos", true, ANIME, ACAO);

    // Series ficcao / drama
    raiz->dir->dir->dir->esq = criarNo("Series de Ficcao Cientifica", true, SERIE, FICCAO);
    raiz->dir->dir->dir->dir = criarNo("Series de Drama", true, SERIE, DRAMA);

    // === NIVEL 5 (Folhas mais profundas) ===
    // Documentarios
    raiz->esq->dir->dir->dir->esq = criarNo("Documentarios de Ciencia/Natureza", true, DOCUMENTARIO, FICCAO);
    raiz->esq->dir->dir->dir->dir = criarNo("Documentarios Historicos/Sociais", true, DOCUMENTARIO, OUTRO_GENERO);
}

void ArvoreDecisao::navegar(NodoDecisao* no, Tipo &tipoResult, Genero &generoResult, FiltroAno &filtroAnoResult) {
    if (no == nullptr) {
        cout << "\n ERRO: Nodo invalido!\n";
        return;
    }

    if (no->ehFolha) {
        cout << "\n >> Recomendacao: " << no->pergunta << " <<\n";
        tipoResult = no->tipoFiltro;
        generoResult = no->generoFiltro;
        filtroAnoResult = no->filtroAno;
        return;
    }

    char resposta;
    cout << "\n " << no->pergunta;
    cout << "\n (s/n): ";
    cin >> resposta;

    if (resposta == 's' || resposta == 'S') {
        navegar(no->esq, tipoResult, generoResult, filtroAnoResult);  // Sim -> esquerda
    } else {
        navegar(no->dir, tipoResult, generoResult, filtroAnoResult);   // Nao -> direita
    }
}

//  Imprimir arvore 2D (igual print2DUtil do ArvoreB.cpp)
//  Percurso: dir - raiz - esq
void ArvoreDecisao::imprimir(NodoDecisao* no, int espaco) {
    if (no == nullptr) return;

    espaco += ESPACO_ARVORE;

    // Processa direita primeiro
    imprimir(no->dir, espaco);

    cout << "\n";
    for (int i = ESPACO_ARVORE; i < espaco; i++)
        cout << " ";

    if (no->ehFolha)
        cout << "[" << no->pergunta << "]";
    else
        cout << "(" << no->pergunta.substr(0, 20) << "...)";

    // Processa esquerda
    imprimir(no->esq, espaco);
}

void ArvoreDecisao::imprimirArvore() {
    exibirCabecalho("ARVORE DE DECISAO (2D)");
    cout << centralizar("(Esq=Sim / Dir=Nao | [] = folha)") << "\n";
    imprimir(raiz, 0);
    cout << "\n";
}