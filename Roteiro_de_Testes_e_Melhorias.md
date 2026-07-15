# Roteiro de Testes e Melhorias Futuras 

Este documento detalha o script de validação para garantir a integridade do **Sistema de Recomendação de Streaming** e mapeia possíveis melhorias estéticas e funcionais para evolução do projeto.

---

## 🏗️ Caso de Uso 1: Roteiro de Testes Essenciais

### 1. Teste de Integridade do Catálogo (Inserção e Ordenação)
1. **Ação:** Entrar na opção `1. Cadastrar conteúdo`.  
2. **Setup:** Adicionar um filme com inicial 'A' e um filme com inicial 'Z' usando tipos variados.  
3. **Verificação Esperada:** Entrar na opção `3. Listar conteudos`. Todo o catálogo deve ser exibido **estritamente em ordem alfabética**, com o filme 'A' no topo e 'Z' no fim. A numeração de 1-N não deve pular números.

### 2. Teste de Operações Críticas (Remoção Posicional)
1. **Ação:** Entrar na opção `2. Remover conteúdo`.
2. **Cenário A (Limites):** Remover o primeiro filme da lista (ID 1) e o último filme da lista (ID N).
3. **Cenário B (Erro):** Tentar remover o ID 999 (fora dos limites).
4. **Verificação Esperada:** Retornar com sucesso a exclusão, recuar todos os outros para as antigas posições (sem Memory Leaks), e apontar `Índice Não Encontrado` para o Erro 999.

### 3. Teste do Mecanismo de Recomendação e Filtro
1. **Ação:** Entrar na opção `4. Obter recomendação`.
2. **Setup:** Responder a árvore até bater em "Lançamentos Recentes".
3. **Verificação Esperada:** A listagem deve retornar um array, garantindo que TODOS os filmes que surgiram têm que, impreterivelmente, possuir data de lançamento igual ou superior a 2021 (Obrigatoriamente filtrando os clássicos para fora da resposta).

### 4. Teste de Balanceamento do Histórico (Lista Duplamente Encadeada)
1. **Ação:** Selecionar `4. Obter recomendação`, achar o "Filme Z" e apertar `s` para assistir ele **DUAS vezes**. 
2. **Ação:** Assistir a uma "Série X" **TRÊS vezes**.
3. **Verificação Esperada:** Acessar o `5. Histórico de Mais Assistidos`. O sistema deve, de maneira autônoma, reorganizar a série para a primeira posição do terminal (já que 3 > 2), arrastando seus ponteiros antecessor e sucessores de forma íntegra. Além disso, as `8. Estatísticas Completas` devem dizer que o "Filme Z" é o "Filme mais assistido" de seu Tipo/Gênero.

---

## 🚀 Caso de Uso 2: Possíveis Melhorias e Escalabilidade

### 1. Estética Avançada e Experiência do Usuário (Centralização e ASCII)
Atualmente o terminal imprime os dados encostados na borda esquerda. Uma forte melhoria de apresentação seria introduzir formatação ASCII e _Padding_:
```text
      =======================================================
     ||                                                     ||
     ||             🍿 FLIX RECOMENDAÇÕES 🍿               ||
     ||                                                     ||
      =======================================================
```
* **Menu Centralizado**: Usar uma macro ou função iterando um `string(espacos, ' ')` antes do `cout` baseado no tamanho horizontal do CLI (ex: `80` colunas de formatação), gerando um sistema encorpado similar às BIOS antigas.
* **Cores (ANSI Escape Codes)**: Implementar as famosas cores em C++ (`\033[1;31m` para o erro em vermelho; e `\033[1;32m` para O Sucesso em Verde) transformaria completamente a usabilidade.

### 2. Persistência de Dados
No atual nível, o sistema vive inteiramente alocado na memória volátil RAM. O passo natural de evolução seria adicionar as bibliotecas `<fstream>`:
* **Banco de Arquivo `.CSV` ou `.TXT`:** Sempre que o laço principal atingir `0. Sair`, o sistema pega todos os elementos da lista `ListaSimples` (Filmes) e `ListaDupla` (Históricos) iterando-os num `ofstream` para salvar num txt (Export). 
* E no construtor `ListaSimples::ListaSimples()` implementaria um `ifstream` que lê esse arquivo do disco rígido puxando todos para a RAM (Import), mantendo assim seus Filmes cadastrados e histórico preservado sempre que você fechar ou abrir o terminal amanhã.

### 3. Melhoria na Busca (Árvores vs Listas)
A `ListaSimples` ordenada foi eficiente para os requisitos de ED1, mas no mundo real do Streaming com 10.000 filmes, uma inserção numa lista encadeada vai custar O(N) laços de checagem, tornando o software lento. 
* **Refatoração Estratégica**: Substituir, no futuro, todo o módulo do catálogo de conteúdo de Lista Encadeada para uma **Árvore AVL**. Isso reduziria a varredura linear lenta por buscas O(log N) em questão de milissegundos.

### 4. Gestão de Contas (Perfis Múltiplos)
* Hoje os filmes mais assistidos contam para a TV da casa inteira! Mas assim como a Netflix tem o sistema de Perfis (Quem Está Assistindo?), o sistema estaria pronto para criar um módulo `usuarios.cpp`, onde a struct de `ConteudoHistorico` ficaria presa dentro da `Struct Conta`, assim o sistema tem históricos e recomendações isoladas e baseada apenas no comportamento de quem se encotrar logado.
