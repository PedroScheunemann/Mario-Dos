#include "telaCarregarMapaRankingContinuar.h"
#include "telaNovoJogo.h"
#include "movimentacao.h"
#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>


// inicializando as variaveis pra armazenar a string do nome digitado
char numeroFase[2]= "\0";
int contadorNrosFase = 0;
// variavel Flag pra saber se o usuario terminou de digitar o nome
int faseInserida = 0;
// variavel Flag pra saber se o programa já leu o arquivo bin do ranking
int arquivoBinLido = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// CARREGAR MAPA - TELA DO NRO DA FASE //////////////////////////////////////////////////////////////////

void desenhaNomeFase(char *nroFase, Font *fontMario){

    // mensagem digite seu nome
    const char msgDigiteAfase[20] = "nro da fase:";
    // posição e tamanho da caixa de texto para inserir o nome
    Rectangle caixaNomeFase = {(screenWidth/2)+20, (screenHeight/2)-50, 170, 100};
    // posição da mensagem digite seu nome
    Vector2 posiDigiteAfase = {caixaNomeFase.x-270, caixaNomeFase.y + 45};


    // desenhando a mensagem digite seu nome
    DrawTextEx(*fontMario, msgDigiteAfase, posiDigiteAfase, 28, 4, WHITE);
    // desenhando a caixa
    DrawRectangleRounded(caixaNomeFase, 0.8, 0, LIGHTGRAY);
    DrawRectangleRoundedLines(caixaNomeFase, 0.8, 0, 2, BLUE);
    // desenhando o nome escrito no array com as teclas digitadas pelo usuário
    DrawText(nroFase, caixaNomeFase.x + 58, caixaNomeFase.y + 30, 50, BLACK);

    // desenhando o _, para ficar com animação de digitação
    if (contadorNrosFase < 2){
        DrawText("_", caixaNomeFase.x + 60 + MeasureText(nroFase, 50), caixaNomeFase.y + 40, 50, BLACK);
    }
}

void desenhaTelaFase(char *nroFase, Font *fontMario){

    BeginDrawing();
    ClearBackground(BLACK);

    desenhaNomeFase(nroFase, fontMario);
    desenhaVoltar(fontMario);

    EndDrawing();
}

void atualizaTelaFase(char *nroFase, MENU *menuJogo, MIDIA *allMidias){

    atualizaNome(nroFase, 2, &contadorNrosFase, &faseInserida, allMidias);
    atualizaVoltar(menuJogo);
    if (menuJogo->optionSelected == 0){
        for (int i = contadorNrosFase; i >= 0; i--) nroFase[i] = '\0';  // reinicializando o array com o nome escrito pra voltar pro menu
        contadorNrosFase = 0;
    }
}

void construirTelaNroFase(char *nroFase, MENU *menuJogo, MIDIA *allMidias){

    atualizaTelaFase(nroFase, menuJogo, allMidias);
    desenhaTelaFase(nroFase, &allMidias->fonteMario);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// CARREGAR MAPA - TELA DA GAMEPLAY /////////////////////////////////////////////////////////////////////

void carregarTelaGameplay(char *nroFase, MENU *menuJogo, MIDIA *allMidias){

    int fase, pontuacaoMario = 0, vidasMario = 3, usosPower = 3;

    // transformando o nro da fase de char para inteiro
    fase = atoi(nroFase);

    // carregando o looping da gameplay
    loopingGameplay(fase, &pontuacaoMario, vidasMario, usosPower, menuJogo, allMidias);

    for (int i = contadorNrosFase; i >= 0; i--) nroFase[i] = '\0';  // reinicializando o array com o numero escrito pra voltar pro menu
    contadorNrosFase = 0;
    faseInserida = 0;                                               // reinicializando a flag pra passar pra tela de gameplay depois de inserir a fase

}

void construirTelaCarregarMapa(MENU *menuJogo,  MIDIA *allMidias){

    // se a fase não foi inserida, atualiza e desenha a tela de inserir o nome
    if (!faseInserida) construirTelaNroFase(numeroFase, menuJogo, allMidias);
    // senão, atualiza e desenha a tela do jogo;
    else carregarTelaGameplay(numeroFase, menuJogo, allMidias);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////// TELA RANKING /////////////////////////////////////////////////////////////////////////////////


void desenhaTelaRanking(MENU *menuJogo, MIDIA *allMidias, int pontuacaoPrimeiro, int pontuacaoSegundo, int pontuacaoTerceiro, int pontuacaQuarto, int pontuacaoQuinto, char *nome1, char *nome2, char *nome3, char *nome4, char *nome5){

     // mensagens na tela inicial
    const char msgRanking[10] = "Ranking";

    // tamanho das letras das mensagens
    const int tamanhoLetra = 28;
    const int espacamento = 4;

    // pontuações em char
    char pontuacaoPrimeiroEmChar[10], pontuacaoSegundoEmChar[10], pontuacaoTerceiroEmChar[10], pontuacaQuartoEmChar[10], pontuacaoQuintoEmChar[10];

    // mensagem "ranking"
    Vector2 posiRanking = {(screenWidth/2)-125, screenHeight*3/14};
    // vetores com as posições de cada nome na tela
    Vector2 posi1 = {430, (screenHeight*5/14)+5};
    Vector2 posi2 = {430, (screenHeight*6/14)+5};
    Vector2 posi3 = {430, (screenHeight*7/14)+5};
    Vector2 posi4 = {430, (screenHeight*8/14)+5};
    Vector2 posi5 = {430, (screenHeight*9/14)+5};
    // vetores com as posições de cada pontuação na tela
    Vector2 posiPont1 = {740, (screenHeight*5/14)+5};
    Vector2 posiPont2 = {740, (screenHeight*6/14)+5};
    Vector2 posiPont3 = {740, (screenHeight*7/14)+5};
    Vector2 posiPont4 = {740, (screenHeight*8/14)+5};
    Vector2 posiPont5 = {740, (screenHeight*9/14)+5};
    // vetores com as posições de cada número na tela
    Vector2 posiNum1 = {370, screenHeight*5/14};
    Vector2 posiNum2 = {370, screenHeight*6/14};
    Vector2 posiNum3 = {370, screenHeight*7/14};
    Vector2 posiNum4 = {370, screenHeight*8/14};
    Vector2 posiNum5 = {370, screenHeight*9/14};
    // posições dos retangulos coloridos
    Rectangle frame1 = {posiNum1.x-20,  posiNum1.y-20, 520, 270};
    Rectangle frame2 = {frame1.x-5,  frame1.y-5, frame1.width+10, frame1.height+10};
    Rectangle frame3 = {frame2.x-5,  posiRanking.y-30, frame2.width+10, (frame2.height+110)};

    // transformando as pontuações de inteiro pra char
    sprintf(pontuacaoPrimeiroEmChar, "%d", pontuacaoPrimeiro);
    sprintf(pontuacaoSegundoEmChar, "%d", pontuacaoSegundo);
    sprintf(pontuacaoTerceiroEmChar, "%d", pontuacaoTerceiro);
    sprintf(pontuacaQuartoEmChar, "%d", pontuacaQuarto);
    sprintf(pontuacaoQuintoEmChar, "%d", pontuacaoQuinto);


    BeginDrawing();
    ClearBackground(BLACK);

    // desenhando "Ranking"
    DrawTextEx(allMidias->fonteMario, msgRanking, posiRanking, tamanhoLetra+24, espacamento, WHITE);
    // desenhando todas os nomes em branco
    DrawTextEx(allMidias->fonteMario, nome1, posi1, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, nome2, posi2, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, nome3, posi3, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, nome4, posi4, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, nome5, posi5, tamanhoLetra, espacamento, WHITE);
    // desenhando todas as pontuações em branco
    DrawTextEx(allMidias->fonteMario, pontuacaoPrimeiroEmChar, posiPont1, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, pontuacaoSegundoEmChar, posiPont2, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, pontuacaoTerceiroEmChar, posiPont3, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, pontuacaQuartoEmChar, posiPont4, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, pontuacaoQuintoEmChar, posiPont5, tamanhoLetra, espacamento, WHITE);
    // desenhando os numeros em azul
    DrawTextEx(allMidias->fonteMario, "1", posiNum1, tamanhoLetra+10, espacamento, BLUE);
    DrawTextEx(allMidias->fonteMario, "2", posiNum2, tamanhoLetra+10, espacamento, BLUE);
    DrawTextEx(allMidias->fonteMario, "3", posiNum3, tamanhoLetra+10, espacamento, BLUE);
    DrawTextEx(allMidias->fonteMario, "4", posiNum4, tamanhoLetra+10, espacamento, BLUE);
    DrawTextEx(allMidias->fonteMario, "5", posiNum5, tamanhoLetra+10, espacamento, BLUE);
    // retangulo em volta
    DrawRectangleLinesEx(frame1, 5, GREEN);
    DrawRectangleLinesEx(frame2, 5, BLUE);
    DrawRectangleLinesEx(frame3, 5, WHITE);
    // desenha o voltar
    desenhaVoltar(&allMidias->fonteMario);

    EndDrawing();
}

void atualizaTelaRanking(MENU *menuJogo){

    atualizaVoltar(menuJogo);
    if (menuJogo->optionSelected == 0) arquivoBinLido = 0;  // reinicializando a flag de ler o arquuvo binario
}

void construirTelaRanking(MENU *menuJogo, MIDIA *allMidias){

    int pontuacaoPrimeiro, pontuacaoSegundo, pontuacaoTerceiro, pontuacaQuarto, pontuacaoQuinto;
    char nome1[MAX_CARACTERES+1], nome2[MAX_CARACTERES+1], nome3[MAX_CARACTERES+1], nome4[MAX_CARACTERES+1], nome5[MAX_CARACTERES+1];

    // se o arquivo do ranking não foi lido, lê
    if (!arquivoBinLido) lerArquivoBinRanking(&arquivoBinLido, &pontuacaoPrimeiro, &pontuacaoSegundo, &pontuacaoTerceiro, &pontuacaQuarto, &pontuacaoQuinto, nome1, nome2, nome3, nome4, nome5);

    // atualiza e desenha a tela de ranking
    atualizaTelaRanking(menuJogo);
    desenhaTelaRanking(menuJogo, allMidias, pontuacaoPrimeiro, pontuacaoSegundo, pontuacaoTerceiro, pontuacaQuarto, pontuacaoQuinto, nome1, nome2, nome3, nome4, nome5);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////// TELA CONTINUAR /////////////////////////////////////////////////////////////////////////////////


void construirTelaContinuar(MENU *menuJogo,  MIDIA *allMidias){

    // inicializando as informações da fase
    FASE infoFase = {0};
    int fase;
    // inicializando o contador de frames e o temporizador das moedas
    float framesCounter, relogioMoedas;
    int pontuacaoMario, vidasMario, usosPower;
    // inicializando as variaveis pra armazenar a string do nome digitado
    char nome[MAX_CARACTERES + 1] = "\0";

    // lendo as informações do arquivo pra criar as estruturas
    lerArquivoContinuarPT1(&infoFase, &fase, &pontuacaoMario, &vidasMario, &usosPower, &framesCounter, &relogioMoedas, nome);

    // cria as estruturas de acordo com o arquivo lido
    INIMIGO tartarugas[infoFase.numMaxTartarugas], caranguejos[infoFase.numMaxCaranguejos];
    OBSTACULO plataformas[infoFase.numPlataformas+1], canos[infoFase.numCanos], retornosCanos[infoFase.numRetornosCanos];
    MOEDA moedas[infoFase.numMaxMoedas];
    POWER botaoPower;
    MARIO mario = {0};

    // lendo as informações do arquivo pra atualizar as estruturas criadas
    lerArquivoContinuarPT2(&mario, tartarugas, caranguejos, plataformas, canos, retornosCanos, moedas, &botaoPower, infoFase);

    // enquanto o mario tiver mais que 0 vidas, atualiza e desenha a tela de gameplay
    while(vidasMario > 0){

        atualizaTelaGameplay(&infoFase, allMidias, &fase, &pontuacaoMario, &vidasMario, &usosPower, &framesCounter, &relogioMoedas, &mario, tartarugas, caranguejos, plataformas, canos, retornosCanos, moedas, &botaoPower, &allMidias->caixaMarioDos);
        desenhaTelaGameplay(infoFase, allMidias, &mario, tartarugas, caranguejos, plataformas, canos, retornosCanos, moedas, &botaoPower);

        // se matou todos inimigos, pula de fase
        if (matouTodosInimigos(tartarugas, caranguejos, infoFase)){ fase++; break; }

        // se apertou Backspace pra voltar, abre o menu do pause
        if (IsKeyPressed(KEY_DELETE)){
            do{
                desenhaTelaPause(infoFase, allMidias, &mario, tartarugas, caranguejos, plataformas, canos, retornosCanos, moedas, &botaoPower);
                // se clicou pra sair do jogo, seta tudo pra sair
                if (WindowShouldClose()){ menuJogo->optionSelect = Sair; break; }
                // se clicou "Backspace", seta pra voltar ao menu
                if (IsKeyPressed(KEY_DELETE)){ menuJogo->optionSelected = 0; break; }
                // se clicou pra salvar, salva o jogo
                if (IsKeyPressed(KEY_S)){
                        salvarArquivoContinuarPT1(&infoFase, &fase, &pontuacaoMario, &vidasMario, &usosPower, &framesCounter, &relogioMoedas, nome);
                        salvarArquivoContinuarPT2(&mario, tartarugas, caranguejos, plataformas, canos, retornosCanos, moedas, &botaoPower, infoFase);
                }
            }while(!IsKeyPressed(KEY_ENTER)); // se clicou enter pra voltar ao jogo, volta
        }

        // se clicou pra sair do jogo, seta a opção pra sair
        if (WindowShouldClose()) menuJogo->optionSelect = Sair;

        // se em algum momento clicou pra sair do jogo, sai
        if(menuJogo->optionSelect == Sair || !menuJogo->optionSelected) break;
    }

    // enquanto o mario tiver 0 vidas, desenha a tela de game over e depois volta pro menu
    while(vidasMario == 0){
        desenhaTelaGameOver(allMidias, framesCounter, plataformas, canos, retornosCanos, &botaoPower, infoFase.numPlataformas, infoFase.numCanos, infoFase.numRetornosCanos);
        framesCounter++;
        if (framesCounter == 380) PlaySound(allMidias->gameOver);  // som da tela de game over
        if (framesCounter > 600){ menuJogo->optionSelected = 0; framesCounter = 0; break; }
        // se clicou pra sair do jogo, sai
        if (WindowShouldClose()){ menuJogo->optionSelect = Sair; break; }
        // se clicou "Backspace", seta pra voltar ao menu
        if (IsKeyPressed(KEY_DELETE)){ menuJogo->optionSelected = 0; break; }
    }

    if (IsSoundPlaying(allMidias->inicio)) StopSound(allMidias->inicio);           // se saiu da gameplay e a musica de inicio de fase está tocando, para
    if (IsSoundPlaying(allMidias->gameEnd)) StopSound(allMidias->gameEnd);         // ou a musica de gameEnd
    if (IsSoundPlaying(allMidias->gameOver)) StopSound(allMidias->gameOver);       // ou a musica de gameOver
    if (IsSoundPlaying(allMidias->marioMorreu)) StopSound(allMidias->marioMorreu); // ou a musica do mario morrer

    // começa looping da gameplay normal dps de ter passado de fase
    loopingGameplay(fase, &pontuacaoMario, vidasMario, usosPower, menuJogo, allMidias);

    // chama a função pra salvar as coisas no arquivo binario de ranking
    escreverArquivoBinRanking(nome, pontuacaoMario);
}














