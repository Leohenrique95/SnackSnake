===============================================================================
                          S N A C K   S N A K E
===============================================================================

Snack Snake e uma versao moderna do classico jogo da cobra, escrita em C.
O jogo apresenta graficos em modo texto (UTF-8), sistema de niveis, 
obstaculos dinamicos e uma Inteligencia Artificial integrada.

[ CARACTERISTICAS ]

* 3 Modos de Dificuldade: Easy, Normal e Hard.
* Power-ups: Coma itens especiais para ganhar pontos extras ou buffs.
* IA (BFS): Algoritmo de busca que encontra o caminho mais curto.
* Ranking: Recordes salvos localmente por categoria.
* Obstaculos: Bombas que surgem e mudam de lugar no modo Hard.

-------------------------------------------------------------------------------
[ COMO EXECUTAR ]
-------------------------------------------------------------------------------

O arquivo executavel (SnackHunt.exe) ja esta incluso neste repositorio.

1. Abra o Windows Terminal, PowerShell ou CMD.
2. Navegue ate a pasta do projeto.
3. IMPORTANTE: Para que os emojis apareçam corretamente, configure o terminal
   para UTF-8 executando o comando:
   
   chcp 65001

4. Inicie o jogo:
   .\SnackHunt.exe

-------------------------------------------------------------------------------
[ CONTROLES ]
-------------------------------------------------------------------------------

NO MENU:
- Setas (Cima/Baixo): Navegar
- Enter: Confirmar
- Esc: Sair

NO JOGO:
- W, A, S, D ou Setas: Controlar a Cobra
- P: Pausar / Retomar
- T: Ativar/Desativar IA (Modo Automatico)
- Q: Voltar para o Menu Principal

-------------------------------------------------------------------------------
[ ITENS E PONTUACAO ]
-------------------------------------------------------------------------------

- Pizza (10 pts): Comida basica.
- Hamburguer (50 pts): Bonus de pontos.
- Bolo: Ativa modo Power-Up (gera muitas pizzas no mapa).
- Nivel (150 pts): Ganho ao acumular pontos; aumenta a velocidade.

-------------------------------------------------------------------------------
[ DETALHES TECNICOS ]
-------------------------------------------------------------------------------

- Linguagem: C
- Algoritmo de IA: BFS (Breadth-First Search)
- Interface: Windows Console API (High Performance)
- Persistencia: Arquivos .txt para o Ranking

===============================================================================
                      Duvidas ou Sugestoes? Verifique o codigo-fonte!
===============================================================================
